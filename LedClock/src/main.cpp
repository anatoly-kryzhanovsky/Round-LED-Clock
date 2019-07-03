#include <Arduino.h>

#include <time.hpp>
#include <rtcTimeSource.hpp>
#include <clockControl.hpp>
#include <clockFace.hpp>
#include <configuration.hpp>

RtcTimeSource timeSource;
ClockControl control(&timeSource);
ClockFace clockFace;

void setup() {
  control.init();
  timeSource.init();
  clockFace.init();
}

void loop() {
  timeSource.updateTime(Configuration::TickTime);
  control.update();
  clockFace.showTime(timeSource.getCurrentTime());
 
  delay(Configuration::TickTime);
}

