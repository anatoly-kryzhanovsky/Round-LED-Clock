#pragma once

#include <timeSource.h>
#include <clockControl.hpp>
#include <clockFace.hpp>
#include <remoteControl.hpp>

class Clock {
    private:
        Configuration* _configuration;
        TimeSource* _timeSource;
        ClockFace* _clockFace;
        ClockControl* _clockControl;
        RemoteControl* _remoteControl;

        unsigned long _lastDebugTime;

    public:
        Clock(Configuration* configuration, TimeSource* timeSource, ClockFace* clockFace, ClockControl* clockControl, RemoteControl* remoteControl)
            : _configuration(configuration), _timeSource(timeSource), _clockFace(clockFace), _clockControl(clockControl), _remoteControl(remoteControl), _lastDebugTime(0)
        {
        }

        Configuration* getConfiguration() {
            return _configuration;
        }

        void init() {
            if(_clockControl) {
                _clockControl->init();
                _clockControl->selfCheck();
            }
            
            _timeSource->init();
            _timeSource->selfCheck();

            _clockFace->init();
            _clockFace->selfCheck();

            if(_remoteControl)
                _remoteControl->init();
        }

        void update() {
            if(_remoteControl)
                _remoteControl->update();

            _timeSource->updateTime();
            if(_clockControl)
                _clockControl->update();
  
            _clockFace->showTime(_timeSource->getCurrentTime());

#ifdef DEBUG
            if(millis() - _lastDebugTime > 5000)
            {
                _lastDebugTime = millis();
                Serial.println("TimeSource state");                
                Serial.println(_timeSource->toString());

                if(_clockControl)
                {
                    Serial.println("Control state");
                    Serial.println(_clockControl->toString());
                }

                Serial.println("ClockFace state");
                Serial.println(_clockFace->toString());
            }
#endif
        }
};