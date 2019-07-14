#pragma once

#include "time.hpp"

class TimeSource {
  public:
    virtual void init() = 0;
    virtual void updateTime() = 0;

    virtual Time getCurrentTime() const = 0;

    virtual void adjustTime(int dHour, int dMinute) = 0;
};