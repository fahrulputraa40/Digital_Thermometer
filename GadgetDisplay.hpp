#ifndef _GADGET_DISPLAY_H_
#define _GADGET_DISPLAY_H_

#include "Arduino.h"
#include "U8glib.h"
#include "stdarg.h"

#ifndef GADGETDISPLAY_BUFFER
  #define GADGETDISPLAY_BUFFER 15
 #endif

class GadgetDisplay
{
  private:
    U8GLIB *u8g;
    char value[GADGETDISPLAY_BUFFER];
    unsigned char title[GADGETDISPLAY_BUFFER];
    void clear();
    void drawValue();
  public:
    GadgetDisplay(U8GLIB *u8g);
    void setTitle(const char *str, ...);
    void setValue(const char *str, ...);
    void draw();
    void drawTitle();

};

GadgetDisplay::GadgetDisplay(U8GLIB *u8g){
  this->u8g = u8g;
  clear();
}

void GadgetDisplay::clear(){
  for(unsigned char i=0;i<GADGETDISPLAY_BUFFER;i++){
    title[i] = '\0';
    value[i] = '\0';
  }
}

void GadgetDisplay::setTitle(const char *str, ...){
  va_list arg;
  va_start(arg, str);
  vsnprintf(title, sizeof(title), str, arg);
  va_end(arg);
}

void GadgetDisplay::setValue(const char *str, ...){
  va_list arg;
  va_start(arg, str);
  vsnprintf(value, sizeof(title), str, arg);
  va_end(arg);
}

void GadgetDisplay::draw(){
  u8g->firstPage();
  do{
    drawValue();
    drawTitle();
  }while(u8g->nextPage());
}

void GadgetDisplay::drawValue(){
  unsigned int y = 45, d;
  u8g->setFont(u8g_font_fur20);
  d = u8g->getWidth() - u8g->getStrWidth(this->value);
  d /= 2;
  u8g->drawStr(d, y, this->value);
}

void GadgetDisplay::drawTitle(){
  unsigned int x = 2, y = 15;
  u8g->setFont(u8g_font_9x18);
  u8g->drawStr(x, y, this->title);
  u8g->drawHLine(0,18, 127);
}

#endif
