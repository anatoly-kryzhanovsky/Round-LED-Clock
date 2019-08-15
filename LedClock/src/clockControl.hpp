#pragma once

#include <Arduino.h>
#include <Bounce2.h>

#include <component.hpp>
#include <timeSource.h>
#include <configuration.hpp>

class ClockControl: public Component {        
    enum SystemMode {
        Normal,
        Hours,
        Minutes
    };

    private:
        static char* _buffer;

    private:
        Configuration::ControlConfiguration* _configuration;
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
        ClockControl(TimeSource* _timeSource, Configuration::ControlConfiguration* configuration): 
            _configuration(configuration), _timeSource(_timeSource), _increaseButtonPressed(false), _decreaseButtonPressed(false), _modeButtonPressed(false), _prevModeChange(0),  _currentMode (SystemMode::Normal) {                                   
        }

        virtual void selfCheck() {
            for(int i = 0; i < 3; i++)
            {
                digitalWrite(Configuration::ControlConfiguration::HourModeLedPin, LOW);
                digitalWrite(Configuration::ControlConfiguration::MinuteModeLedPin, LOW);

                delay(500);

                digitalWrite(Configuration::ControlConfiguration::HourModeLedPin, HIGH);
                digitalWrite(Configuration::ControlConfiguration::MinuteModeLedPin, HIGH);

                delay(500);
            }

            digitalWrite(Configuration::ControlConfiguration::HourModeLedPin, LOW);
            digitalWrite(Configuration::ControlConfiguration::MinuteModeLedPin, LOW);
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
            _modeDebouncer.interval(_configuration->ButtonDebounceInterval);

            _increaseDebouncer.attach(Configuration::ControlConfiguration::IncreaseButtonPin);
            _increaseDebouncer.interval(_configuration->ButtonDebounceInterval);

            _decreaseDebouncer.attach(Configuration::ControlConfiguration::DecreaseButtonPin);
            _decreaseDebouncer.interval(_configuration->ButtonDebounceInterval);

            _currentMode = SystemMode::Normal;

            _prevModeChange = 0;
        }

        void update() {
            _modeDebouncer.update();
            _increaseDebouncer.update();
            _decreaseDebouncer.update();

            bool modeButtonPressed = _modeDebouncer.read() == HIGH;
            if(modeButtonPressed && modeButtonPressed != _modeButtonPressed) {               
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

            _modeButtonPressed = modeButtonPressed;

            int dHours = 0, dMinutes = 0;
            Time currentTime = _timeSource->getCurrentTime();

            bool increaseButtonPressed = _increaseDebouncer.read() == HIGH;
            bool decreaseButtonPressed = _decreaseDebouncer.read() == HIGH;  

            if(increaseButtonPressed && decreaseButtonPressed) {
                 Serial.println("RESET");

                _timeSource->reset();
                _increaseButtonPressed = increaseButtonPressed;
                _decreaseButtonPressed = decreaseButtonPressed;

                _currentMode = SystemMode::Normal;

                return;
            }

            if(increaseButtonPressed && _increaseButtonPressed != increaseButtonPressed) {
                switch(_currentMode) {
                    case SystemMode::Hours:
                        dHours = 1;
                        break;

                    case SystemMode::Minutes:
                        if(currentTime.getMinutes() == 59)
                            dHours = -1;
                        dMinutes = 1;
                        break;

                    default:
                        break;
                }

                _prevModeChange = millis();
            }

            _increaseButtonPressed = increaseButtonPressed;
                                 
            if(decreaseButtonPressed && _decreaseButtonPressed != decreaseButtonPressed) {
                switch(_currentMode) {
                    case SystemMode::Hours:
                        dHours = -1;
                        break;

                    case SystemMode::Minutes:
                        if(currentTime.getMinutes() == 0)
                            dHours = 1;
                        dMinutes = -1;
                        break;

                    default:
                        break;
                }

                _prevModeChange = millis();
            }

            _decreaseButtonPressed = decreaseButtonPressed;

            _timeSource->adjustTime(dHours, dMinutes);   

            if(millis() - _prevModeChange > _configuration->EditModeTimeout) {
                digitalWrite(Configuration::ControlConfiguration::HourModeLedPin, LOW);
                digitalWrite(Configuration::ControlConfiguration::MinuteModeLedPin, LOW);
                _currentMode = SystemMode::Normal;
            }         
        }        
};

char* ClockControl::_buffer = new char[255];