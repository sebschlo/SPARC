#ifndef comms_h
#define comms_h

#include "Arduino.h"

class Comms{
  
  public:
    Comms();
    void comm_init();
    void read_comm(); 
    void parse_comm(byte *packet);
    void send_comm(byte *trans, byte length);
    void comm_test();
    void set_baud(byte rate);
    void ack();
    void send_ack();
    void nack();
    void send_nack();
    void comm_listen(int packet_length);
    
  private:
    byte _max_size;
    byte _packet_size;
    long _baud_rate;
    byte _packet[154];
  
};

#endif
