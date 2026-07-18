#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>

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

String getCsvValue(String, int);

// If data has stopped being recieved raise a warning after 5 seconds
bool staleFlag{false};
unsigned long staleTimer{0};

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  delay(1000);

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    // Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.clearDisplay();
  for (int radius{0}; radius < display.width() / 2; radius += 2) {
    display.drawCircle(display.width() / 2, display.height() / 2, radius, SSD1306_WHITE);
    display.display();
    delay(25);
  }
  delay(100);
  display.clearDisplay();

  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(2);
  display.setCursor(0, 0);
  const String text = F("Strange\nComputing");
  for (int i{0}; i < text.length(); i++) {
    display.print(text.charAt(i));
    display.display();
    delay(50);
  }
  delay(1000);
  display.clearDisplay();
  display.display();

  // Reset the staleTimer to wait at least 5 seconds after the boot-up
  staleTimer = millis();
}

void loop() {
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(2);
  display.setCursor(0, 0);

  String input{""};
  
  while(Serial1.available() > 0) {
    char c{(char) Serial1.read()};
    input += c;

    // Wait for next byte to arrive
    // DANGEROUS!!! This can do some weird things if another line arrives before this one is done being read. Be careful, maybe just look for \n character
    delay(5);
  }
  // Serial.print("Recieved line: ");
  // Serial.println(input);
  unsigned long time{millis()};
  
  if (input == "") {
    if (!staleFlag) {
      staleFlag = true;
      staleTimer = time;
      // Serial.print("staleFlag Timer Set: ");
      // Serial.println(staleTimer);
    } else if (staleFlag) {
      unsigned long duration = time - staleTimer;
      if (duration > 5000) {
        // Serial.print("Stale Data trigger, current time: ");
        // Serial.println(time);
        display.clearDisplay();
        display.println("STALE DATA");
        display.print("Stale for ");
        display.print(duration / 1000);
        display.print('s');
        display.display();
        delay(500);
      }
    }
    return;
  } else {
    staleFlag = false;
    display.clearDisplay();
  }

  long cps = getCsvValue(input, 1).toInt();
  long cpm = getCsvValue(input, 3).toInt();
  String mode = getCsvValue(input, 6);

  Serial.println("\nPARSE DATA:");
  Serial.print("\tLine: ");
  Serial.println(input);
  Serial.print("\tcps: ");
  Serial.println(cps);
  Serial.print("\tcpm: ");
  Serial.println(cpm);
  Serial.print("\tmode: ");
  Serial.println(mode);

  display.print(F("CPM: "));
  display.println(cpm);
  display.print(F("CPS: "));
  display.println(cps);
  display.print(F("Mode: "));
  display.setTextSize(1);
  display.println(mode);
  display.display();
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