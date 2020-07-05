#ifndef _GADGETMENU_H_
#define _GADGETMENU_H_

#include "Arduino.h"
#include "U8glib.h"

#ifndef GADGETMENU_TOTALMENU
  #define GADGETMENU_TOTALMENU 3
#endif

class GadgetMenu {
  private:
    GadgetDisplay *gd;
    U8GLIB *u8g;
    char **listMenu;
    unsigned char totalMenu;
    unsigned char offSet;
    unsigned char positionMenu;
    void drawMenu();
  public:
    GadgetMenu(GadgetDisplay *gd, U8GLIB *u8g){
      this->gd = gd;
      this->u8g = u8g;
      positionMenu = 0;
      totalMenu = 0;
    }
    
    void setMenu(char **listMenu, unsigned char totalMenu){
      this->listMenu = listMenu;
      this->totalMenu = totalMenu > GADGETMENU_TOTALMENU ? GADGETMENU_TOTALMENU : totalMenu;
    }
    
    void draw(){
      this->u8g->firstPage();
      do{
        drawMenu();
        gd->drawTitle();
      }while(u8g->nextPage());
    }

    void setTitle(char *str){
      this->gd->setTitle(str);
    }
    
    void nextMenu(){
      positionMenu++;
      positionMenu %= totalMenu;
    }
    
    void prevMenu(){
      positionMenu += totalMenu;
      positionMenu--;
      positionMenu %= totalMenu;
    }
    unsigned char position(){
      return this->positionMenu;
    }
};

void GadgetMenu::drawMenu()
{
  unsigned int x = 1, y = 33;
  unsigned char menuShowed = this->totalMenu > GADGETMENU_TOTALMENU ? GADGETMENU_TOTALMENU : this->totalMenu;
  unsigned char offSet = this->positionMenu < GADGETMENU_TOTALMENU ? 0 : this->positionMenu - GADGETMENU_TOTALMENU - 1;
  
  if(positionMenu < offSet || positionMenu >= offSet + GADGETMENU_TOTALMENU - 1){
    if(this->offSet > offSet)
      this->offSet--;
     else
      this->offSet = offSet;
  }

  const char *menu[menuShowed]; 
  u8g->setFont(u8g_font_9x18);
  for(unsigned char i = 0;i<menuShowed;i++){
    menu[i] = this->listMenu[i+this->offSet];
    if(this->positionMenu - this->offSet == i){
      this->u8g->drawStr(x+8, y+(i*14), ">");
      this->u8g->drawStr(x+16, y+(i*14), menu[i]);  
    }
    else
      this->u8g->drawStr(x+16, y+(i*14), menu[i]);
  }
  
}

#endif
