#include <TMAG5170.h>
TMAG5170 sensor(31);


void setup() {

  pinMode(31, OUTPUT);
  pinMode(13, OUTPUT);
  sensor.setOperatingMode(OPERATING_MODE_activeMeasureMode);
  sensor.setAverage(CONV_AVG_1x);
  sensor.enableChannels(false, false, true);
  sensor.setRange(X_RANGE_300mT, Y_RANGE_300mT, Z_RANGE_300mT);
  sensor.setTriggerMode(TRIGGER_MODE_SPI);
  
  Serial.begin(9600);


}


unsigned x;
unsigned flush;



void loop() {
  while(Serial.available() != 0){
    x = sensor.getBRaw(0xB);
    Serial.write(sensor.getBRaw(0xB));
    flush = Serial.read();
  }
  
  
}
