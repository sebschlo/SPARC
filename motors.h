#ifndef motors_h
#define motors_h

#include "Arduino.h"

class Motors{
  
  public:
    Motors();
    void motor_init();
    void set_left(int pwm);
    void set_right(int pwm);
    
  private:
    int _left_speed;
    int _right_speed;
  
};

#endif
