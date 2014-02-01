#include "globals.h"

SoftwareSerial _gps_serial(10, 11);

GPS::GPS(){
  
  _max_size = 100;    
  _packet_size = 0;
    
  _gps_mode = 0;
  _gps_sv = 0;
  _gps_week = 0;
  _gps_tow = 0;
  _gps_lat = 0;
  _gps_long = 0;
  _gps_alt = 0;
  _gps_gdop = 0;
  
}

void GPS::gps_begin(){
  
  // Set GPS baud rate to 38400
  _gps_serial.begin(9600);
  byte serialCommand[4] = {5, 0, 3, 0};
  gps_send(serialCommand,4);
  delay(300);
  _gps_serial.begin(38400);
  // Turn off output temporarily.
  byte offCommand[3] = {9, 0, 0};
  gps_send(offCommand,3);
  delay(1200);
  // Set power mode: 0xC (Normal, Update to SRAM);
  byte powerCommand[3] = {0x0C, 0, 0};
  gps_send(powerCommand,3);
  // Configure position update rate to 1Hz: 0x0E (4Hz update, update to SRAM)
  byte rateCommand[3] = {0x0E, 4, 0};
  gps_send(rateCommand,3);
  // Configure nav output message interval: 0x11 (Output for every 4x updates, update to SRAM)
  byte intCommand[3] = {0x11, 4, 0}; 
  gps_send(intCommand,3);
  // Configure WAAS: 0x37 (Enable WAAS, Update to SRAM)
  byte waasCommand[3] = {0x37, 1, 0};
  gps_send(waasCommand,3);
  // Disable pinning: 0x39, 0x3B
  byte pinningCommand1[2] = {0x39, 0};
  gps_send(pinningCommand1,2);
  byte pinningCommand2[11] = {0x3B, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  gps_send(pinningCommand2,11);
  // Set filtering to pedestrian mode: 0x3C (Pedestrian mode, update to SRAM)
  byte filteringCommand[3] = {0x3C, 1, 0};
  gps_send(filteringCommand,3);
  // Set to output binary instead of NMEA: 0x9 (Binary message, Update to SRAM)
  byte binaryCommand[3] = {9, 2, 0};
  gps_send(binaryCommand,3);
  
}

void GPS::gps_read(){
  
  static int iter = 0;
  static int state = 0;
  
  if (_gps_serial.available())
  {
    byte ch = _gps_serial.read();
    switch(state){
      case 0:
        if(ch == 0xA0){
          state = 1;
        }
        break;
      case 1:
        state = 2 * (ch == 0xA1);
        break;
      case 2:
        state = 3;
        break;
      case 3:
        _packet_size = ch;
        iter = 0;
        memset(&_gps_packet[0], 0, _max_size);
        state = 4;
        break;
      case 4:
        _gps_packet[iter] = ch;
        iter++;
        if (iter >= _packet_size)
          state = 5;
        break;  
      case 5:
        state = 6;
        break;
      case 6:
        state = 7 * (ch == 0x0D);
        break;
      case 7:
        if(ch == 0x0A){
          gps_parse(_gps_packet);
        }
        state = 0;
        break;
    }
  }
}

void GPS::gps_parse(byte *gpsBinary){
  if(gpsBinary[0] == 0xA8){
    _gps_mode = gpsBinary[1];
    _gps_sv   = gpsBinary[2];
    _gps_week = _get_short(gpsBinary, 3);
    _gps_tow  = _get_long(gpsBinary, 5);
    _gps_lat  = _get_long(gpsBinary, 9);
    _gps_long = _get_long(gpsBinary, 13);
    _gps_alt  = _get_long(gpsBinary, 21);
    _gps_gdop = _get_short(gpsBinary, 25);
    gps_print();
  } 
}

void GPS::gps_send(byte *trans, byte length){
  
  byte command[length + 7];
  command[0] = (byte)0xA0;
  command[1] = (byte)0xA1;
  command[2] = (byte)0;
  command[3] = length;
  
  byte i = 0;
  byte cksm = 0;
  for(i = 0; i < length; i++){
    command[i+4] = trans[i];
    cksm ^= trans[i];
  }
  
  command[length + 4] = cksm;
  command[length + 5] = (byte)0x0D;
  command[length + 6] = (byte)0x0A;
  
  Serial.print("Sent: ");
  Serial.println(trans[0]);
  Serial.println("");
  
  _gps_serial.write(command, length + 7);
  delay(300);
  
}

void GPS::gps_print(){
  Serial.print("Mode: ");
  Serial.println(_gps_mode);
  Serial.print("SV: ");
  Serial.println(_gps_sv);
  Serial.print("Week: ");
  Serial.println(_gps_week);
  Serial.print("TOW: ");
  Serial.println(_gps_tow);
  Serial.print("Lat: ");
  Serial.println(_gps_lat);
  Serial.print("Lon: ");
  Serial.println(_gps_long);
  Serial.print("Alt: ");
  Serial.println(_gps_alt);
  Serial.print("GDOP: ");
  Serial.println(_gps_gdop);
  Serial.println("");
}

long GPS::_get_long(byte *arr, int i){
  return ((long)arr[i] << 24) + ((long)arr[i+1] << 16) + ((long)arr[i+2]<<8) + (long)arr[i+3];
}

short GPS::_get_short(byte *arr, int i){
  return ((short)arr[i] << 8) + (short)arr[i+1];
}
