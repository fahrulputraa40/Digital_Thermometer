#ifndef _DFPALYER_HPP_
#define _DFPALYER_HPP_

#include "Arduino.h"
#include "Stream.h"
#include "SoftwareSerial.h"

//Command Data Format
#define Start_Byte 0x7E
#define Version_Byte 0xFF
#define Command_Length 0x06
#define End_Byte 0xEF
#define Acknowledge 0x00 //Returns info with command 0x41 [0x01: info, 0x00: no info]

//List Control Command
#define DFPlayer_Next                   0x01
#define DFPlayer_Prev                   0x02
#define DFPlayer_SpesifyTracking        0x03    //0-2999
#define DFPlayer_IncVolume              0x04
#define DFPlayer_DecVolume              0x05
#define DFPlayer_SpecifyVolume          0x06    //0-30
#define DFPlayer_SpecifyEQ              0x07    //(0,1,2,3,4,5) Normal,Pop,Rock,Jazz,Classic,Base
#define DFPlayer_SpecifyPlayMode        0x08    //(0,1,2,3) Repeat,folder repeat/single repeat/random
#define DFPlayer_SpecifyPlaybackSrc     0x09    //(0,1,2,3,4) U/TF/AUX/SLEEP/FLASH
#define DFPlayer_EnterStandbyMode       0x0A
#define DFPlayer_NormalMode             0x0B
#define DFPlayer_Reset                  0x0C
#define DFPlayer_PlayBack               0x0D
#define DFPlayer_Pause                  0x0E
#define DFPlayer_SpecifyFolder          0x0F    //1-10
#define DFPlayer_VolumeAdjustSet        0x10    
#define DFPlayer_RepeatPlay             0x11    //1:start repeat  0:stop repeat

class DFPlayer{
  public:
    uint8_t pinMask;
    uint8_t *port;
    
    DFPlayer(Stream *serial, unsigned char pinBusy = 255);
    void init();
    void setVolume(unsigned char vol);
    void pause();
    void play(unsigned int num = 0);
    void execute_CMD(byte CMD, byte Par1, byte Par2);
    
   private:
    void wait();
    Stream *serial;
};

void DFPlayer::wait(){
  unsigned long timeOut = millis();
  if(port == NULL && pinMask == 0)
    return;
   else{
    while((pinMask & *port) == 0 && millis() - timeOut < 2000)
      delay(1);
   }
}

DFPlayer::DFPlayer(Stream *serial, unsigned char pinBusy){
  this->serial = serial;
  if(pinBusy != 255){
    this->pinMask = digitalPinToBitMask(pinBusy);
    this->port = (uint8_t *)portInputRegister(digitalPinToPort(pinBusy));
  }
  else{
    this->pinMask = 0;
    this->port = NULL;
  }
}

void DFPlayer::init(){
  execute_CMD(0x3F, 0, 0);
  delay(500);
  setVolume(25);
  execute_CMD(0x11, 0, 0);
  delay(500);
}

void DFPlayer::setVolume(unsigned char vol){
  wait();
  execute_CMD(0x06, 0, vol); // Set the volume (0x00~0x30)
  delay(500);
}

void DFPlayer::pause(){
  wait();
  execute_CMD(0x0E, 0, 0);
  delay(50);
}

void DFPlayer::play(unsigned int num){
  wait();
  execute_CMD(0x03, (num & 0xFF00)>>8, num & 0xFF);
  
  delay(50);
}

void DFPlayer::execute_CMD(byte CMD, byte Par1, byte Par2)
{
  word checksum = -(Version_Byte + Command_Length + CMD + Acknowledge + Par1 + Par2);
  // Build the command line
  byte Command_line[10] = { Start_Byte, Version_Byte, Command_Length, CMD, Acknowledge,
                            Par1, Par2, highByte(checksum), lowByte(checksum), End_Byte
                          };
  //Send the command line to the module
  for (byte k = 0; k < 10; k++)
  {
    this->serial->write(Command_line[k]);
  }
}

#endif
