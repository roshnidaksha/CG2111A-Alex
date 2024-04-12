#include <stdarg.h> 
#include <serialize.h>

#include "packet.h"
#include "constants.h"

#include <math.h>

volatile TDirection dir;
int MANUAL_MODE = false;

/*
 * Alex's configuration constants
 */

// PI, for calculating circumference
#define PI 3.141592654

// Number of ticks per revolution from the wheel encoder.
#define COUNTS_PER_REV 4

// Wheel circumference in cm.
// We will use this to calculate forward/backward distance traveled 
// by taking revs * WHEEL_CIRC
#define WHEEL_CIRC (6.2 * PI)

// Alex's length and breadth in cm.
#define ALEX_LENGTH 25.8
#define ALEX_BREADTH 16

// Alex's diagonal. We compute and store this once since
// it is expensive to compute and never changes.
float alexDiagonal = 0.0;

// Alex's turning circumference, calculated once. We
// assume that Alex "turns on a dime"
float alexCirc = 0.0;


/*
 *    Alex's State Variables
 */

// Store the ticks from Alex's left and
// right encoders.
volatile unsigned long leftForwardTicks; 
volatile unsigned long rightForwardTicks;
volatile unsigned long leftReverseTicks; 
volatile unsigned long rightReverseTicks;

// Store the revolutions on Alex's left
// and right wheels
volatile unsigned long leftForwardTicksTurns;
volatile unsigned long rightForwardTicksTurns;
volatile unsigned long leftReverseTicksTurns;
volatile unsigned long rightReverseTicksTurns;

// Forward and backward distance traveled
volatile unsigned long forwardDist;
volatile unsigned long reverseDist;

// Variables to keep track of whether we've
// moved a command distance
unsigned long deltaDist;
unsigned long newDist;

// Variables to keep track of our turning angle
unsigned long deltaTicks;
unsigned long targetTicks;


// Function to estimate number of wheel ticks
// needed to turn an angle
unsigned long computeDeltaTicks(float ang) {
  // We will assume that angular distance moved = linear distance moved in one wheel
  // revolution. This is (probably) incorrect but simplifies calculation.
  // # of wheel revs to make one full 360 turn is alexCirc / WHEEL_CIRC
  // This is for 360 degrees. For ang degrees, it will be (ang * alexCirc) / (360 * WHEEL_CIRC)
  // To convert to ticks, we multiply by COUNTS_PER_REV.

  unsigned long ticks = (unsigned long)((ang * alexCirc * COUNTS_PER_REV) / (360.0 * WHEEL_CIRC));

  return ticks;
}


void left(float ang, float speed) {
  if(ang == 0) { 
    deltaTicks = 99999999; 
  }
  else { 
    deltaTicks = computeDeltaTicks(ang); 
  }
  targetTicks = leftReverseTicksTurns + deltaTicks;
  ccw(ang, speed);
}

void right(float ang, float speed) {
  if(ang == 0) { 
    deltaTicks = 99999999; 
  }
  else { 
    deltaTicks = computeDeltaTicks(ang); 
  }
  targetTicks = rightReverseTicksTurns + deltaTicks;
  cw(ang, speed);
}


void handleCommand(TPacket *command) {
  switch(command->command) {
    // For movement commands, param[0] = distance, param[1] = speed.
    case COMMAND_GET_STATS:
      sendOK();
      sendStatus();
      break;

    case COMMAND_CLEAR_STATS:
      sendOK();
      clearOneCounter(command->params[0]); 
      break;

    case COMMAND_FORWARD:
      sendOK();
      if (MANUAL_MODE) forward((float) command->params[0], (float) command->params[1]);
      else forward(5.0, 40);
      break;

    case COMMAND_REVERSE:
      sendOK();
      if (MANUAL_MODE) backward((float) command->params[0], (float) command->params[1]);
      else backward(5.0, 40);
      break;

    case COMMAND_TURN_LEFT:
      sendOK();
      if (MANUAL_MODE) left((float) command->params[0], (float) command->params[1]);
      else left(20, 50);
      break;

    case COMMAND_TURN_RIGHT:
      sendOK();
      if (MANUAL_MODE) right((float) command->params[0], (float) command->params[1]);
      else right(20, 50);
      break;

    case COMMAND_STOP:
      sendOK();
      stop();
      break;

    case COMMAND_COLOR:
      getcolor();
      sendColor();
      sendOK();
      break;

    case COMMAND_MANUAL:
      MANUAL_MODE = !MANUAL_MODE;
      sendOK();
      break;

    case COMMAND_DIST:
      sendDistance();
      sendOK();
      break;
        
    default:
      sendBadCommand();
  }
}

void handlePacket(TPacket *packet) {
  switch(packet->packetType) {
    case PACKET_TYPE_COMMAND:
      handleCommand(packet);
      break;

    case PACKET_TYPE_RESPONSE:
      break;

    case PACKET_TYPE_ERROR:
      break;

    case PACKET_TYPE_MESSAGE:
      break;

    case PACKET_TYPE_HELLO:
      break;
  }
}


void setup() {
  // put your setup code here, to run once:
  alexDiagonal = sqrt((ALEX_LENGTH * ALEX_LENGTH) + (ALEX_BREADTH *  ALEX_BREADTH)); 
  alexCirc = PI  * alexDiagonal; 

  cli();
  setupEINT();
  setupSerial();
  startSerial();
  setupColor();
  setupUltrasonic();
  enablePullups();
  initializeState();
  sei();
}

void loop() {

  TPacket recvPacket; // This holds commands from the Pi
  TResult result = readPacket(&recvPacket);
  
  if(result == PACKET_OK) {
    handlePacket(&recvPacket);
  }
  else if(result == PACKET_BAD) {
    sendBadPacket();
  }
  else if(result == PACKET_CHECKSUM_BAD) {
    sendBadChecksum();
  } 

  if(deltaDist > 0) { 
    if(dir == FORWARD) { 
      if(forwardDist > newDist) { 
        deltaDist = 0; 
        newDist = 0; 
        stop(); 
      } 
    } 
    else if(dir == BACKWARD) { 
      if(reverseDist > newDist) { 
        deltaDist = 0; 
        newDist = 0; 
        stop(); 
      } 
    } 
    else if(dir == STOP) { 
      deltaDist = 0; 
      newDist = 0; 
      stop(); 
    } 
  } 

  if (deltaTicks > 0) {
    if (dir == LEFT) {
      if (leftReverseTicksTurns >= targetTicks) {
        deltaTicks = 0;
        targetTicks = 0;
        stop();
      }
    }
    else if (dir == RIGHT) {
      if (rightReverseTicksTurns >= targetTicks) {
        deltaTicks = 0;
        targetTicks = 0;
        stop();
      }
    }
    else if (dir == STOP) {
      deltaTicks = 0;
      targetTicks = 0;
      stop();
    }
  }    
}
