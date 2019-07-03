#pragma once

#include <Arduino.h>
#include <Bounce2.h>

#include <timeSource.h>
#include <configuration.hpp>

class ClockControl {        
    enum SystemMode {
        Normal,
        Hours,
        Minutes
    };

    private:
        Bounce _modeDebouncer;
        Bounce _increaseDebouncer;
        Bounce _decreaseDebouncer;
        SystemMode _currentMode;
        unsigned long _prevModeChange;
        TimeSource* _timeSource;
    
    public:
        ClockControl(TimeSource* _timeSource): _timeSource(_timeSource) {            
        }

        void init() {
            pinMode(Configuration::ModeButtonPin, INPUT);
            pinMode(Configuration::IncreaseButtonPin, INPUT);
            pinMode(Configuration::DecreaseButtonPin, INPUT);
            pinMode(Configuration::HourModeLedPin, OUTPUT);
            pinMode(Configuration::MinuteModeLedPin, OUTPUT);

            _modeDebouncer.attach(Configuration::ModeButtonPin);
            _modeDebouncer.interval(Configuration::ButtonDebounceInterval);

            _increaseDebouncer.attach(Configuration::IncreaseButtonPin);
            _increaseDebouncer.interval(Configuration::ButtonDebounceInterval);

            _decreaseDebouncer.attach(Configuration::DecreaseButtonPin);
            _decreaseDebouncer.interval(Configuration::ButtonDebounceInterval);

            _currentMode = SystemMode::Normal;

            _prevModeChange = 0;
        }

        void update() {
            _modeDebouncer.update();
            _increaseDebouncer.update();
            _decreaseDebouncer.update();

            bool modePressed = _modeDebouncer.read() == LOW;
            if(modePressed) {
                switch(_currentMode) {
                    case SystemMode::Normal:
                        _currentMode = SystemMode::Hours;
                        digitalWrite(Configuration::HourModeLedPin, HIGH);
                        digitalWrite(Configuration::MinuteModeLedPin, LOW);
                        break;

                    case SystemMode::Hours:
                        _currentMode = SystemMode::Minutes;
                        digitalWrite(Configuration::HourModeLedPin, LOW);
                        digitalWrite(Configuration::MinuteModeLedPin, HIGH);
                        break;

                    case SystemMode::Minutes:
                        _currentMode = SystemMode::Normal;
                        digitalWrite(Configuration::HourModeLedPin, LOW);
                        digitalWrite(Configuration::MinuteModeLedPin, LOW);
                        break;
                }

                _prevModeChange = millis();
            }

            int dHours = 0, dMinutes = 0;

            bool increasePressed = _increaseDebouncer.read() == LOW;            
            if(increasePressed) {
                switch(_currentMode) {
                    case SystemMode::Hours:
                        dHours = 1;
                        break;

                    case SystemMode::Minutes:
                        dMinutes = 1;
                        break;

                    default:
                        break;
                }

                _prevModeChange = millis();
            }

            bool decreasePressed = _decreaseDebouncer.read() == LOW;            
            if(decreasePressed) {
                switch(_currentMode) {
                    case SystemMode::Hours:
                        dHours -= 1;
                        break;

                    case SystemMode::Minutes:
                        dMinutes -= 1;
                        break;

                    default:
                        break;
                }

                _prevModeChange = millis();
            }

            _timeSource->adjustTime(dHours, dMinutes);   

            if(millis() - _prevModeChange > Configuration::EditModeTimeout) {
                digitalWrite(Configuration::HourModeLedPin, LOW);
                digitalWrite(Configuration::MinuteModeLedPin, LOW);
                _currentMode = SystemMode::Normal;
            }         
        }        
};