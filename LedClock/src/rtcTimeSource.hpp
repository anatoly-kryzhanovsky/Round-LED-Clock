#pragma once

#include <Wire.h> 
#include <RtcDS1307.h>

#include <timeSource.h>
#include <configuration.hpp>

class RtcTimeSource: public TimeSource {
    private:
        static char* _buffer;

    private:
        Configuration::RtcTimeSourceConfiguration* _configuration;
        unsigned long _prevSyncTime;
        unsigned long _prevUpdateTime;
        Time _currentTime;
        RtcDS1307<TwoWire> _rtc;

    public:
        RtcTimeSource(Configuration::RtcTimeSourceConfiguration* configuration):
            _configuration(configuration), _prevSyncTime(0), _prevUpdateTime(0), _rtc(Wire) {            
        }

        const char* toString() {
            RtcDateTime rtcTime = _rtc.GetDateTime(); 
            
            sprintf(
                _buffer,
                "IsValid: %s, IsRunning: %s, RtcTime: %02d:%02d:%02d, InternalTime: %02d:%02d:%02d (%ld), TimeAge: %ld",
                _rtc.IsDateTimeValid() ? "yes" : "no", 
                _rtc.GetIsRunning() ? "yes" : "no", 
                rtcTime.Hour(),
                rtcTime.Minute(),
                rtcTime.Second(),
                _currentTime.getHours(),
                _currentTime.getMinutes(),
                _currentTime.getSeconds(),
                (long)_currentTime.totalMillis(),
                millis() - _prevSyncTime);               

            return _buffer;
        }

        virtual void setTime(int hour, int minute, int second) {
            _currentTime.setHours(hour);
            _currentTime.setMinutes(minute);
            _currentTime.setSeconds(second);

            RtcDateTime adjustedTime = RtcDateTime(0, 0, 0, _currentTime.getHours(), _currentTime.getMinutes(), _currentTime.getSeconds());
            _rtc.SetDateTime(adjustedTime);
        }

        virtual void init() {
            _rtc.Begin();
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

        virtual void updateTime() {
            unsigned long currentMillis = millis();

            if (_prevSyncTime == 0 || currentMillis - _prevSyncTime > _configuration->SyncInterval) {
                _prevSyncTime = currentMillis;
                Serial.println("Sync time with RTC");   
                RtcDateTime rtcTime = _rtc.GetDateTime(); 
                _currentTime.setHours(rtcTime.Hour());
                _currentTime.setMinutes(rtcTime.Minute());
                _currentTime.setSeconds(rtcTime.Second());                
            }
            else
                _currentTime.addMillis(currentMillis - _prevUpdateTime);

            _prevUpdateTime = currentMillis;
        }

        virtual Time getCurrentTime() const {
            return _currentTime;
        }

        virtual void reset() {
             RtcDateTime zero = RtcDateTime(0, 0, 0, 0, 0, 0);
             _currentTime.setHours(0);
             _currentTime.setMinutes(0);
             _currentTime.setSeconds(0);
             
             _rtc.SetDateTime(zero);
        }

        virtual void adjustTime(int dHour, int dMinute) {
            _currentTime.addMinutes(dMinute);
            _currentTime.addHours(dHour);

            RtcDateTime adjustedTime = RtcDateTime(0, 0, 0, _currentTime.getHours(), _currentTime.getMinutes(), _currentTime.getSeconds());
            _rtc.SetDateTime(adjustedTime);
        }
};

char* RtcTimeSource::_buffer = new char[255];