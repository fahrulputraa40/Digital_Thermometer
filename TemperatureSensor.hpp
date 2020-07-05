#ifndef _TEMPERATURE_SENSOR_H_
#define _TEMPERATURE_SENSOR_H_
#include "Arduino.h"
#include <Adafruit_MLX90614.h>

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

float Temperature_Sensor_value;

extern Adafruit_MLX90614 mlx;
extern float Temperature_Sensor_value;
#endif
