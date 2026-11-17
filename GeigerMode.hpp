#ifndef GEIGER_MODE_HPP
#define GEIGER_MODE_HPP

#include <Adafruit_SSD1306.h>

class GeigerMode {
  private:
    bool oldClick;
    bool signalRising(bool currentClick, bool oldClick);

  protected:
    Adafruit_SSD1306& screen;
  
  public:
    GeigerMode(Adafruit_SSD1306& screen);

    // The tick function should be called every loop that a mode is active
    //  click - Whether or not the Geiger Counter's tube has fired
    virtual void tick(bool click) = 0;
};

#endif