#define TRIGGER 41
#define ECHO 40

double distance;
double duration;

void setupUltrasonic() {
  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);
}

void getDistance() {
  // Stop Trigger Pin
  digitalWrite(TRIGGER, LOW);
  delayMicroseconds(2);

  // Send signal for 10 microseconds
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER, LOW);

  // Calculate distance in cm from duration measured in microseconds 
  duration = pulseIn(ECHO, HIGH); 
  distance = (duration * 0.0343) / 2;
}

void sendDistance() {
  uint32_t ultrasonicDist = getDistance();
  
  TPacket distancePacket;
  distancePacket.packetType = PACKET_TYPE_RESPONSE;
  distancePacket.command = RESP_DIST;
  distancePacket.params[0] = ultrasonicDist;
  sendResponse(&distancePacket);
  
  sendOK();
}
