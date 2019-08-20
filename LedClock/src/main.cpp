#include <Arduino.h>

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

#include <clockFactory.hpp>

Clock* myClock;

void setup() {
  Serial.begin(115200);

#if defined(ESP8266)
  WiFi.mode(WIFI_AP_STA);
#endif

  myClock = ClockFactory::createClockWithRemoeControl();
  myClock->init();
}

void loop() {
  myClock->update();
  delay(myClock->getConfiguration()->getSystemConfiguration()->TickTime);
}

