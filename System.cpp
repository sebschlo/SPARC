#include "globals.h"

extern Comms wireless;

System::System(){
    _light_on = false;
    _light_pin = 13;
    _light_time = 0;
    _light_length = 1000000;
}

void System::system_init(){
  pinMode(_light_pin, OUTPUT);
}

void System::system_loop(unsigned long time){
  
  if (_light_on && (time - _light_time >= _light_length)){
    digitalWrite(_light_pin, LOW);
    _light_on = false;
  }
  
}

void System::flash_light(){
  digitalWrite(_light_pin, HIGH);
  _light_time = micros();
  _light_on = true;
}


