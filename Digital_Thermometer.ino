#include "U8glib.h"
#include "Interface.hpp"
#include <Wire.h>
#include "TemperatureSensor.hpp" 
#include "DFPlayer.hpp"
#include <EEPROM.h>


#define DFPlayer_Busy_Pin     7


unsigned long Interface_timeOut = 0;
unsigned char step = 0;
SoftwareSerial dfplayerSerial(9, 8);
DFPlayer voice(&dfplayerSerial, 7);

void playVoice();
void Button_Speaker();
void Button_Reading();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(Button_Reading_Pin, INPUT_PULLUP);
  pinMode(Button_Speaker_Pin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(Button_Reading_Pin), Button_Reading, FALLING );
  attachInterrupt(digitalPinToInterrupt(Button_Speaker_Pin), Button_Speaker, FALLING );
  dfplayerSerial.begin(9600);
  Interface_init(&lcd);
  voice.init();
  soundActivated = EEPROM.read(ADDRESS_SOUNDACTIVATED) > 0 ? 1 : 0;
  EEPROM.get(ADDRESS_GAIN, sensor_gain);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  
  if (millis() - Interface_timeOut > 500 || Buntton_Speaker_state || Buntton_Reading_state) {
    Interface_timeOut = millis();
    switch(step){
      case 0:
        Interface_loop();
        step++;
        break;
      default:
        Temperature_Sensor_value = mlx.readAmbientTempC();
        Temperature_Sensor_value = isnan(Temperature_Sensor_value) ? 0.0 : Temperature_Sensor_value;
        Temperature_Sensor_value = Temperature_Sensor_value > 99.9 ? 99.9 : Temperature_Sensor_value;
        if(isnan(Temperature_Sensor_value*sensor_gain))
          sensor_gain = 1;
        step = 0;
        break;
    }
  }

  if(Buntton_Reading_state && Interface_page == 0 && step == 0 && soundActivated){
    playVoice(Temperature_Sensor_value*sensor_gain);
    Buntton_Reading_state = 0;
  }
  
}

void playVoice(float num){
  if(isnan(num))
  return;
  unsigned char tens, parse[3] = {0,0,0};
  char buff[5] = {'\0','\0','\0','\0','\0'}, buf[4] = {'\0','\0','\0','\0'};
  sprintf(buff, "%s",dtostrf(num,2,1,buf));
  if(strlen(buff) == 4){
    parse[0] = buff[0]  - '0';
    parse[1] = buff[1]  - '0';
    parse[2] = buff[3]  - '0';
  }
  else if(strlen(buff) == 3){
    parse[1] = buff[0] - '0';
    parse[2] = buff[2] - '0';
  }
  tens = parse[0] * 10 + parse[1];
  for(unsigned int i = 0; i<5;i++){
    if(i == 0 && parse[0] == 0)
      continue;
    else if(i == 2)
      voice.play(1);
    else if(i == 4)
      voice.play(30);
     else if(i == 0 && tens > 9 && tens < 20){
      i++;
      tens -= 10;
      voice.play(19-tens);
     }
     else if(i == 0){
      parse[0] -= 2;
      voice.play(9-parse[0]);
      if(parse[1] == 0)
        i++;
     }
     else{
      voice.play(29 - parse[i > 2 ? 2 : i]);
     }
  }
}

void Button_Speaker(){
   Buntton_Speaker_state = 1;
   delay(15);
}

void Button_Reading(){
    Buntton_Reading_state = 1;
    delay(15);
}

