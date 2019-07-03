#pragma once

#include <cmath>

class Time {
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
        return (int)(_millis / (1000 * 60 * 60));
    }

    int getMinutes() const {
        return (int)(_millis / (1000 * 60));
    }

    int getSeconds() const {
        return (int)(_millis / 1000);
    }

    void addSeconds(double s) {
        _millis += s * 1000;
    }

    void addMinutes(double m) {
        _millis += m * 1000 * 60;
    }

    void addHours(double h) {
        _millis += h * 1000 * 60 * 60;
    }
};