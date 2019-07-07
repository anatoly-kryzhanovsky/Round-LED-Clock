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
        static char* _buffer;

    private:
        TimeSource* _timeSource;
        bool _increaseButtonPressed;
        bool _decreaseButtonPressed;
        bool _modeButtonPressed;
        unsigned long _prevModeChange;
        SystemMode _currentMode;
        Bounce _modeDebouncer;
        Bounce _increaseDebouncer;
        Bounce _decreaseDebouncer;        
    
    public:
        ClockControl(TimeSource* _timeSource): 
            _timeSource(_timeSource), _increaseButtonPressed(false), _decreaseButtonPressed(false), _modeButtonPressed(false), _prevModeChange(0),  _currentMode (SystemMode::Normal) {            

            pinMode(Configuration::ControlConfiguration::ModeButtonPin, INPUT);                
            pinMode(Configuration::ControlConfiguration::IncreaseButtonPin, INPUT);                
            pinMode(Configuration::ControlConfiguration::DecreaseButtonPin, INPUT);                
        }

        const char* toString() {
            const char* modeName = "Normal";
            if(_currentMode == SystemMode::Hours)
                modeName = "Hours";
            if(_currentMode == SystemMode::Minutes)
                modeName = "Minutes";

            sprintf(
                _buffer, 
                "ModeButton: %s, IncreaseButton: %s, DecreaseButton: %s, CurrentState: %s, Last action time: %ld", 
                _modeButtonPressed ? "pressed": "normal",
                _increaseButtonPressed ? "pressed": "normal",
                _decreaseButtonPressed ? "pressed": "normal",
                modeName,
                millis() - _prevModeChange);            
            
            return _buffer;
        }

        void init() {
            pinMode(Configuration::ControlConfiguration::ModeButtonPin, INPUT);
            pinMode(Configuration::ControlConfiguration::IncreaseButtonPin, INPUT);
            pinMode(Configuration::ControlConfiguration::DecreaseButtonPin, INPUT);
            pinMode(Configuration::ControlConfiguration::HourModeLedPin, OUTPUT);
            pinMode(Configuration::ControlConfiguration::MinuteModeLedPin, OUTPUT);

            _modeDebouncer.attach(Configuration::ControlConfiguration::ModeButtonPin);
            _modeDebouncer.interval(Configuration::ControlConfiguration::ButtonDebounceInterval);

            _increaseDebouncer.attach(Configuration::ControlConfiguration::IncreaseButtonPin);
            _increaseDebouncer.interval(Configuration::ControlConfiguration::ButtonDebounceInterval);

            _decreaseDebouncer.attach(Configuration::ControlConfiguration::DecreaseButtonPin);
            _decreaseDebouncer.interval(Configuration::ControlConfiguration::ButtonDebounceInterval);

            _currentMode = SystemMode::Normal;

            _prevModeChange = 0;
        }

        void update() {
            _modeDebouncer.update();
            _increaseDebouncer.update();
            _decreaseDebouncer.update();

            _modeButtonPressed = _modeDebouncer.read() == HIGH;
            if(_modeButtonPressed) {
                switch(_currentMode) {
                    case SystemMode::Normal:
                        _currentMode = SystemMode::Hours;
                        digitalWrite(Configuration::ControlConfiguration::HourModeLedPin, HIGH);
                        digitalWrite(Configuration::ControlConfiguration::MinuteModeLedPin, LOW);
                        break;

                    case SystemMode::Hours:
                        _currentMode = SystemMode::Minutes;
                        digitalWrite(Configuration::ControlConfiguration::HourModeLedPin, LOW);
                        digitalWrite(Configuration::ControlConfiguration::MinuteModeLedPin, HIGH);
                        break;

                    case SystemMode::Minutes:
                        _currentMode = SystemMode::Normal;
                        digitalWrite(Configuration::ControlConfiguration::HourModeLedPin, LOW);
                        digitalWrite(Configuration::ControlConfiguration::MinuteModeLedPin, LOW);
                        break;
                }

                _prevModeChange = millis();
            }

            int dHours = 0, dMinutes = 0;

            _increaseButtonPressed = _increaseDebouncer.read() == HIGH;            
            if(_increaseButtonPressed) {
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

            _decreaseButtonPressed = _decreaseDebouncer.read() == HIGH;            
            if(_decreaseButtonPressed) {
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

            if(millis() - _prevModeChange > Configuration::ControlConfiguration::EditModeTimeout) {
                digitalWrite(Configuration::ControlConfiguration::HourModeLedPin, LOW);
                digitalWrite(Configuration::ControlConfiguration::MinuteModeLedPin, LOW);
                _currentMode = SystemMode::Normal;
            }         
        }        
};

char* ClockControl::_buffer = new char[255];