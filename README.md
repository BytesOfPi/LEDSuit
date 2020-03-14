# LEDSuit
Android BLE app to control LED Suit

### Board
  - [Espressif ESP32](https://dl.espressif.com/dl/package_esp32_index.json) - Added as additional board manager in Arduino IDE

### Libraries used
  - [FastLED](http://fastled.io/docs/3.1/index.html) - Primary workhorse API for calculating and drawing LEDs.
  - [Keypad](https://playground.arduino.cc/Code/Keypad/) - For use with Adafruit Membrane 3.4 keypad entry
  
### Libraries modified
  - [FastLED_GFX](https://github.com/Jorgen-VikingGod/FastLED-GFX) - Modified GFcanvas to work with my matrix layout
  - [Aurora](https://github.com/pixelmatix/aurora) - Straight up lifted the classes and algorithms and embedded some of these patterns directly into this code base.  Used for Matrix patterns.

### Tools
  - Arduino IDE - Used basic Arduino IDE to setup the ESP32 board manager drivers and import libraries
  - VSCode (with Arduino Plugin) - REALLY useful IDE and plugin to manage code.  Git plugin allows for easy source repository changes and able to manage multiple files in folders.

### Other related links
  - [Using BLE on Arduino](https://randomnerdtutorials.com/esp32-bluetooth-low-energy-ble-arduino-ide/)
test
