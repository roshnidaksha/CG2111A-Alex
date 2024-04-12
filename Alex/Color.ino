#define S0 26
#define S1 28
#define S2 32
#define S3 34
#define sensorOut 30

int frequency = 0;
int r = 0;
int g = 0;
int b = 0;

void setup() {
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);
  
  // Setting frequency-scaling to 20%
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
}

int getAvgReading(int times) {      
  //find the average reading for the requested number of times of scanning LDR
  int reading;
  int total = 0;

  //take the reading as many times as requested and add them up
  for(int i = 0; i < times; i += 1){
    reading = pulseIn(sensorOut, LOW);
    total = reading + total;
    delay(10);
  }

  //calculate the average and return it
  return total/times;
}

void getColor(){
  // Setting red filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  // Reading the output frequency
  //r = pulseIn(sensorOut, LOW);
  r = getAvgReading(5);
  delay(100);

  // Setting Green filtered photodiodes to be read
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  // Reading the output frequency
  //g = pulseIn(sensorOut, LOW);
  g = getAvgReading(5);
  delay(100);

  // Setting Blue filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  // Reading the output frequency
  //b = pulseIn(sensorOut, LOW);
  b = getAvgReading(5);
  delay(100);
}

void sendColor() {
  TPacket colorPacket;
  colorPacket.packetType = PACKET_TYPE_RESPONSE;
  colorPacket.command = RESP_COLOR;
  
  colorPacket.params[0] = redFreq;
  colorPacket.params[1] = greenFreq;
  colorPacket.params[2] = blueFreq;
  
  sendResponse(&colorPacket);  
}
