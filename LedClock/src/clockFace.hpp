#pragma once

#include <FastLED.h>

#include <component.hpp>
#include <time.hpp>
#include <configuration.hpp>

class ClockFace : public Component {
    private:
        static char* _buffer;

    private:
        CRGB _clockFace[Configuration::ClockFaceConfiguration::LedsCount];

    public:
        void init() {
            FastLED.delay(3000);
            FastLED.addLeds<WS2812B, Configuration::ClockFaceConfiguration::LedDataPin, GRB>(_clockFace, Configuration::ClockFaceConfiguration::LedsCount);  
        }

        virtual void selfCheck() {
            FastLED.setBrightness(255); 

            for (int i = 0; i < Configuration::ClockFaceConfiguration::LedsCount; i++) 
            {
                _clockFace[i] = CRGB::Green;                
                FastLED.show();

                delay(40);
            }

            for (int i = 0; i < Configuration::ClockFaceConfiguration::LedsCount; i++) 
            {
                _clockFace[i] = CRGB::Black;            
                FastLED.show();

                delay(40);
            }
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

            int ledsPerMinute = Configuration::ClockFaceConfiguration::LedsCount / 60;
            if (hours >= 12)
	            hours = hours - 12;
            
            int hourPosition = hours * ledsPerMinute * 5;
            if (Configuration::ClockFaceConfiguration::HourSmoothMove)
	            hourPosition += minutes * 5 * ledsPerMinute / 60;

            hourPosition += Configuration::ClockFaceConfiguration::ZeroLed;
            if (Configuration::ClockFaceConfiguration::Direction < 0)
	            hourPosition = Configuration::ClockFaceConfiguration::LedsCount - hourPosition;
            if (hourPosition < 0)
	            hourPosition = hourPosition + Configuration::ClockFaceConfiguration::LedsCount;
            if (hourPosition >= Configuration::ClockFaceConfiguration::LedsCount)
	            hourPosition = hourPosition - Configuration::ClockFaceConfiguration::LedsCount;

            return hourPosition;  
        }

        int getMinutePosition(const Time& time) {
            int minutes = time.getMinutes();
            int minutePosition = minutes + Configuration::ClockFaceConfiguration::ZeroLed;
            if (Configuration::ClockFaceConfiguration::Direction < 0)
	            minutePosition = Configuration::ClockFaceConfiguration::LedsCount - minutePosition;

            if (minutePosition < 0)
	            minutePosition = minutePosition + Configuration::ClockFaceConfiguration::LedsCount;
            if (minutePosition >= Configuration::ClockFaceConfiguration::LedsCount)
	            minutePosition = minutePosition  - Configuration::ClockFaceConfiguration::LedsCount;

            return minutePosition;
        }

        int getSecondPosition(const Time& time) {
            int seconds = time.getSeconds();
            int secondPosition = seconds + Configuration::ClockFaceConfiguration::ZeroLed;
            if (Configuration::ClockFaceConfiguration::Direction < 0)
	            secondPosition = Configuration::ClockFaceConfiguration::LedsCount - secondPosition;

            if (secondPosition < 0)
	            secondPosition = secondPosition + Configuration::ClockFaceConfiguration::LedsCount;
            if (secondPosition >= Configuration::ClockFaceConfiguration::LedsCount)
	            secondPosition = secondPosition  - Configuration::ClockFaceConfiguration::LedsCount;

            return secondPosition;            
        }

        bool isNight(const Time& now) {  
            return !(now.getHours() >= Configuration::ClockFaceConfiguration::NightEndAt && now.getHours() <= Configuration::ClockFaceConfiguration::NightStartAt);
        }
};

char* ClockFace::_buffer = new char[20];