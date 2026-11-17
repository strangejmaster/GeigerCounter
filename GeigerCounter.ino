  #include <SPI.h>
  #include <Wire.h>
  #include <Adafruit_GFX.h>
  #include <Adafruit_SSD1306.h>
  #include <SoftwareSerial.h>
  #include "Button.h"

  #define SCREEN_WIDTH 128 // OLED display width, in pixels
  #define SCREEN_HEIGHT 64 // OLED display height, in pixels

  // Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
  // The pins for I2C are defined by the Wire-library. 
  // On an arduino UNO:       A4(SDA), A5(SCL)
  // On an arduino MEGA 2560: 20(SDA), 21(SCL)
  // On an arduino LEONARDO:   2(SDA),  3(SCL), ...
  #define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
  #define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
  Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

  // RX on 18, TX on 19
  SoftwareSerial GeigerSerial = SoftwareSerial(18, 19);

  const int buttonPin{4};
  Button button(buttonPin);


  String getCsvValue(String, int);

  int displayMode{0};
  const int maxDisplayModes{2}; // Number of display modes

  unsigned long lastDisplayTime{0}; // Ensures that displaying is OK to prevent the display from being written to too often
  const unsigned long displayDelay{100}; // Minimum time in milliseconds between each display write

  bool displayWritable(unsigned long, unsigned long&, unsigned long);

  // Displays
  void displayGeiger(Adafruit_SSD1306&, long, long, String);
  void displayRandom(Adafruit_SSD1306&, long, long, String, long);
  void displaySample(Adafruit_SSD1306&, long, long, String, long);

  // If data has stopped being recieved raise a warning after 5 seconds
  bool staleFlag{false};
  unsigned long staleTimer{0};

  int randomCounter{0};
  int randomCounterStore{0};
  unsigned long randomCounterHoldTime{2000}; // How to keep values displayed on the random counter
  unsigned long randomCounterSetTime{0}; // What time the random counter was last set
  long oldCps{0};
  long cps{0};
  long cpm{0};
  String mode{""};

  void setup() {
    // Serial.begin(9600);
    Serial1.begin(9600);
    // ADD THIS LINE IN FOR DEBUGGING
    // delay(1000);

    button.begin();

    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
      // Serial.println(F("SSD1306 allocation failed"));
      for(;;); // Don't proceed, loop forever
    }

    display.clearDisplay();
    int targetRadius{ sqrt(sq(SCREEN_WIDTH) + sq(SCREEN_HEIGHT)) / 2 };
    for (int radius{0}; radius < targetRadius; radius += 1) {
      display.drawCircle(display.width() / 2, display.height() / 2, radius, SSD1306_WHITE);
      display.display();
      delay(2);
    }
    delay(50);
    display.clearDisplay();

    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(2);
    display.setCursor(0, 0);
    const String text = F("Strange\nComputing");
    for (int i{0}; i < text.length(); i++) {
      display.print(text.charAt(i));
      display.display();
      delay(25);
    }
    delay(1000);
    display.clearDisplay();
    display.display();

    
    
    // Reset the staleTimer so it doesn't begin on startup
    staleTimer = millis();
  }
    
  void loop() {
    unsigned long time{millis()};

    if (button.pressed()) {
      displayMode++;

      if (displayMode >= maxDisplayModes) {
        displayMode = 0;
      }
    }

    // Get FTDI data
    String input{""};
    if (Serial1.available() > 0) {
      input = Serial1.readStringUntil('\n');
    }
    

    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(2);
    display.setCursor(0, 0);

    // Check for stale input
    if (input == "") {
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
          display.clearDisplay();
          display.setCursor(0, 0);
          display.println("STALE DATA");
          display.print("Stale for ");
          display.print(duration / 1000);
          display.print('s');
          display.display();
          delay(500);
          return;
        }
      }
    } else {
      staleFlag = false;
      display.clearDisplay();
    }

    if (input != "") {
      oldCps = cps;
      cps = getCsvValue(input, 1).toInt();
      cpm = getCsvValue(input, 3).toInt();
      mode = getCsvValue(input, 6);
    }
  
    // Serial.print("Recieved line: ");
    // Serial.println(input);
    // Serial.println("\nPARSE DATA:");
    // Serial.print("\tLine: ");
    // Serial.println(input);
    // Serial.print("\tcps: ");
    // Serial.println(cps);
    // Serial.print("\tcpm: ");
    // Serial.println(cpm);
    // Serial.print("\tmode: ");
    // Serial.println(mode);

    

    if (displayWritable(time, lastDisplayTime, displayDelay)) {
      display.clearDisplay();
      if (displayMode == 0) {
        displayGeiger(display, cps, cpm, mode);
      } else if (displayMode == 1) {
        displayRandom(display, cps, randomCounter, randomCounterStore, oldCps);
      }
    }

    if (time - randomCounterSetTime > randomCounterHoldTime) {
      if (cps > oldCps) {
        randomCounterSetTime = time;
        randomCounterStore = randomCounter;
      }
      randomCounterStore = randomCounter;
    }
    randomCounter++;
    if (randomCounter > 9) {
      randomCounter = 0;
    }
  }

  void displayGeiger(Adafruit_SSD1306& screen, long cps, long cpm, String mode) {
    screen.print(F("CPM: "));
    screen.println(cpm);
    screen.#include <SPI.h>
  #include <Wire.h>
  #include <Adafruit_GFX.h>
  #include <Adafruit_SSD1306.h>
  #include <SoftwareSerial.h>
  #include "Button.h"

  #define SCREEN_WIDTH 128 // OLED display width, in pixels
  #define SCREEN_HEIGHT 64 // OLED display height, in pixels

  // Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
  // The pins for I2C are defined by the Wire-library. 
  // On an arduino UNO:       A4(SDA), A5(SCL)
  // On an arduino MEGA 2560: 20(SDA), 21(SCL)
  // On an arduino LEONARDO:   2(SDA),  3(SCL), ...
  #define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
  #define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
  Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

  // RX on 18, TX on 19
  SoftwareSerial GeigerSerial = SoftwareSerial(18, 19);

  const int buttonPin{4};
  Button button(buttonPin);


  String getCsvValue(String, int);

  int displayMode{0};
  const int maxDisplayModes{2}; // Number of display modes

  unsigned long lastDisplayTime{0}; // Ensures that displaying is OK to prevent the display from being written to too often
  const unsigned long displayDelay{100}; // Minimum time in milliseconds between each display write

  bool displayWritable(unsigned long, unsigned long&, unsigned long);

  // Displays
  void displayGeiger(Adafruit_SSD1306&, long, long, String);
  void displayRandom(Adafruit_SSD1306&, long, long, String, long);

  // If data has stopped being recieved raise a warning after 5 seconds
  bool staleFlag{false};
  unsigned long staleTimer{0};

  int randomCounter{0};
  int randomCounterStore{0};
  unsigned long randomCounterHoldTime{2000}; // How to keep values displayed on the random counter
  unsigned long randomCounterSetTime{0}; // What time the random counter was last set
  long oldCps{0};
  long cps{0};
  long cpm{0};
  String mode{""};

  void setup() {
    // Serial.begin(9600);
    Serial1.begin(9600);
    // ADD THIS LINE IN FOR DEBUGGING
    // delay(1000);

    button.begin();

    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
      // Serial.println(F("SSD1306 allocation failed"));
      for(;;); // Don't proceed, loop forever
    }

    display.clearDisplay();
    int targetRadius{ sqrt(sq(SCREEN_WIDTH) + sq(SCREEN_HEIGHT)) / 2 };
    for (int radius{0}; radius < targetRadius; radius += 1) {
      display.drawCircle(display.width() / 2, display.height() / 2, radius, SSD1306_WHITE);
      display.display();
      delay(2);
    }
    delay(50);
    display.clearDisplay();

    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(2);
    display.setCursor(0, 0);
    const String text = F("Strange\nComputing");
    for (int i{0}; i < text.length(); i++) {
      display.print(text.charAt(i));
      display.display();
      delay(25);
    }
    delay(1000);
    display.clearDisplay();
    display.display();

    
    
    // Reset the staleTimer so it doesn't begin on startup
    staleTimer = millis();
  }
    
  void loop() {
    unsigned long time{millis()};

    if (button.pressed()) {
      displayMode++;

      if (displayMode >= maxDisplayModes) {
        displayMode = 0;
      }
    }

    // Get FTDI data
    String input{""};
    if (Serial1.available() > 0) {
      input = Serial1.readStringUntil('\n');
    }
    

    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(2);
    display.setCursor(0, 0);

    // Check for stale input
    if (input == "") {
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
          display.clearDisplay();
          display.setCursor(0, 0);
          display.println("STALE DATA");
          display.print("Stale for ");
          display.print(duration / 1000);
          display.print('s');
          display.display();
          delay(500);
          return;
        }
      }
    } else {
      staleFlag = false;
      display.clearDisplay();
    }

    if (input != "") {
      oldCps = cps;
      cps = getCsvValue(input, 1).toInt();
      cpm = getCsvValue(input, 3).toInt();
      mode = getCsvValue(input, 6);
    }
  
    // Serial.print("Recieved line: ");
    // Serial.println(input);
    // Serial.println("\nPARSE DATA:");
    // Serial.print("\tLine: ");
    // Serial.println(input);
    // Serial.print("\tcps: ");
    // Serial.println(cps);
    // Serial.print("\tcpm: ");
    // Serial.println(cpm);
    // Serial.print("\tmode: ");
    // Serial.println(mode);

    

    if (displayWritable(time, lastDisplayTime, displayDelay)) {
      display.clearDisplay();
      if (displayMode == 0) {
        displayGeiger(display, cps, cpm, mode);
      } else if (displayMode == 1) {
        displayRandom(display, cps, randomCounter, randomCounterStore, oldCps);
      }
    }

    if (time - randomCounterSetTime > randomCounterHoldTime) {
      if (cps > oldCps) {
        randomCounterSetTime = time;
        randomCounterStore = randomCounter;
      }
      randomCounterStore = randomCounter;
    }
    randomCounter++;
    if (randomCounter > 9) {
      randomCounter = 0;
    }
  }

  void displayGeiger(Adafruit_SSD1306& screen, long cps, long cpm, String mode) {
    screen.print(F("CPM: "));
    screen.println(cpm);
    screen.print(F("CPS: "));
    screen.println(cps);

    screen.println();
    screen.print(F("Mode:"));
    screen.println(mode);
    screen.display();
  }

  void displayRandom(Adafruit_SSD1306& screen, long cps, int rand, int randStore, long oldCps) {
    screen.print(F("Pick: "));
    screen.println(randStore);
    // screen.println();
    screen.print(F("CPS: "));
    screen.println(cps);

    screen.println();
    screen.print(F("Pool: "));
    screen.println(rand);
    screen.display();
  }

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

  bool displayWritable(unsigned long currentTime, unsigned long& lastDisplayTime, unsigned long displayDelay) {
    bool displayable = true;
    if (currentTime - lastDisplayTime < displayDelay) {
      displayable = false;
    }

    if (displayable) {
      lastDisplayTime = currentTime;
    }
    return displayable;
  }print(F("CPS: "));
    screen.println(cps);

    screen.println();
    screen.print(F("Mode:"));
    screen.println(mode);
    screen.display();
  }

  void displayRandom(Adafruit_SSD1306& screen, long cps, int rand, int randStore, long oldCps) {
    screen.print(F("Pick: "));
    screen.println(randStore);
    // screen.println();
    screen.print(F("CPS: "));
    screen.println(cps);

    screen.println();
    screen.print(F("Pool: "));
    screen.println(rand);
    screen.display();
  }

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

  bool displayWritable(unsigned long currentTime, unsigned long& lastDisplayTime, unsigned long displayDelay) {
    bool displayable = true;
    if (currentTime - lastDisplayTime < displayDelay) {
      displayable = false;
    }

    if (displayable) {
      lastDisplayTime = currentTime;
    }
    return displayable;
  }