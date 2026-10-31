# Geiger Counter
Homemade Geiger Counters are awesome!
This project expands on the MightyOhm Geiger Counter kit.
By using an Arduino Pro Micro, a small battery bank with power conversion, a button, and a tiny SSD1306 OLED screen.
With these features, the capabilities of this Geiger Counter can be extended for longer battery life
and for experimental use to get numerical measurements!

The codebase also contains simple code to create a **true** random generator,
which triggers based on the detection of cosmic rays or nuclear decays.

## Setup
To set up the code, simply compile and run in Android Studio. 
Ensure the Geiger Counter's data header pins are soldered so that the SCK, SDA I2C pins, and GND can be connected to the Arduino, as noted in the code.
The screen should also be connected to allow for image rendering.
A button can also be added to allow for switching between survey mode and random number generation!

## Goals
While the codebase already contains useful code, there are many features that would be useful to have
- Specific implementation of click counting - Rather than relying on the preprogrammed calculations, longer averages of clicks or other calculations may be preferred
  - Fast/Slow Mode Implementation - Nuclear experiments and surveying require fast or slow surveying to get accurate measurements. The prewritten firmware does not accomplish this correctly and does not allow for a slow sampling average when measured counts are high enough.
- Data tracking - Adding data storage to the project would allow for quantitative results over longer periods to be stored for more complex experiments
- Wireless Connectivity - The ability to connect another device to either control the Geiger Counter or see data from farther away would be highly beneficial
  - Random Number Generation API - Wireless connection could allow for a random number generation API allowing for true, at-home, random number generation for secure uses
- Desk mode - A way to save power and periodically save results would also be interesting as a tool to monitor background radiation
