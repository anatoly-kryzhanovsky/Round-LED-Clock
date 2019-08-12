#pragma once

#include <clock.hpp>
#include <clockControl.hpp>
#include <clockFace.hpp>
#include <rtcTimeSource.hpp>
#include <ntpTimeSource.hpp>

class ClockFactory {
    public:
        static Clock* createRtcClockWithControl()
        {
            TimeSource* timeSource = new RtcTimeSource();
            ClockControl* control = new ClockControl(timeSource);
            ClockFace* clockFace = new ClockFace();

            return new Clock(timeSource, clockFace, control);
        }

        static Clock* createRtcClockWithoutControl()
        {
            TimeSource* timeSource = new RtcTimeSource();            
            ClockFace* clockFace = new ClockFace();

            return new Clock(timeSource, clockFace, nullptr);
        }

        static Clock* createNtpClockWithControl()
        {
            TimeSource* timeSource = new NtpTimeSource();
            ClockControl* control = new ClockControl(timeSource);
            ClockFace* clockFace = new ClockFace();

            return new Clock(timeSource, clockFace, control);
        }

         static Clock* createNtpClockWithoutControl()
        {
            TimeSource* timeSource = new NtpTimeSource();            
            ClockFace* clockFace = new ClockFace();

            return new Clock(timeSource, clockFace, nullptr);
        }
};