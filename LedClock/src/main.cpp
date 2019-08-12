#include <Arduino.h>

#include <clockFactory.hpp>

Clock* myClock;

void setup() {
  Serial.begin(115200);

  myClock = ClockFactory::createRtcClockWithControl();
  myClock->init();
}

void loop() {
  myClock->update();
  delay(Configuration::SystemConfiguration::TickTime);
}

