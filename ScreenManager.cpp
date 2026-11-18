#include "ScreenManager.hpp"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

bool ScreenManager::begin() {
  bool started{screen.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)};

  return started;
}

bool ScreenManager::displayable(unsigned long currentTime) {
  bool writable{ ! ( (currentTime - lastDisplayTime) < displayDelay ) };
  return writable;
}

void ScreenManager::display(unsigned long currentTime) {
  if (displayable(currentTime)) {
    screen.display();
    lastDisplayTime = currentTime;
  }
}

void ScreenManager::clear() {
  screen.clearDisplay();
}

void ScreenManager::setCursor(int x, int y) {
  screen.setCursor(x, y);
}
void ScreenManager::setTextSize(int fontPointSize) {
  screen.setTextSize(fontPointSize);
}
void ScreenManager::print(String text) {
  screen.print(text);
}

void ScreenManager::playIntro() {
  screen.clearDisplay();
  int targetRadius{ sqrt(sq(SCREEN_WIDTH) + sq(SCREEN_HEIGHT)) / 2 };
  for (int radius{0}; radius < targetRadius; radius += 1) {
    screen.drawCircle(screen.width() / 2, screen.height() / 2, radius, SSD1306_WHITE);
    screen.display();
    delay(2);
  }
  delay(50);
  screen.clearDisplay();

  screen.setTextSize(2);
  screen.setCursor(0, 0);
  const String text = F("Strange\nComputing");
  for (int i{0}; i < text.length(); i++) {
    screen.print(text.charAt(i));
    screen.display();
    delay(25);
  }
  delay(1000);
  screen.clearDisplay();
  screen.display();
}