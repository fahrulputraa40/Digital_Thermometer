#ifndef _INTERFACE_H_
#define _INTERFACE_H_

#include "Arduino.h"
#include "U8glib.h"
#include "GadgetDisplay.hpp"
#include "GadgetMenu.hpp"
#include "Localization.hpp"
#include "TemperatureSensor.hpp"

unsigned int soundActivated;

U8GLIB_SSD1306_128X64 lcd(U8G_I2C_OPT_NONE | U8G_I2C_OPT_DEV_0);
GadgetDisplay gd(&lcd);
GadgetMenu gm(&gd, &lcd);

unsigned char Interface_page;

void Interface_mainDisplayLoop();
void Interface_menuDisplay();

void Interface_init(U8GLIB *u8g) {
  if ( u8g->getMode() == U8G_MODE_R3G3B2 ) {
    u8g->setColorIndex(255);     // white
  }
  else if ( u8g->getMode() == U8G_MODE_GRAY2BIT ) {
    u8g->setColorIndex(3);         // max intensity
  }
  else if ( u8g->getMode() == U8G_MODE_BW ) {
    u8g->setColorIndex(1);         // pixel on
  }
  else if ( u8g->getMode() == U8G_MODE_HICOLOR ) {
    u8g->setHiColorByRGB(255, 255, 255);
  }
  //  u8g->setFont(u8g_font_unifont);
}

void Interface_loop() {

  switch (Interface_page) {
    case 1: //page menu
      Interface_menuDisplay();
      break;
     case 2: //calibration temperature sensor
      Interface_mainDisplayLoop(); 
     break;
    default:// main display
      Interface_mainDisplayLoop();
//      Interface_page = 1;
      break;
  }

}

void Interface_mainDisplayLoop() {
  char buffer[21];
  char buf[6];
  strcpy_P(buffer, (char *)pgm_read_word(&(lang[Temperature])));
  gd.setTitle(buffer);
  strcpy_P(buffer, (char *)pgm_read_word(&(lang[Read])));
  gd.setValue(buffer, dtostrf(Temperature_Sensor_value,2,1,buf), 0xB0);
  gd.draw();
}

void Interface_menuDisplay() {
  char menu[3][21];
  char sound[5];
  char **listMenu, *listMenu_[3];
  strcpy_P(menu[1], (char *)pgm_read_word(&(lang[Calibration])));
  strcpy_P(menu[0], (char *)pgm_read_word(&(lang[Sound])));
  strcpy_P(sound, soundActivated == 1 ?(char *)pgm_read_word(&(lang[Sound_on])) : (char *)pgm_read_word(&(lang[Sound_on])));
  sprintf(menu[0], menu[0], sound);
  strcpy_P(menu[2], (char *)pgm_read_word(&(lang[Back])));
  listMenu_[0] = menu[0];
  listMenu_[1] = menu[1];
  listMenu_[2] = menu[2];
  listMenu = listMenu_;
  gm.setMenu(listMenu, 3);
  gm.draw();
}

extern U8GLIB_SSD1306_128X64 lcd;
extern GadgetDisplay gd;
extern unsigned char Interface_page;
#endif
