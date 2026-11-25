// #include <Wire.h>
// #include <SoftwareSerial.h>
#include "Button.h"
#include "ScreenManager.hpp"

// RX on 18, TX on 19
// SoftwareSerial GeigerSerial = SoftwareSerial(18, 19);

const int buttonPin{ 4 };
Button button(buttonPin);

// Digital pin for reading a single ionization event from the Geiger Counter
const int pulsePin{ 8 };

// The pulses since the last tick of the mode (how many detection events have accrued)
volatile int pulses{0};

// A very basic interrupt function to count pulses
void incrementPulse() {
  pulses++;
}

// An extra signal LED on the Geiger Counter
const int ledPin{ 7 };

// ----- Modes -----
// These modes change the functionality of the Geiger Counter
int mode{ 0 };
const int maxModes{ 2 };  // Number of display modes

// Minimum time in milliseconds before the screen can change
const unsigned long displayDelay{100}; 

// If data has stopped being recieved raise a warning after 5 seconds
bool staleFlag{ false };
unsigned long staleTimer{ 0 };

// Create the display object
ScreenManager screen(displayDelay);

void setup() {
  // Serial.begin(9600);
  Serial1.begin(9600);
  // ADD THIS LINE IN FOR DEBUGGING
  // delay(1000);

  button.begin();

  bool screenInitialized{ screen.begin() };

  while (!screenInitialized) {
    // Don't continue the program if the screen failed to initialize.
    Serial.println("Screen failed to initialize");
  }

  screen.playIntro();

  pinMode(pulsePin, INPUT); // Get digital input on the pulse pin. The Geiger Counter sends HIGH signals when triggered and LOW when not
  attachInterrupt(digitalPinToInterrupt(pulsePin), incrementPulse, RISING); // Watch for the rising edges

  // Reset the staleTimer so it doesn't begin on startup
  staleTimer = millis();
}

void loop() {
  unsigned long time{ millis() };

  if (button.pressed()) {
    mode++;

    if (mode >= maxModes) {
      mode = 0;
    }
  }

  screen.setTextSize(2);
  screen.setCursor(0, 0);

  // Check for stale input
  // if (input == "") {
    if (!staleFlag) {
      staleFlag = true;
      staleTimer = time;
      // Serial.print("staleFlag Timer Set: ");
      // Serial.println(staleTimer);
    } else if (staleFlag) {
      unsigned long duration = time - staleTimer;

      // If data is stale, stop any more execution
      if (duration > 5000) {
        // Serial.print("Stale Data trigger, current time: ");
        // Serial.println(time);
        screen.clear();
        screen.setCursor(0, 0);
        screen.print("STALE DATA\n");
        screen.print("Stale for ");
        screen.print(String(duration / 1000));
        screen.print("s");
        screen.display(time);
        delay(500);
        return;
      }
    }
  // } else {
  //   staleFlag = false;
  //   screen.clear();
  // }

  // if (input != "") {
  //   oldCps = cps;
  //   cps = getCsvValue(input, 1).toInt();
  //   cpm = getCsvValue(input, 3).toInt();
  //   mode = getCsvValue(input, 6);
  // }

  // if (displayWritable(time, lastDisplayTime, displayDelay)) {
  //   display.clearDisplay();
  //   if (displayMode == 0) {
  //     displayGeiger(display, cps, cpm, mode);
  //   } else if (displayMode == 1) {
  //     displayRandom(display, cps, randomCounter, randomCounterStore, oldCps);
  //   }
  // }

  // if (time - randomCounterSetTime > randomCounterHoldTime) {
  //   if (cps > oldCps) {
  //     randomCounterSetTime = time;
  //     randomCounterStore = randomCounter;
  //   }
  //   randomCounterStore = randomCounter;
  // }
  // randomCounter++;
  // if (randomCounter > 9) {
  //   randomCounter = 0;
  // }
}
