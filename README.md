# Arduino Round-LED Clock 
 Project based on awesome project <a href="https://github.com/leonvandenbeukel/Round-LED-Clock">Wi-Fi Connected Round-LED-Clock</a>
 
 # Changes from original
 1) move to <a href="https://platformio.org">PlatformIO</a> editor and split projects to separate classes
 2) support different timesources (RTC, NTP based or custom provided by your code)
 3) add new model (in progress) and PCB for RTC-based case
 4) add control block for adjust time
  
 ## Schema
 
 ![alt text](https://github.com/anatoly-kryzhanovsky/Round-LED-Clock/blob/master/docs/readme assets/schema.png) 
 
 Connection:
 * connect 5 volts from power source to Vin pin of arduino board (Vcc, 5V, Vin,...)
 * connect 5 volts from power source to +5V of led strip
 * connect 5 volts from power source to Vcc pin of DS1307 RTC board
 * connect 5 volts from power source to three switches
 * connect Ground from power source to Gnd pin of arduino board
 * connect Ground from power source to Gnd pin of led strip
 * connect Ground from power source to Gnd pin of DS1307 RTC board
 * connect Ground from power source to another pin of switches throught 10k ohm resisors
 * connect Ground from power source to cathode (shortes) pin of two leds
 * connect three switches to digital pins of arduino board (can be any not used digital pin - pins number are configurable through code)
 * connect led anode through 220 ohm resistor to another digital pin of arduino board
 * connect SDA and SCL pins of DS1307 board to corresponding pins of your arduino board (I2C bus)
 * connect led strip data pin to not used digital pin of arduino board.  

## Notes

* There are two level of debounce - hardware (using pull-up resistor for switches) and software (using bounce2 library). You can ommit hardware part for simplification.
* You can use RTC timesource or NTP. For NTP you must have Wi-Fi with Internet access. Current NTP time source code witks only with Wemos board based on ESP8266.
* There are 60 leds in strip, each led consume 0.3W, so if you want to turn all of them you need at least 18W power adapter (with 5V). In my configuration only 3 leds turn on at same time (hour, minute and second), so i need only 0.9W or ~0.2A, so i use power adapter from my old phone.
* Be attention with power source - all units in scheme use 5V and can burn out from higher voltage
  
## Parts list (just Google on the name to find a link where to buy)

| Image | Count | Components|
| ------|-------|------|
| ![alt text](https://github.com/anatoly-kryzhanovsky/Round-LED-Clock/blob/master/docs/readme assets/wemos.png) | 1 |WeMOS D1 Mini ESP module (if you want to use NTP sync) |
| ![alt text](https://github.com/anatoly-kryzhanovsky/Round-LED-Clock/blob/master/docs/readme assets/ledstrip.png) | 1 |LED strip WS2812B 60 RGB LED's 1 meter           |
| ![alt text](https://github.com/anatoly-kryzhanovsky/Round-LED-Clock/blob/master/docs/readme assets/filament_black.png) | X | Black PLA filament                        |
| ![alt text](https://github.com/anatoly-kryzhanovsky/Round-LED-Clock/blob/master/docs/readme assets/filament_white.png) | X | White PLA filament                        |
| ![alt text](https://github.com/anatoly-kryzhanovsky/Round-LED-Clock/blob/master/docs/readme assets/screw.png) | 4 | M3 screws                                          |
| ![alt text](https://github.com/anatoly-kryzhanovsky/Round-LED-Clock/blob/master/docs/readme assets/power_adapter.png)  | 1| Power adapter (5V DC, 0.5A)                |
| ![alt text](https://github.com/anatoly-kryzhanovsky/Round-LED-Clock/blob/master/docs/readme assets/switch.png) | 3 | switch                                            |
| ![alt text](https://github.com/anatoly-kryzhanovsky/Round-LED-Clock/blob/master/docs/readme assets/led.png) | 2 | leds (5V)                                            |
| ![alt text](https://github.com/anatoly-kryzhanovsky/Round-LED-Clock/blob/master/docs/readme assets/resistor_10kohm.png) | 3 | resistors of 10kOm                       |
| ![alt text](https://github.com/anatoly-kryzhanovsky/Round-LED-Clock/blob/master/docs/readme assets/resistor_220ohm.png) | 2 | resistors of 220Om                       |
| ![alt text](https://github.com/anatoly-kryzhanovsky/Round-LED-Clock/blob/master/docs/readme assets/ds1307.png) | 1 | DS1307 RTC if you want to use RTC sync)           |
| ![alt text](https://github.com/anatoly-kryzhanovsky/Round-LED-Clock/blob/master/docs/readme assets/terminal.png) | 2 | 2 power terminal)                               |
| ![alt text](https://github.com/anatoly-kryzhanovsky/Round-LED-Clock/blob/master/docs/readme assets/wires.png) | X | Jumper wires                                       |


## YouTube video (in progress)

<a href="https://youtu.be/Z4b4v84smpg" target="_blank"><img src="https://img.youtube.com/vi/Z4b4v84smpg/0.jpg" 
alt="Click to view: LED Clock" width="500" border="1" /></a>

## Configuration

All configuration stored in [https://github.com/anatoly-kryzhanovsky/Round-LED-Clock/blob/master/LedClock/src/configuration.hpp](https://github.com/anatoly-kryzhanovsky/Round-LED-Clock/blob/master/LedClock/src/configuration.hpp) file.

There are configurations for different part of the system:

### Control configuration (ControlConfiguration class)

* ModeButtonPin - number of digital pin where mode button was connected. Mode button allow change mode of control block. By default control block disabled. If you press mode button once then you enter to hour ajustment mode and can use increase or decrease buttons for add or substract one hour to current time. If you press mode button again then you enter to minute adjustment mode and can use increase or decrease buttons for add or substract one minute to current time. If you press mode button again you exit from edit mode and return to initial state. There is timeout for autoexit from edit mode - if you not press any button in *EditModeTimeout* period then system exit from ediit mode and return to initial state.
* IncreaseButtonPin - number of digital pin where increate button was connected. In edit mode (hour or minute) you can use this button for add one minute to current time.
* DecreaseButtonPin - number of digital pin where decrease button was connected. In edit mode (hour or minute) you can use this button for subtract one minute to current time.
* HourModeLedPin - number of digital pin where hour mode led was connected. In edit mode while you adjust hour this led is on.
* MinuteModeLedPin - number of digital pin where minute mode led was connected. In edit mode while you adjust minute this led is on.
* EditModeTimeout - timeout (in millis) for autoexit from edit mode.
* ButtonDebounceInterval - interval (in millis) for buttons debounce (used by Bounce2 library)

### Clockfact configuration (ClockFaceConfiguration class)

* HourSmoothMove - if true then at 20 minutes hour led should be at second led in hour group, if false then hour led move only after 59 minutes 59 seconds reached
* UseNightMode - use different brightness for night time (defined by *NightStartAt* and *NightEndAt*)
* NightEndAt - hours then nightmode should disabled (in 24 hour notation)
* NightStartAt - hours then nightmode should enabled (in 24 hour notation)
* NightmodeBrightness - brightness for nightmode (from 0 to 255, where 0 turn leds off and 255 - use maximum brightnes)
* LedsCount - count of leds in strip
* LedDataPin - number of digital pin where led strip data pin was connected
* ColorHour - color for hour led
* ColorMinute - color for minute led
* ColorSecond - color for second led
* ColorHourMinute - color for led if hour and minute has same position
* ColorMinuteSecond - color for led if minute and second has same position
* ColorAll - color for led if hour, minute and second has same position

### RTC timesource configuration (RtcTimeSourceConfiguration class)

* SyntInterval - interval (in millis) for synchornization internal clock (calculated by system millis() function) and external RTC

### NTP timesource configuration (NtpTimeSourceConfiguration class)

* NTPServerName - NTP server name (server should be available through WiFi)
* SSID - ssid of WiFi network
* Password - key for connect to WiFi network
* TimeZone - local timezone 
* SyncInterval - interval (in millis) for synchronization with NTP server 

### System-wide configuration (SystemConfiguration class)

* TickTime - delay (in millis) between loop iteration
