#include "GeigerMode.hpp"

void TestMode::tick(int clicks, unsigned long time) {
  // Get FTDI data
  String input{""};

  int bufferAvailable{counterSerial.available()};
  if (bufferAvailable > 0) {
    // debugSerial.print("A: ");
    // debugSerial.println(counterSerial.available());
    if (time > (lastBufferCheck + baudCheckPeriod)) {
      if (lastBufferAvailable == bufferAvailable) {
        input = counterSerial.readStringUntil('\n');
      }

      lastBufferAvailable = counterSerial.available();
      lastBufferCheck = time;
    } 
  }

  if (input != "") {
    oldCps = cps;
    cps = getCsvValue(input, 1).toInt();
    cpm = getCsvValue(input, 3).toInt();
    counterMode = getCsvValue(input, 6);

    screen.reset();

    screen.print(F("CPM: "));
    screen.print(String(cpm));
    screen.print(F("\nCPS: "));
    screen.print(String(cps));

    screen.print("\n\n");

    screen.print(F("Mode:"));
    screen.print(counterMode);

    screen.display(time);

    // debugSerial.print("Recieved line: ");
    // debugSerial.println(input);
    // debugSerial.println("\nPARSE DATA:");
    // debugSerial.print("\tLine: ");
    // debugSerial.println(input);
    // debugSerial.print("\tcps: ");
    // debugSerial.println(cps);
    // debugSerial.print("\tcpm: ");
    // debugSerial.println(cpm);
    // debugSerial.print("\tmode: ");
    // debugSerial.println(counterMode);
  }
}


