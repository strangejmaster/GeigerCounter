#ifndef GEIGER_MODE_HPP
#define GEIGER_MODE_HPP

#include "ScreenManager.hpp"

class GeigerMode {
  private:
    bool oldClick;
    bool signalRising(bool currentClick, bool oldClick);

  protected:
    ScreenManager& screen;
  
  public:
    GeigerMode(ScreenManager& screen) : screen(screen) {}

    // The tick function should be called every loop that a mode is active
    //  click - Whether or not the Geiger Counter's tube has fired
    virtual void tick(bool click) = 0;
};

// void displayGeiger(Adafruit_SSD1306& screen, long cps, long cpm, String mode) {
//   screen.print(F("CPM: "));
//   screen.println(cpm);
//   screen.print(F("CPS: "));
//   screen.println(cps);

//   screen.println();
//   screen.print(F("Mode:"));
//   screen.println(mode);
//   screen.display();
// }

// void displayRandom(Adafruit_SSD1306& screen, long cps, int rand, int randStore, long oldCps) {
//   screen.print(F("Pick: "));
//   screen.println(randStore);
//   // screen.println();
//   screen.print(F("CPS: "));
//   screen.println(cps);

//   screen.println();
//   screen.print(F("Pool: "));
//   screen.println(rand);
//   screen.display();
// }



// int randomCounter{0};
// int randomCounterStore{0};
// unsigned long randomCounterHoldTime{2000}; // How to keep values displayed on the random counter
// unsigned long randomCounterSetTime{0}; // What time the random counter was last set
// long oldCps{0};
// long cps{0};
// long cpm{0};
// // Displays
// void displayGeiger(Adafruit_SSD1306&, long, long, String);
// void displayRandom(Adafruit_SSD1306&, long, int, int, long);
// // void displaySample(Adafruit_SSD1306&, long, long, Strin/g, long);






// // Could go in GeigerTestMode

// // Serial.print("Recieved line: ");
//   // Serial.println(input);
//   // Serial.println("\nPARSE DATA:");
//   // Serial.print("\tLine: ");
//   // Serial.println(input);
//   // Serial.print("\tcps: ");
//   // Serial.println(cps);
//   // Serial.print("\tcpm: ");
//   // Serial.println(cpm);
//   // Serial.print("\tmode: ");
//   // Serial.println(mode);

// // Serial1 is of class HardwareSerial
//  // Get FTDI data
//   String input{""};
//   if (Serial1.available() > 0) {
//     input = Serial1.readStringUntil('\n');
//   }
// String getCsvValue(String, int);
// String mode{""};
// String getCsvValue(String input, int index) {
//   int valueIndex{0};
//   String result{""};

//   for (int i{0}; i < input.length(); i++) {
//     char currentChar{input.charAt(i)};

//     // Basic state machine, if character is ',' go to the next value index, if the character isn't ','
//     // and the valueIndex is the desired index simply add that character to the result
//     if (currentChar == ',') {
//       valueIndex++;
//       continue;
//     }

//     if (valueIndex == index) {
//       result += currentChar;
//     }
//   }

//   return result;
// }

#endif