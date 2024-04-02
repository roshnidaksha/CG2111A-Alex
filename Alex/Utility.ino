void waitForHello() {
  int exit = 0;

  while(!exit) {
    TPacket hello;
    TResult result;
    
    do {
      result = readPacket(&hello);
    } while (result == PACKET_INCOMPLETE);

    if(result == PACKET_OK) {
      if(hello.packetType == PACKET_TYPE_HELLO) {
        sendOK();
        exit = 1;
      }
      else {
        sendBadResponse();
      }
    }
    else if(result == PACKET_BAD) {
      sendBadPacket();
    }
    else if(result == PACKET_CHECKSUM_BAD) {
      sendBadChecksum();
    }
  } // !exit
}

/*
 * Alex's setup and run codes
 * 
 */

// Clears all our counters
void clearCounters() {
  leftForwardTicks = 0;
  rightForwardTicks = 0;
  leftReverseTicks = 0;
  rightReverseTicks = 0;

  leftForwardTicksTurns = 0;
  rightForwardTicksTurns = 0;
  leftReverseTicksTurns = 0;
  rightReverseTicksTurns = 0;

  forwardDist = 0;
  reverseDist = 0; 
}

// Clears one particular counter
void clearOneCounter(int which) {
  switch(which) {
    case 0:
      clearCounters();
      break;

    case 1:
      leftForwardTicks = 0;
      break;

    case 2:
      rightForwardTicks = 0;
      break;

    case 3:
      leftReverseTicks = 0;
      break;

    case 4:
      rightReverseTicks = 0;
      break;

    case 5:
      leftForwardTicksTurns = 0;
      break;

    case 6:
      rightForwardTicksTurns = 0;
      break;

    case 7:
      leftReverseTicksTurns = 0;
      break;

    case 8:
      rightReverseTicksTurns = 0;
      break;

    case 9:
      forwardDist = 0;
      break;

    case 10:
      reverseDist = 0;
      break;
  }
}

// Intialize Alex's internal states

void initializeState() {
  clearCounters();
}
