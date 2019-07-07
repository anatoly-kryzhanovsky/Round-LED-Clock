#pragma once

#include <FastLED.h>

class Configuration {
    public:
    class ControlConfiguration {
        public:
            static const int ModeButtonPin           = D8;
            static const int IncreaseButtonPin       = D7;
            static const int DecreaseButtonPin       = D6;
            static const int HourModeLedPin          = D0;
            static const int MinuteModeLedPin        = D5;
            static const int EditModeTimeout         = 5000;
            static const int ButtonDebounceInterval  = 5;
    };

    class ClockFaceConfiguration {
        public:        
            static const bool HourSmoothMove         = true;
            static const bool UseNightMode           = false;
            static const int NightEndAt              = 8;
            static const int NightStartAt            = 20;
            static const int NightModeBrightness     = 20;
            static const unsigned long syncInterval  = 300000;
            static const int LedsCount               = 60;
            static const int LedDataPin              = D4;

            static const CRGB ColorHour;
            static const CRGB ColorMinute;
            static const CRGB ColorSecond;
            static const CRGB ColorHourMinute;
            static const CRGB ColorHourSecond;
            static const CRGB ColorMinuteSecond;
            static const CRGB ColorAll;
    };

    class TimeSourceConfiguration {
        public:
            static const unsigned long SyncInterval = 5 * 1000;
    };
        
    class SystemConfiguration {
        public:
            static const int TickTime               = 5;
    };       
};

const CRGB Configuration::ClockFaceConfiguration::ColorHour              = CRGB::Red;
const CRGB Configuration::ClockFaceConfiguration::ColorMinute            = CRGB::Green;
const CRGB Configuration::ClockFaceConfiguration::ColorSecond            = CRGB::Blue;
const CRGB Configuration::ClockFaceConfiguration::ColorHourMinute        = CRGB::Yellow;
const CRGB Configuration::ClockFaceConfiguration::ColorHourSecond        = CRGB::Magenta;
const CRGB Configuration::ClockFaceConfiguration::ColorMinuteSecond      = CRGB::Cyan;
const CRGB Configuration::ClockFaceConfiguration::ColorAll               = CRGB::White;