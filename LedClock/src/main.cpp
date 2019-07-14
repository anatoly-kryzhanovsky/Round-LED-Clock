#include <Arduino.h>

#include <time.hpp>
#include <rtcTimeSource.hpp>
#include <clockControl.hpp>
#include <clockFace.hpp>
#include <configuration.hpp>

#define DEBUG

RtcTimeSource timeSource;
ClockControl control(&timeSource);
ClockFace clockFace;

unsigned long lastDebugTime = 0;

void setup() {
  Serial.begin(115200);

  control.init();
  timeSource.init();
  clockFace.init();
}

void loop() {
  timeSource.updateTime();
  control.update();
  clockFace.showTime(timeSource.getCurrentTime());

#ifdef DEBUG
  if(millis() - lastDebugTime > 5000)
  {
    Serial.println("TimeSource state");
    Serial.println(timeSource.toString());
    Serial.println("Control state");
    Serial.println(control.toString());
    Serial.println("ClockFace state");
    Serial.println(clockFace.toString());
  }
#endif
 
  delay(Configuration::SystemConfiguration::TickTime);
}

