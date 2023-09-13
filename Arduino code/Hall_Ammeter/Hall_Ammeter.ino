#include <TMAG5170.h>
TMAG5170 sensor(31);

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


void setup() {

  pinMode(31, OUTPUT);
  pinMode(41, OUTPUT);
  sensor.setOperatingMode(OPERATING_MODE_activeMeasureMode);
  sensor.setAverage(CONV_AVG_1x);
  sensor.enableChannels(false, true, false);
  sensor.setRange(X_RANGE_75mT, Y_RANGE_75mT, Z_RANGE_75mT);
  sensor.setTriggerMode(TRIGGER_MODE_SPI);
  Serial.begin(9600);


}


void loop() {



  // wait for serial command
  while(Serial.available() == 0){
  }


  String serialread = Serial.readStringUntil(";");
  serialread = removechar(serialread);
  int time = serialread.toInt() *1000;

  //collect 50 samples for background average
  float avarray[50];
  for(int i = 0; i < 50; i += 1){
    avarray[i] = bit12getB(sensor, 0xA, 75);
    delay(10);
    
  }




 // define varibles for time tracking
  unsigned start;
  unsigned elapsed = 0;


  //create arrays for data collection
  float collectedvalues[1000];
  int collectedtimes[1000];
  int count = 0;

  //turn on mosfet
  digitalWrite(41, HIGH);
// loop to collect data and time stamps
  start = micros();
  while(elapsed <= time){
    // time collected
    collectedtimes[count] = elapsed;
    // magnetic field collected
    collectedvalues[count] = bit12getB(sensor, 0xA, 75);

    // bump count recalculate elapsed time
    elapsed = micros() - start;
    count += 1;
  }

  //turn off mosfet
  digitalWrite(41, LOW);



// loop to print average data
  for( int i = 0; i < 50; i += 1){
    Serial.println(avarray[i], 10);
  }



// print count lengh 
  Serial.println(count);

// print time data
  for(int i = 0; i < count; i += 1){
    Serial.println(collectedtimes[i],10);
  }


//print value data 
  for(int i = 0; i < count; i += 1){
    Serial.println(collectedvalues[i], 10);
  
  }



  unsigned flush;
  flush = Serial.read();

}
