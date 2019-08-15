#pragma once

#include <FastLED.h>

#include <component.hpp>
#include <time.hpp>
#include <configuration.hpp>

class ClockFace : public Component {
    private:
        static char* _buffer;

    private:
        CRGB* _clockFace;
        Configuration::ClockFaceConfiguration* _configuration;

    public:
        ClockFace(Configuration::ClockFaceConfiguration* configuration)
            : _configuration(configuration) {
            _clockFace = new CRGB[configuration->LedsCount];
        }

        void init() {
            FastLED.delay(3000);
            FastLED.addLeds<WS2812B, Configuration::ClockFaceConfiguration::LedDataPin, GRB>(_clockFace, _configuration->LedsCount);  
        }

        virtual void selfCheck() {
            FastLED.setBrightness(255); 

            for (int i = 0; i < _configuration->LedsCount; i++) 
            {
                _clockFace[i] = CRGB::Green;                
                FastLED.show();

                delay(40);
            }

            for (int i = 0; i < _configuration->LedsCount; i++) 
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
            for (int i = 0; i < _configuration->LedsCount; i++) 
                _clockFace[i] = CRGB::Black;
 
            int second = getSecondPosition(time);
            int minute = getMinutePosition(time);
            int hour = getHourPosition(time);

            _clockFace[second] = _configuration->ColorSecond;
            _clockFace[minute] = _configuration->ColorMinute;  
            _clockFace[hour] = _configuration->ColorHour;  

            if (hour == minute)
                _clockFace[hour] = _configuration->ColorHourMinute;

            if (hour == second)
                _clockFace[hour] = _configuration->ColorHourSecond;

            if (minute == second)
                _clockFace[minute] = _configuration->ColorMinuteSecond;

            if (minute == second && minute == hour)
                _clockFace[minute] = _configuration->ColorAll;

            if (isNight(time) && _configuration->UseNightMode == true )
                FastLED.setBrightness(_configuration->NightModeBrightness); 
            else
                FastLED.setBrightness(255); 

            FastLED.show();
        }

        int getHourPosition(const Time& time) {
            int hours = time.getHours();
            int minutes = time.getMinutes();

            int ledsPerMinute = _configuration->LedsCount / 60;
            if (hours >= 12)
	            hours = hours - 12;
            
            int hourPosition = hours * ledsPerMinute * 5;
            if (_configuration->HourSmoothMove)
	            hourPosition += minutes * 5 * ledsPerMinute / 60;

            hourPosition += _configuration->ZeroLed;
            if (_configuration->Direction < 0)
	            hourPosition = _configuration->LedsCount - hourPosition;
            if (hourPosition < 0)
	            hourPosition = hourPosition + _configuration->LedsCount;
            if (hourPosition >= _configuration->LedsCount)
	            hourPosition = hourPosition - _configuration->LedsCount;

            return hourPosition;  
        }

        int getMinutePosition(const Time& time) {
            int minutes = time.getMinutes();
            int minutePosition = minutes + _configuration->ZeroLed;
            if (_configuration->Direction < 0)
	            minutePosition = _configuration->LedsCount - minutePosition;

            if (minutePosition < 0)
	            minutePosition = minutePosition + _configuration->LedsCount;
            if (minutePosition >= _configuration->LedsCount)
	            minutePosition = minutePosition  - _configuration->LedsCount;

            return minutePosition;
        }

        int getSecondPosition(const Time& time) {
            int seconds = time.getSeconds();
            int secondPosition = seconds + _configuration->ZeroLed;
            if (_configuration->Direction < 0)
	            secondPosition = _configuration->LedsCount - secondPosition;

            if (secondPosition < 0)
	            secondPosition = secondPosition + _configuration->LedsCount;
            if (secondPosition >= _configuration->LedsCount)
	            secondPosition = secondPosition  - _configuration->LedsCount;

            return secondPosition;            
        }

        bool isNight(const Time& now) {  
            return !(now.getHours() >= _configuration->NightEndAt && now.getHours() <= _configuration->NightStartAt);
        }
};

char* ClockFace::_buffer = new char[20];