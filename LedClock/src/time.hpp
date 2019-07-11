#pragma once

class Time {
  private:
    static const long HourTotalMillis = 1000L * 60L * 60L;
    static const long MinuteTotalMillis = 1000L * 60L * 60L;
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

    int getHours() const {
        return (int)(_millis / HourTotalMillis);
    }

    int getMinutes() const {
        return (int)(_millis / MinuteTotalMillis);
    }

    int getSeconds() const {
        return (int)(_millis / SecondTotalMillis);
    }

    void addSeconds(double s) {
        _millis += s * SecondTotalMillis;
    }

    void addMinutes(double m) {
        _millis += m * MinuteTotalMillis;
    }

    void addHours(double h) {
        _millis += h * HourTotalMillis;
    }

    const char* toString() {
        sprintf(_buffer, "%02d:%02d:%02d.%d", getHours(), getMinutes(), getSeconds(), (int)_millis % 1000);
        return _buffer;
    }
};

char* Time::_buffer = new char[20];