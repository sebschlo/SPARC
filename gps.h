#ifndef gps_h
#define gps_h

#include "Arduino.h"

class GPS{
  
  public:
    GPS();
    void gps_begin();
    void gps_read();
    void gps_parse(byte *gpsBinary);
    void gps_send(byte *trans, byte length);
    void gps_print();
    
  private:
    int _max_size;
    int _packet_size;
    
    byte _gps_mode;
    byte _gps_sv;
    unsigned int  _gps_week;
    unsigned long _gps_tow;
    long _gps_lat;
    long _gps_long;
    unsigned long _gps_alt;
    int  _gps_gdop;
    byte _gps_packet[100];
  
    long _get_long(byte *arr, int i);
    short _get_short(byte *arr, int i);
    
};

#endif
