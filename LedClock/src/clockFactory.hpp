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
            Configuration* configuration = new Configuration();
            configuration->init();
            configuration->load();

            TimeSource* timeSource = new RtcTimeSource(configuration->getRtcTimeSourceConfiguration());
            ClockControl* control = new ClockControl(timeSource, configuration->getControlConfiguration());
            ClockFace* clockFace = new ClockFace(configuration->getClockFaceConfiguration());

            return new Clock(configuration, timeSource, clockFace, control);
        }

        static Clock* createRtcClockWithoutControl()
        {
            Configuration* configuration = new Configuration();
            configuration->init();
            configuration->load();

            TimeSource* timeSource = new RtcTimeSource(configuration->getRtcTimeSourceConfiguration());            
            ClockFace* clockFace = new ClockFace(configuration->getClockFaceConfiguration());

            return new Clock(configuration, timeSource, clockFace, nullptr);
        }

        static Clock* createNtpClockWithControl()
        {
            Configuration* configuration = new Configuration();
            configuration->init();
            configuration->load();

            TimeSource* timeSource = new NtpTimeSource(configuration->getNtpTimeSourceConfiguration());
            ClockControl* control = new ClockControl(timeSource, configuration->getControlConfiguration());
            ClockFace* clockFace = new ClockFace(configuration->getClockFaceConfiguration());

            return new Clock(configuration, timeSource, clockFace, control);
        }

        static Clock* createNtpClockWithoutControl()
        {
            Configuration* configuration = new Configuration();
            configuration->init();
            configuration->load();

            TimeSource* timeSource = new NtpTimeSource(configuration->getNtpTimeSourceConfiguration());            
            ClockFace* clockFace = new ClockFace(configuration->getClockFaceConfiguration());

            return new Clock(configuration, timeSource, clockFace, nullptr);
        }
};