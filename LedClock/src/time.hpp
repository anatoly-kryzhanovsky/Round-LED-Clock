#pragma once

#include <math.h>

class Time {
  private:
    static const long DayTotalMillis = 1000L * 60L * 60L * 24L;
    static const long HourTotalMillis = 1000L * 60L * 60L;
    static const long MinuteTotalMillis = 1000L * 60L;
    static const long SecondTotalMillis = 1000L;

  private:
    static char* _buffer;

  private:
    double _millis;

  public:
    Time() : _millis(0) {        
    }
    
    Time(int hours, int minutes, int seconds) : _millis(0) {
        addSeconds(seconds);
        addMinutes(minutes);
        addHours(hours);        
    }

    void setHours(int hours) {
        int minutes = getMinutes();
        int seconds = getSeconds();
        _millis = hours * HourTotalMillis + minutes * MinuteTotalMillis + seconds * SecondTotalMillis;
        clamp();
    }

    void setMinutes(int minutes) {
        int hours = getHours();
        int seconds = getSeconds();
        _millis = hours * HourTotalMillis + minutes * MinuteTotalMillis + seconds * SecondTotalMillis;
        clamp();
    }

    void setSeconds(int seconds) {
        int hours = getHours();
        int minutes = getMinutes();
        _millis = hours * HourTotalMillis + minutes * MinuteTotalMillis + seconds * SecondTotalMillis;
        clamp();
    }

    int getHours() const {
        return (int)(_millis / HourTotalMillis);
    }

    int getMinutes() const {
        return (int)((long)(_millis / MinuteTotalMillis) % 60);
    }

    int getSeconds() const {
        return (int)((long)(_millis / SecondTotalMillis) % 60);
    }

    void addMillis(double m) {
        _millis += m;
        clamp();
    }

    void addSeconds(double s) {
        _millis += s * SecondTotalMillis;
        clamp();
    }

    void addMinutes(double m) {
        _millis += m * MinuteTotalMillis;
        clamp();
    }

    void addHours(double h) {
        _millis += h * HourTotalMillis;
        clamp();
    }

    double totalMillis() const {
        return _millis;
    }

    const char* toString() {
        sprintf(_buffer, "%02d:%02d:%02d.%d", getHours(), getMinutes(), getSeconds(), (int)_millis % 1000);
        return _buffer;
    }

  private:
    void clamp() {
        _millis = fmod(_millis, DayTotalMillis);
        if(_millis < 0)
            _millis = DayTotalMillis + _millis;
    }
};

char* Time::_buffer = new char[20];