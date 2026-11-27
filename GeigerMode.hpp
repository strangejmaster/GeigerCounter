#ifndef GEIGER_MODE_HPP
#define GEIGER_MODE_HPP

#include "ScreenManager.hpp"

class GeigerMode {
  protected:
    ScreenManager& screen;
    Stream& debugSerial;

  public:
    GeigerMode(ScreenManager& screen, Stream& debugSerial) : screen(screen), debugSerial(debugSerial) {}

    // The tick function should be called every loop that a mode is active
    //  clicks - The number of impulses detected since the last loop
    //  time - The time in milliseconds
    virtual void tick(int clicks, unsigned long time) = 0;
};

class TestMode : public GeigerMode {
  private:
    HardwareSerial& counterSerial;

    // TODO: Inject the baudrate into this object since its specified in GeigerCounter.ino
    // Minimum millibaudrate shared over UART with the Geiger Counter (i.e. how the minimum number of symbols that can be sent every MILLIsecond)
    // For 9600 baud, it's at least 9 symbols/millisecond = 0.009 symbols/microsecond = 9 symbols / 1000 microseconds
    // const int millibaudrate{9}; 
    // Since we work with milliseconds, the smallest check period we can get is one millisecond where 9 symbols should be able to be sent
    // It can be guaranteed that UART reports will only be sent every second, so another report will not be able to start (and thus fill the buffer) if at the end of one, 
    // an at minimum 1 millisecond checking period occurs
    const int baudCheckPeriod{1};
    // The amount of available buffer since the last check (one baudCheckPeriod (or more) before), 
    // ensures that the counter and Arduino don't get into a dangerous asychronous position where
    // bytes sent from the Geiger counter are always read too early by the Arduino 
    // Basically, check if the counter is done sending its full message by seeing if it doesn't send any additional 
    // bytes following some potential sends (according to the baudrate)
    int lastBufferAvailable{0}; 

    // The time that the lastBufferAvailable was set, once time > lastBufferCheck + baudCheckPeriod, we can compare the buffer to the lastBufferAvailable
    unsigned long lastBufferCheck{0}; 
  
    int oldCps{0};
    int cps{0};
    int cpm{0};
    String counterMode{"Loading"};

    String getCsvValue(String input, int index) {
      int valueIndex{0};
      String result{""};

      for (int i{0}; i < input.length(); i++) {
        char currentChar{input.charAt(i)};

        // Basic state machine, if character is ',' go to the next value index, if the character isn't ','
        // and the valueIndex is the desired index simply add that character to the result
        if (currentChar == ',') {
          valueIndex++;
          continue;
        }

        if (valueIndex == index) {
          result += currentChar;
        }
      }

      return result;
    }

  public:
    TestMode(ScreenManager& screen, Stream& debugSerial, HardwareSerial& counterSerial) : GeigerMode(screen, debugSerial), counterSerial(counterSerial) {};
    void tick(int clicks, unsigned long time) override;
};

// // Detect if a click should exist (impulses are sent by the Attiny2313 on the board and last 100 uS)
// A 3 MICROsecond debounce to wait before the next pulse is detected
    // const int debounce{3};

    // // The analog signal threshold (from 0 to 100%) for a signal to be considered HIGH
    // const double clickThreshold{40};

    // // The last time the impulse signal rose
    // unsigned long lastClickRise{0};

    // // Time in microseconds that an impulse signal (tube detection event) from the Attiny microcontroller lasts
    // // This is effectively a one hundred microsecond hold to wait for the signal to clear
    // const unsigned long impulseDuration{100};

    // // Time in microseconds to debounce after the impulseDuration has passed (allows the signal to restabalize to LOW)
    // const unsigned long impulseDebounce{5};
//     bool clickDetected(unsigned long time, int click) {
//       bool active{click > clickThreshold};

//       if (active && ((time - lastClickRise) > (impulseDuration + impulseDebounce))) {
//         lastClickRise = time;
//         return true;
//       }
//       return false;
//     }


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







#endif