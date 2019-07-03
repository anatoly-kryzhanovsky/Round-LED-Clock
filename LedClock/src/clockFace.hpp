#pragma once

#include <FastLED.h>

#include <time.hpp>
#include <configuration.hpp>

class ClockFace {
    private:
        CRGB clockFace[Configuration::LedsCount];

    public:
        void init() {
            FastLED.delay(3000);
            FastLED.addLeds<WS2812B, Configuration::LedDataPin, GRB>(clockFace, Configuration::LedsCount);  
        }

        void showTime(const Time& time) {
            for (int i = 0; i < Configuration::LedsCount; i++) 
            clockFace[i] = CRGB::Black;
 
            int second = getSecondPosition(time);
            int minute = getMinutePosition(time);
            int hour = getHourPosition(time);

            clockFace[second] = Configuration::ColorSecond;
            clockFace[minute] = Configuration::ColorMinute;  
            clockFace[hour] = Configuration::ColorHour;  

            if ( hour == minute)
                clockFace[hour] = Configuration::ColorHourMinute;

            if ( hour == second)
                clockFace[hour] = Configuration::ColorHourSecond;

            if ( minute == second)
                clockFace[minute] = Configuration::ColorMinuteSecond;

            if ( minute == second && minute == hour)
                clockFace[minute] = Configuration::ColorAll;

            if (isNight(time) && Configuration::UseNightMode == true )
                FastLED.setBrightness(Configuration::NightModeBrightness); 

            FastLED.show();
        }

        int getHourPosition(const Time& time) {
            int hours = time.getHours();
            int minutes = time.getMinutes();

            if (hours > 12)
                hours = hours - 12;

            int hourPosition;
            if (hours <= 5) 
                hourPosition = (hours * 5) + 30;
            else  
                hourPosition = (hours * 5) - 30;

            if (Configuration::HourSmoothMove == true)
                hourPosition += (minutes / 12);

            return hourPosition;  
        }

        int getMinutePosition(const Time& time) {
            int minutes = time.getMinutes();
            if (minutes < 30) 
                return minutes + 30;
            else 
                return minutes - 30;
        }

        int getSecondPosition(const Time& time) {
            int minutes = time.getSeconds();
            if (minutes < 30) 
                return minutes + 30;
            else 
                return minutes - 30;
        }

        bool isNight(const Time& now) {  
            return !(now.getHours() >= Configuration::NightEndAt && now.getHours() <= Configuration::NightStartAt);
        }
};