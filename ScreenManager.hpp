#ifndef SCREEN_MANAGER_HPP
#define SCREEN_MANAGER_HPP

// TODO: Is SPI needed?
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
class ScreenManager {
  private:
  // Screen Dimensions in Pixels
    const int SCREEN_WIDTH{128};
    const int SCREEN_HEIGHT{64};
  
  // Pin to reset the screen (-1 => it just uses the Arduino RST pin)
    const int OLED_RESET{-1};

  // I2C Address
    const int SCREEN_ADDRESS{0x3c}; //< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

    // The amount of time in milliseconds before the display can be updated (on a large scale, not an actual hardware limit)
    unsigned long displayDelay;
    
    // Last time the screen was written to
    unsigned long lastDisplayTime{0};

    Adafruit_SSD1306 screen;

  public:
    ScreenManager(unsigned long displayDelay) : screen(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET), displayDelay(displayDelay) {
      screen.setTextColor(SSD1306_WHITE);
      setTextSize(2);
    }
    bool begin();

    void setCursor(int x, int y);
    void setTextSize(int fontPointSize);
    void print(String text);
    void clear();
    void reset(); // Clears and resets cursor position and font size

    void playIntro();

    // Returns whether the display can be written to (useful to prevent over calling it)
    bool displayable(unsigned long currentTime);

    // Specially controls the screen so that it's not refreshed too often when display() calls are made frequently (more often than display delay)
    void display(unsigned long currentTime);
};

#endif