/*
 * Usage: ./send <systemCode> <unitCode> <command>
 * Command is 0 for OFF and 1 for ON
 */

#include "RCSwitch.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {

  /*
   * output PIN is hardcoded for testing purposes
   * see https://projects.drogon.net/raspberry-pi/wiringpi/pins/
   * for pin mapping of the raspberry pi GPIO connector
   */
  int PIN = 0;
  char*    systemCode;
  int      unitCode;
  int      command;
  RCSwitch mySwitch = RCSwitch();

  if (argc <= 3) {
    printf ("usage: send <systemCode> <unitCode> <command>\n");
      goto err;
  }

  if (sizeof (argv[1]) != 5) {
    printf ("Systemcode must be 5 bit binary code!\n");
    goto err;
  }

  systemCode = argv[1];
  unitCode   = atoi(argv[2]);
  command    = atoi(argv[3]);

  if (wiringPiSetup () == -1) return 1;
  piHiPri(20);
  printf("sending systemCode[%s] unitCode[%i] command[%i]\n", systemCode, unitCode, command);
  mySwitch.setPulseLength(300);
  mySwitch.enableTransmit(PIN);

  switch(command) {
    case 1:
      mySwitch.switchOn(systemCode, unitCode);
      break;
    case 0:
      mySwitch.switchOff(systemCode, unitCode);
      break;
    case 2:
      // 00001 2 on binary coded
      mySwitch.send("010101010001000101010001");
      break;
    case 3:
      // 00001 2 on as TriState
      mySwitch.sendTriState("FFFF0F0FFF0F");
      break;
    default:
      printf("command[%i] is unsupported\n", command);
      return -1;
  }
  return 0;
err:
  return -1;
}
