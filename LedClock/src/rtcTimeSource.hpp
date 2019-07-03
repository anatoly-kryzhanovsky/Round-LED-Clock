#pragma once

#include <Wire.h> 
#include <RtcDS1307.h>

#include <timeSource.h>
#include <configuration.hpp>

class RtcTimeSource: public TimeSource {
    private:
        

    private:
        unsigned long _prevSyncTime;
        Time _currentTime;
        RtcDS1307<TwoWire> _rtc;

    public:
        RtcTimeSource(): _rtc(Wire) {            
        }

        virtual void init() {
            RtcDateTime defaultTime = RtcDateTime(__DATE__, __TIME__);
            if (!_rtc.IsDateTimeValid()) {
                if (_rtc.LastError() != 0) {
                    Serial.print("RTC communications error = ");
                    Serial.println(_rtc.LastError());
                }
                else {
                    Serial.println("RTC lost confidence in the DateTime!");
                    _rtc.SetDateTime(defaultTime);
                }
            }
    
            if (!_rtc.GetIsRunning()) {
                Serial.println("RTC was not actively running, starting now");
                _rtc.SetIsRunning(true);
            }
            
            _rtc.SetSquareWavePin(DS1307SquareWaveOut_Low); 
        }

    virtual void updateTime(int tickTime) {
         unsigned long currentMillis = millis();

        if (_prevSyncTime == 0 || currentMillis - _prevSyncTime > Configuration::SyncInterval) {
            _prevSyncTime = currentMillis;
            Serial.println("Sync time with RTC");   
            RtcDateTime rtcTime = _rtc.GetDateTime(); 
            _currentTime = Time(rtcTime.Hour(), rtcTime.Minute(), rtcTime.Second());
        }
        else
            _currentTime.addSeconds(tickTime / 1000.0);
    }

    virtual Time getCurrentTime() const {
        return _currentTime;
    }

    virtual void adjustTime(int dHour, int dMinute) {
        _currentTime.addMinutes(dMinute);
        _currentTime.addHours(dHour);
    }
};