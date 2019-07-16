# Arduino Round-LED Clock 
 Project based on awesome project <a href="https://github.com/leonvandenbeukel/Round-LED-Clock">Wi-Fi Connected Round-LED-Clock</a>
 
 # Changes from original
 1) move to <a href="https://platformio.org">PlatformIO<a> editor and split projects to separate classes
 2) add ability (in progress) for switch between different time source (RTC or NTP based)
 3) add new model and PCB (in progress) for RTC-based case
 4) add control block for adjust time
  
 ### Schema (in progress)
 
 ![alt text](https://github.com/anatoly-kryzhanovsky/Round-LED-Clock/blob/master/schema.png) 
 
 Connect 5 volts from power source to Vin pin of arduino board (Vccm 5V, Vin,...)
 Connect 5 volts from power source to +5V of led strip
 Connect 5 volts from power source to Vcc pin of DS1307 RTC board
 Connect 5 volts from power source to three switches
 Connect Ground from power source to Gnd pin of arduino board
 Connect Ground from power source to Gnd pin of led strip
 Connect Ground from power source to Gnd pin of DS1307 RTC board
 Connect Ground from power source to another pin of switches throught 10kOm resisors
 Connect Ground from power source to cathode (shortes) pin of two leds
 Connect three switches to digital pins of arduino board (can be any not used digital pin - pins number configurable through code)'
 Connect led anode through 220Om resistor to another digital pin of arduino board
 Connect SDA and SCL pins of DS1307 board to corresponding pins of arduino board (I2C bus)
 Connect led strip data pin to not used digital pin of arduino boardp.  
 
 ### Parts list (just Google on the name to find a link where to buy)

| Components                              			            |
| -------------                          			             |
| WeMOS D1 Mini ESP module (if you want to use NTP sync) |
| LED strip WS2812B 60 RGB LED's 1 meter                 |
| Black PLA filament                           		        |
| White PLA filament                           		        |
| 4x M3 screws                                           |
| Power adapter (5V DC, 3.6A)                            |
| 3 switchs                                              |
| 2 leds (5V)                                            |
| 3 resistors of 10kOm                                   |
| 2 resistors of 220Om                                   |
| DS1307 RTC if you want to use RTC sync)                |
| Jumper wires                                           |


### YouTube video (in progress)

<a href="https://youtu.be/Z4b4v84smpg" target="_blank"><img src="https://img.youtube.com/vi/Z4b4v84smpg/0.jpg" 
alt="Click to view: LED Clock" width="500" border="1" /></a>



