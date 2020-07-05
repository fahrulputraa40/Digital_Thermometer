#include "U8glib.h"
#include "Interface.hpp"
#include <Wire.h>
#include "TemperatureSensor.hpp"
#include "DFPlayer.hpp"

unsigned long Interface_timeOut = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Interface_init(&lcd);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (millis() - Interface_timeOut > 500) {
    Interface_timeOut = millis();
    Interface_loop();
  }
  soundActivated = 0;
  Temperature_Sensor_value = mlx.readAmbientTempC();
}
