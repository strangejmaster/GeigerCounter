#include "ScreenManager.hpp"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

bool ScreenManager::begin() {
  bool started{display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)};

  return started;
}

bool ScreenManager::displayable(unsigned long currentTime) {
  bool writable{ ! ( (currentTime - lastDisplayTime) < displayDelay ) };
  return writable;
}

void ScreenManager::display(unsigned long currentTime) {
  if (writable(currentTime)) {
    display.display();
    lastDisplayTime = currentTime;
  }
}

void ScreenManager::clear() {
  display.clearDisplay();
}

void ScreenManager::setCursor(int x, int y) {
  display.setCursor(x, y);
}
void ScreenManager::setTextSize(int fontPointSize) {
  display.setTextSize(fontPointSize);
}
void ScreenManager::print(String text) {
  display.print(text);
}

void ScreenManager::playIntro() {
  display.clearDisplay();
  int targetRadius{ sqrt(sq(SCREEN_WIDTH) + sq(SCREEN_HEIGHT)) / 2 };
  for (int radius{0}; radius < targetRadius; radius += 1) {
    display.drawCircle(display.width() / 2, display.height() / 2, radius, SSD1306_WHITE);
    display.display();
    delay(2);
  }
  delay(50);
  display.clearDisplay();

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

  // Call the ScreenManager's display method to ensure future write's follow delays
  display();
}