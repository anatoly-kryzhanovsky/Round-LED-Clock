#pragma once

#include "time.hpp"
#include <component.hpp>

class TimeSource: public Component {
  public:
    virtual void init() = 0;
    virtual void updateTime() = 0;

    virtual Time getCurrentTime() const = 0;

    virtual void setTime(int hour, int minute, int second) = 0;
    virtual void adjustTime(int dHour, int dMinute) = 0;
    virtual void reset() = 0;

    virtual const char* toString()  = 0;

    virtual void selfCheck() {
    };
};