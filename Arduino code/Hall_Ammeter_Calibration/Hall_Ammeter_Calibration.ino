#include <TMAG5170.h>
TMAG5170 sensor(27);


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

  // Serial.println(result ,5);
  return result;
}

void setup() {
  // put your setup code here, to run once:
  sensor.setOperatingMode(OPERATING_MODE_activeMeasureMode);
  sensor.setAverage(CONV_AVG_1x);
  sensor.enableChannels(false, true, false);
  sensor.setRange(X_RANGE_75mT, Y_RANGE_75mT, Z_RANGE_75mT);
  sensor.setTriggerMode(TRIGGER_MODE_SPI);
  
  Serial.begin(31250);

}

void loop() {
  // put your main code here, to run repeatedly:
  while (Serial.available() == 0){
  }

  
  int array[1000];
  
  for(int i = 0; i < 1000; i +=1){
    array[i] = sensor.getBRaw(0xA);
  }

  for(int i = 0; i < 1000; i +=1){
    Serial.println(array[i]);

  }



  unsigned flush;
  flush = Serial.read();
}
