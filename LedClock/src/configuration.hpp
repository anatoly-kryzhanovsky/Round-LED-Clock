#pragma once

#include <FastLED.h>

class Configuration {
    public:
        static const int ModeButtonPin           = 1;
        static const int IncreaseButtonPin       = 1;
        static const int DecreaseButtonPin       = 1;
        static const int HourModeLedPin          = 1;
        static const int MinuteModeLedPin        = 1;
        static const int EditModeTimeout         = 5000;
        static const int ButtonDebounceInterval  = 5;

        static const unsigned long SyncInterval = 5 * 1000;

        static const bool HourSmoothMove         = true;
        static const bool UseNightMode           = false;
        static const int NightEndAt              = 8;
        static const int NightStartAt            = 20;
        static const int NightModeBrightness     = 20;
        static const unsigned long syncInterval  = 300000;
        static const int LedsCount               = 60;
        static const int LedDataPin              = 1;

        static const int TickTime = 20;

        static const CRGB ColorHour;
        static const CRGB ColorMinute;
        static const CRGB ColorSecond;
        static const CRGB ColorHourMinute;
        static const CRGB ColorHourSecond;
        static const CRGB ColorMinuteSecond;
        static const CRGB ColorAll;
};

const CRGB Configuration::ColorHour              = CRGB::Red;
const CRGB Configuration::ColorMinute            = CRGB::Green;
const CRGB Configuration::ColorSecond            = CRGB::Blue;
const CRGB Configuration::ColorHourMinute        = CRGB::Yellow;
const CRGB Configuration::ColorHourSecond        = CRGB::Magenta;
const CRGB Configuration::ColorMinuteSecond      = CRGB::Cyan;
const CRGB Configuration::ColorAll               = CRGB::White;