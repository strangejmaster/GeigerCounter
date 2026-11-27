#include "GeigerMode.hpp"

void TestMode::setup(unsigned long time) {
  screen.reset();
  screen.print("Awaiting\nData...");
  screen.display(time);
}

void TestMode::tick(int clicks, unsigned long time, bool primaryAction, bool secondaryAction) {
  // Get FTDI data
  String input{""};

  int bufferAvailable{counterSerial.available()};
  if (bufferAvailable > 0) {
    // debugSerial.print("A: ");
    // debugSerial.println(counterSerial.available());
    if (time - lastBufferCheck > baudCheckPeriod) {
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

void RandomMode::setup(unsigned long time) {
  screen.reset();
  screen.print("Loading\nRandom");
  screen.display(time);
}

void RandomMode::tick(int clicks, unsigned long time, bool primaryAction, bool secondaryAction) {
  if (primaryAction) {
    state++;

    if (state >= maxStates) {
      state = 0;
    }

    if (state == 0) {
      upperBound = 9;
      formatPick = [](int pick) -> String { return String(pick); };
    } else if (state == 1) {
      upperBound = 20;
      formatPick = [](int pick) -> String { return String(pick); };
    } else if (state == 2) {
      upperBound = 26;
      formatPick = [](int pick) -> String { return String((char) (65 + pick)); };
    }
  }

  if (time - lastPickTime > minimumPickDelay) {
    if (clicks > 0) {
      pick = formatPick(pool);

      lastPickTime = time;
    }
  }

  if (screen.displayable(time)) {
    screen.reset();
    screen.print("Pick: ");
    screen.print(pick);
    screen.print("\nPool: ");
    screen.print(formatPick(pool));
    screen.print("\Max: ");
    screen.print(formatPick(upperBound));

    screen.display(time);
  }
  
  pool++;
  if (pool > upperBound) {
    pool = 0;
  }
}