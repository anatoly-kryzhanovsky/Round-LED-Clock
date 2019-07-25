#pragma once

#include <FastLED.h>

class Configuration {
    public:
    //configuration for control module
    class ControlConfiguration {
        public:
            //pin number for mode button (none/clock/minutes control)
            static const int ModeButtonPin           = 2;

            //pin number for increase button (add hour or minute)
            static const int IncreaseButtonPin       = 4;

            //pin number for decrease button (substract hour or minute)
            static const int DecreaseButtonPin       = 6;

            //pin number for set hours mode led
            static const int HourModeLedPin          = 8;

            //pin number for set minutes mode led
            static const int MinuteModeLedPin        = 10;

            //timeout (in millis) for autoexit from set time mode
            static const int EditModeTimeout         = 5000;

            //edbounce interval for Bounce2 (millis)
            static const int ButtonDebounceInterval  = 5;
    };

    //configuration for clockface
    class ClockFaceConfiguration {
        public:        
            //enable move hour led between interger parts (i.e. if true then at 20 minutes hour led should be at second led in hour group, if false then hour led move only after 59 minutes 59 seconds reached)
            static const bool HourSmoothMove         = true;

            //use different brightness for night hours
            static const bool UseNightMode           = false;

            //hours when night node should be deactivated
            static const int NightEndAt              = 8;

            //hours when night node should be activated
            static const int NightStartAt            = 20;

            //brightness for nightmod
            static const int NightModeBrightness     = 20;

            //leds count in strip
            static const int LedsCount               = 60;

            //pin for led strip data
            static const int LedDataPin              = D1;

            //color for hour led
            static const CRGB ColorHour;

            //color for minute led
            static const CRGB ColorMinute;

            //color for second led
            static const CRGB ColorSecond;

            //color for led if hours and minutes in same position
            static const CRGB ColorHourMinute;

            //color for led if hours and seconds in same position
            static const CRGB ColorHourSecond;

            //color for led if minutes and seconds in same position
            static const CRGB ColorMinuteSecond;

            //color for led if hours, minutes and seconds in same position
            static const CRGB ColorAll;
    };

    //configuration for RTC timesource
    class RtcTimeSourceConfiguration {
        public:
            //sync internal timer with timesource (millis)
            static const unsigned long SyncInterval = 60000L;
    };

    //config for NTP timesource
    class NtpTimeSourceConfiguration {
        public:
            static const char* NTPServerName;
            static const char* SSID;
            static const char* Password;
            static const unsigned long TimeZone = 1;

            //sync internal timer with timesource (millis)
            static const unsigned long SyncInterval = 60000L;
    };
        
    //global configuration
    class SystemConfiguration {
        public:
            //delay (millis) between update iteration
            static const int TickTime               = 20;
    };       
};

const CRGB Configuration::ClockFaceConfiguration::ColorHour              = CRGB::Red;
const CRGB Configuration::ClockFaceConfiguration::ColorMinute            = CRGB::Green;
const CRGB Configuration::ClockFaceConfiguration::ColorSecond            = CRGB::Blue;
const CRGB Configuration::ClockFaceConfiguration::ColorHourMinute        = CRGB::Yellow;
const CRGB Configuration::ClockFaceConfiguration::ColorHourSecond        = CRGB::Magenta;
const CRGB Configuration::ClockFaceConfiguration::ColorMinuteSecond      = CRGB::Cyan;
const CRGB Configuration::ClockFaceConfiguration::ColorAll               = CRGB::White;

const char* Configuration::NtpTimeSourceConfiguration::NTPServerName = "ntp1.stratum2.ru";
const char* Configuration::NtpTimeSourceConfiguration::SSID = "";
const char* Configuration::NtpTimeSourceConfiguration::Password = "";