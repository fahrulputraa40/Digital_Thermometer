#ifndef _LOCALIZATION_H_
#define _LOCALIZATION_H_

#include "Arduino.h"

#define Calibration             0
#define Sound                   1
#define Back                    2
#define Sound_on                3
#define Sound_off               4
#define Temperature             5
#define Read                    6

const char CALIBRATION[] PROGMEM = "Calibration";
const char SOUND[] PROGMEM = "Sound:%s";
const char BACK[] PROGMEM = "Back";
const char SOUND_ON[] PROGMEM = "ON";
const char SOUND_OFF[] PROGMEM = "OFF";
const char TEMPERATURE[] PROGMEM = "Temperature";
const char READ[] PROGMEM = "%s%cC";

const char *const lang[] PROGMEM = {
  CALIBRATION,
  SOUND,
  BACK,
  SOUND_ON,
  SOUND_OFF,
  TEMPERATURE,
  READ
};

extern const char *const lang[] PROGMEM;

#endif
