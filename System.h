#ifndef system_h
#define system_h

#include "Arduino.h"

class System{
  
  public:
    System();
    void system_init();
    void flash_light();
    void system_loop(unsigned long time);
    
  private:
    boolean _light_on;
    int _light_pin;
    unsigned long _light_time;
    unsigned long _light_length;
  
};

#endif
