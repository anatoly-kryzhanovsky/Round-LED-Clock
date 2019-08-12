#pragma once

#include <timeSource.h>
#include <clockControl.hpp>
#include <clockFace.hpp>

class Clock {
    private:
        TimeSource* _timeSource;
        ClockFace* _clockFace;
        ClockControl* _clockControl;

        unsigned long _lastDebugTime;

    public:
        Clock(TimeSource* timeSource, ClockFace* clockFace, ClockControl* clockControl)
            : _timeSource(timeSource), _clockFace(clockFace), _clockControl(clockControl),  _lastDebugTime(0)
        {
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
        }

        void update() {
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