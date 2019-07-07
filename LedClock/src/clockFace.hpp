#pragma once

#include <FastLED.h>

#include <time.hpp>
#include <configuration.hpp>

class ClockFace {
    private:
        static char* _buffer;


    private:
        CRGB _clockFace[Configuration::ClockFaceConfiguration::LedsCount];

    public:
        void init() {
            FastLED.delay(3000);
            FastLED.addLeds<WS2812B, Configuration::ClockFaceConfiguration::LedDataPin, GRB>(_clockFace, Configuration::ClockFaceConfiguration::LedsCount);  
        }

        const char* toString() {
            return _buffer;                
        }

        void showTime(const Time& time) {
            for (int i = 0; i < Configuration::ClockFaceConfiguration::LedsCount; i++) 
                _clockFace[i] = CRGB::Black;
 
            int second = getSecondPosition(time);
            int minute = getMinutePosition(time);
            int hour = getHourPosition(time);

            _clockFace[second] = Configuration::ClockFaceConfiguration::ColorSecond;
            _clockFace[minute] = Configuration::ClockFaceConfiguration::ColorMinute;  
            _clockFace[hour] = Configuration::ClockFaceConfiguration::ColorHour;  

            if (hour == minute)
                _clockFace[hour] = Configuration::ClockFaceConfiguration::ColorHourMinute;

            if (hour == second)
                _clockFace[hour] = Configuration::ClockFaceConfiguration::ColorHourSecond;

            if (minute == second)
                _clockFace[minute] = Configuration::ClockFaceConfiguration::ColorMinuteSecond;

            if (minute == second && minute == hour)
                _clockFace[minute] = Configuration::ClockFaceConfiguration::ColorAll;

            if (isNight(time) && Configuration::ClockFaceConfiguration::UseNightMode == true )
                FastLED.setBrightness(Configuration::ClockFaceConfiguration::NightModeBrightness); 

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

            if (Configuration::ClockFaceConfiguration::HourSmoothMove == true)
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
            return !(now.getHours() >= Configuration::ClockFaceConfiguration::NightEndAt && now.getHours() <= Configuration::ClockFaceConfiguration::NightStartAt);
        }
};

char* ClockFace::_buffer = new char[20];