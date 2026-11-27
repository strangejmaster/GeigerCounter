// #include <Wire.h>
// #include <SoftwareSerial.h>
#include "Button.h"
#include "ScreenManager.hpp"
#include "GeigerMode.hpp"

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
int activeMode{ 0 };
const int maxModes{ 2 };  // Number of display modes
bool modeSwitched{false}; // A flag to only switch the mode once for each held button press

GeigerMode* modes[maxModes] = {}; // Initialize the array of modes with nullptrs

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

  pinMode(pulsePin, INPUT); // Get digital input on the pulse pin. The Geiger Counter sends HIGH signals when triggered and LOW when not
  attachInterrupt(digitalPinToInterrupt(pulsePin), incrementPulse, RISING); // Watch for the rising edges

  // Initialize all of the modes
  modes[0] = new TestMode(screen, Serial, Serial1);
  modes[1] = new RandomMode(screen, Serial);

  screen.playIntro();

  modes[activeMode]->setup(0);

  // Reset the staleTimer so it doesn't begin on startup
  staleTimer = millis();  
}

void loop() {
  unsigned long time{ millis() };

  if (button.pressed_for(2000)) {
    if (!modeSwitched) {
      activeMode++;

      if (activeMode >= maxModes) {
        activeMode = 0;
      }

      modes[activeMode]->setup(time);

      modeSwitched = true;
    }
  } else {
    // Clear the modeSwitched flag once the button has been released/has not been pressed for >= 2 seconds
    modeSwitched = false;
  }

  // Serial.print("Active mode: ");
  // Serial.println(activeMode);
  
  if (modes[activeMode] != nullptr) {
    modes[activeMode]->tick(pulses, time);

    pulses = 0;
  } else {
    // Serial.println("Nullptr!!!");
  }

  // Check for stale input
  // if (input == "") {
  // if (!staleFlag) {
  //   staleFlag = true;
  //   staleTimer = time;
  //   // Serial.print("staleFlag Timer Set: ");
  //   // Serial.println(staleTimer);
  // } else if (staleFlag) {
  //   unsigned long duration = time - staleTimer;

  //   // If data is stale, stop any more execution
  //   if (duration > 5000) {
  //     // Serial.print("Stale Data trigger, current time: ");
  //     // Serial.println(time);
  //     screen.reset();
  //     screen.print("STALE DATA\n");
  //     screen.print("Stale for ");
  //     screen.print(String(duration / 1000));
  //     screen.print("s");
  //     screen.display(time);
  //     delay(500);
  //     return;
  //   }
  // }
  // } else {
  //   staleFlag = false;
  //   screen.clear();
  // }
}
