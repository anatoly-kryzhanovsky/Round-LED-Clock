#include <Arduino.h>
#include <ESP8266WiFi.h>

#include <clockFactory.hpp>

Clock* myClock;

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_AP_STA);

  myClock = ClockFactory::createRtcClockWithControl();
  myClock->init();
}

void loop() {
  myClock->update();
  delay(myClock->getConfiguration()->getSystemConfiguration()->TickTime);
}

