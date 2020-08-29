#ifndef _INTERFACE_H_
#define _INTERFACE_H_

#include "Arduino.h"
#include "U8glib.h"
#include "GadgetDisplay.hpp"
#include "GadgetMenu.hpp"
#include "Localization.hpp"
#include "TemperatureSensor.hpp"
#include <EEPROM.h>

#define Button_Reading_Pin        2
#define ADDRESS_SOUNDACTIVATED    0
#define ADDRESS_GAIN              1
#define Button_Speaker_Pin        3

unsigned int soundActivated;
unsigned char Buntton_Reading_state, Buntton_Speaker_state;
float sensor_gain;
unsigned long timer;

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
    {
      Interface_menuDisplay();
      if(Buntton_Speaker_state){
        Buntton_Speaker_state = 0;
        gm.nextMenu();
      }
      
      if(Buntton_Reading_state){
        Buntton_Reading_state = 0;
        switch(gm.position())
        {
          case 0:
              soundActivated++;
              soundActivated %= 2;
              EEPROM.write(ADDRESS_SOUNDACTIVATED, soundActivated);
          break;
          case 1:
            Interface_page = 2;
          break;
          case 2: 
            Interface_page = 0;
            delay(100);
          break;
        }
      }
    }
      break;
    case 2: //calibration temperature sensor
      Interface_mainDisplayLoop();
      if(!digitalRead(Button_Speaker_Pin)){
        while(!digitalRead(Button_Speaker_Pin))
          delay(5);
        sensor_gain -= 0.05;
      }
      if(!digitalRead(Button_Reading_Pin)){
        timer = millis();
        while(!digitalRead(Button_Reading_Pin) && millis() - timer < 2000)
          delay(5);
         if(millis() - timer < 2000)
          sensor_gain += 0.05;
         else{
          EEPROM.put(ADDRESS_GAIN, sensor_gain);
         Interface_page = 0;
         }
         
      }
      Buntton_Reading_state = 0;
      Buntton_Speaker_state = 0;
      break;
    default:// main display
      Interface_mainDisplayLoop();
      if(Buntton_Speaker_state){
        Buntton_Speaker_state = 0;
        Interface_page = 1;
        gm.setPos(0);
      Buntton_Reading_state = 0;
      Buntton_Speaker_state = 0;
      }
      //      Interface_page = 1;
      break;
  }

}

void Interface_mainDisplayLoop() {
  char buffer[21];
  char buf[6];
  strcpy_P(buffer, Interface_page == 2 ? (char *)pgm_read_word(&(lang[Calibration])) : (char *)pgm_read_word(&(lang[Temperature])));
  gd.setTitle(buffer);
  strcpy_P(buffer, (char *)pgm_read_word(&(lang[Read])));
  gd.setValue(buffer, dtostrf(Temperature_Sensor_value*sensor_gain, 2, 1, buf), 0xB0);
  gd.draw();
}

void Interface_menuDisplay() {
  char menu[3][21];
  char sound[5];
  char buff[20];
  char **listMenu, *listMenu_[3];
  strcpy_P(menu[1], (char *)pgm_read_word(&(lang[Calibration])));
  strcpy_P(buff, (char *)pgm_read_word(&(lang[Sound])));
  strcpy_P(sound, soundActivated == 1 ? (char *)pgm_read_word(&(lang[Sound_on])) : (char *)pgm_read_word(&(lang[Sound_off])));
  sprintf(menu[0], buff, sound);
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
extern unsigned char Buntton_Reading_state, Buntton_Speaker_state;
extern float sensor_gain;
#endif
