#include "globals.h"

extern System sys;

Comms::Comms(){
  _max_size = 154;
  _packet_size = 0;
  _baud_rate = 115200;
}

void Comms::comm_init(){
  Serial.begin(_baud_rate);
}

void Comms::read_comm(){
  
  static int iter = 0;
  static int state = 0;
  
  while (Serial.available()){
    byte ch = Serial.read();
    switch(state){
      case 0:
        if(ch == 0xA0)
          state = 1;
        break;
      case 1:
        state = 2 * (ch == 0xA1);
        break;
      case 2:
        _packet_size = ch;
        iter = 0;
        memset(&_packet[0], 0, _max_size);
        state = 3;
        break;
      case 3:
        _packet[iter] = ch;
        iter++;
        if (iter >= _packet_size)
          state = 4;
        break;  
      case 4:
        state = 5;
        break;
      case 5:
        state = 6 * (ch == 0x0D);
        break;
      case 6:
        if(ch == 0x0A)
          parse_comm(_packet);
        state = 0;
        break;
      default:
        break;
    }
  }
}

void Comms::parse_comm(byte *packet){
  switch(packet[0]){
    case 10:
      switch(packet[1]){
        case 5:
          comm_test();
          break;
        case 6:
          set_baud(packet[2]);
          break;
        case 7:
          ack();
          break;
        case 8:
          nack();
          break;
        case 9:
          comm_listen(packet[2]);
          break;
        default:
          send_nack();
          break;
      }
      break;
    case 20:
      switch(packet[1]){
        default:
          break;
      }
      break;
    case 21:
      switch(packet[1]){
        default:
          break;
      }
      break;
    case 30:
      switch(packet[1]){
        default:
          break;
      }
      break;
    case 31:
      switch(packet[1]){
        default:
          break;
      }
      break;
    case 40:
      switch(packet[1]){
        default:
          break;
      }
      break;
    case 41:
      switch(packet[1]){
        default:
          break;
      }
      break;
    default:
      send_nack();
      break;
  }
}

void Comms::send_comm(byte *trans, byte length){
  byte command[length + 7];
  command[0] = (byte)0xA0;
  command[1] = (byte)0xA1;
  command[2] = length;
  
  byte i = 0;
  byte cksm = 0;
  for(i = 0; i < length; i++){
    command[i+3] = trans[i];
    cksm ^= trans[i];
  }
  
  command[length + 3] = cksm;
  command[length + 4] = (byte)0x0D;
  command[length + 5] = (byte)0x0A;
  
  Serial.write(command, length + 6);
  delay(300);
}

void Comms::comm_test(){
  sys.flash_light();
  send_ack();
}

void Comms::set_baud(byte rate){
  // DISABLED because xbee needs to also be reconfigured with a new baud rate.
  //long rates[7] = {9600, 14400, 19200, 28800, 38400, 57600, 115200};
  //_baud_rate = rates[rate];
  //Serial.begin(_baud_rate);
  return;
}

void Comms::ack(){
  return;
}

void Comms::send_ack(){
  byte nack_reply[2] = {11, 7};
  send_comm(nack_reply, 2);
}

void Comms::nack(){
  return;
}

void Comms::send_nack(){
  byte nack_reply[2] = {11, 8};
  send_comm(nack_reply, 2);
}

void Comms::comm_listen(int packet_length){
  return;
}
