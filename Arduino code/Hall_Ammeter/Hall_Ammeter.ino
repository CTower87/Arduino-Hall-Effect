#include <TMAG5170.h>


// function to collect magnetic field data when using fastest sampling stored as 12 bits
float bit12getB(TMAG5170 &sensor, uint8_t axis, int range) {
  sensor.readReg(axis);  // read channel result register

  int16_t raw;
  raw = (int16_t)sensor.inBuf[1] << 8 | (int16_t)sensor.inBuf[2];
  
  // Serial.println("hi");
  // Serial.println(sensor.inBuf[1]);
  // Serial.println(sensor.inBuf[2]);
  // Serial.println(raw);

  raw = raw >> 4;
  float result;
  result = raw / 4096.f * 2;  // convert into ratio

  switch (range) {
    case RANGE_150mT:
      result *= 150;
      break;
    case RANGE_75mT:
      result *= 75;
      break;
    case RANGE_300mT:
      result *= 300;
      break;
  }

  return result;
}

String removechar(String something){
  int len = something.length();
  String why = "";

  for(int i = 0; i < (len -1); i += 1){
    why = why + something[i];

  }
  return why;
}


TMAG5170 *pSensor = nullptr;

void setup() {
  Serial.begin(31250);

  pinMode(9, OUTPUT);
  pSensor = new TMAG5170(27);
  pSensor->setOperatingMode(OPERATING_MODE_activeMeasureMode);
  pSensor->setAverage(CONV_AVG_1x);
  pSensor->enableChannels(false, true, false);
  pSensor->setRange(X_RANGE_75mT, Y_RANGE_75mT, Z_RANGE_75mT);
  pSensor->setTriggerMode(TRIGGER_MODE_SPI);

}







void loop() {

  

  // wait for serial command
  while(Serial.available() == 0){}

  String serialread = Serial.readStringUntil(';');
  long time = serialread.toInt() *1000;

  //collect 50 samples for background average
  int avarray[50];
  for(int i = 0; i < 50; i += 1){
    avarray[i] = pSensor->getBRaw(0xA);
    delay(10);
    
  }




 // define varibles for time tracking
  long start;
  long elapsed = 0;
  long elapsed_previous = 0;


  long count = 0;
  //create arrays for data collection
  int collectedvalues[5000];
  int collectedtimes[5000];


  //turn on mosfet
  digitalWrite(9, HIGH);

// loop to collect data and time stamps
  start = micros();
  while(elapsed <= time){
    int delta_elapsed = elapsed - elapsed_previous; 
    // time collected
    collectedtimes[count] = delta_elapsed;
    // magnetic field collected
    collectedvalues[count] = pSensor->getBRaw(0xA);
    // bump count recalculate elapsed time
    elapsed_previous = elapsed;
    delayMicroseconds(50);
    elapsed = micros() - start;
 
    count += 1;
  }

  //turn off mosfet
  digitalWrite(9, LOW);

  


// loop to print average data
  for( int i = 0; i < 50; i += 1){
    Serial.println(avarray[i]);
  }



// print count lengh 
  Serial.println(count);



// print time data
  for(int i = 0; i < count; i += 1){
    Serial.println(collectedtimes[i]);
  }


//print value data 
  for(int i = 0; i < count; i += 1){
    Serial.println(collectedvalues[i]);
  
  }



  unsigned flush;
  flush = Serial.read();

}
