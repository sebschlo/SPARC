// Todo: checksum, python sender script, motor functions, listen, change packets to report system information on a regular basis, report GPS
#include <SoftwareSerial.h>
#include "globals.h"

Comms wireless;
System sys;
Motors motors;
GPS gps;

unsigned long time = 0;

void setup() {
  // initialize the LED pin as an output:
  sys.system_init();
  // initialize serial communication:
  wireless.comm_init();
  // initialize gps
  gps.gps_begin();
}

void loop() {
  
  time = micros();
  
  sys.system_loop(time);
  wireless.read_comm();
  gps.gps_read();
}
