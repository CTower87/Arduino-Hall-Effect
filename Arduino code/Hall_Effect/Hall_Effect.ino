#include <TMAG5170.h>
TMAG5170 sensor(31);


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

  pinMode(31, OUTPUT);
  pinMode(13, OUTPUT);
  sensor.setOperatingMode(OPERATING_MODE_activeMeasureMode);
  sensor.setAverage(CONV_AVG_1x);
  sensor.enableChannels(false, false, true);
  sensor.setRange(X_RANGE_75mT, Y_RANGE_75mT, Z_RANGE_75mT);
  sensor.setTriggerMode(TRIGGER_MODE_SPI);
  
  Serial.begin(9600);


}







void loop() {
  delay(100);

  // x = sensor.getBRaw(Z_CH_RESULT);
  float x;
  x = bit12getB(sensor, 0xB, 75);

  Serial.println(x,5);
}
