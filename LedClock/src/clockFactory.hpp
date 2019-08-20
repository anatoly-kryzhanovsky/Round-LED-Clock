#pragma once

#include <clock.hpp>
#include <clockControl.hpp>
#include <clockFace.hpp>
#include <rtcTimeSource.hpp>
#include <ntpTimeSource.hpp>
#include <remoteControl.hpp>

class ClockFactory {
    public:
        static Clock* createClockWithRemoeControl() {
            Configuration* configuration = new Configuration();
            configuration->init();
            configuration->load();

            TimeSource* timeSource;
            if(configuration->getSystemConfiguration()->ClockSourceType == Configuration::SystemConfiguration::RtcClockSourceType)
                timeSource = new RtcTimeSource(configuration->getRtcTimeSourceConfiguration());
            else if(configuration->getSystemConfiguration()->ClockSourceType == Configuration::SystemConfiguration::NtpClockSourceType)
                timeSource = new NtpTimeSource(configuration->getNtpTimeSourceConfiguration());

            ClockControl* control = new ClockControl(timeSource, configuration->getControlConfiguration());
            ClockFace* clockFace = new ClockFace(configuration->getClockFaceConfiguration());

            RemoteControl* remoteControl = new RemoteControl(timeSource, configuration);
            remoteControl->init();

            return new Clock(configuration, timeSource, clockFace, control, remoteControl);
        }

        static Clock* createRtcClockWithControl()
        {
            Configuration* configuration = new Configuration();
            configuration->init();
            configuration->load();

            TimeSource* timeSource = new RtcTimeSource(configuration->getRtcTimeSourceConfiguration());
            ClockControl* control = new ClockControl(timeSource, configuration->getControlConfiguration());
            ClockFace* clockFace = new ClockFace(configuration->getClockFaceConfiguration());

            return new Clock(configuration, timeSource, clockFace, control, nullptr);
        }

        static Clock* createRtcClockWithoutControl()
        {
            Configuration* configuration = new Configuration();
            configuration->init();
            configuration->load();

            TimeSource* timeSource = new RtcTimeSource(configuration->getRtcTimeSourceConfiguration());            
            ClockFace* clockFace = new ClockFace(configuration->getClockFaceConfiguration());

            return new Clock(configuration, timeSource, clockFace, nullptr, nullptr);
        }

        static Clock* createNtpClockWithControl()
        {
            Configuration* configuration = new Configuration();
            configuration->init();
            configuration->load();

            TimeSource* timeSource = new NtpTimeSource(configuration->getNtpTimeSourceConfiguration());
            ClockControl* control = new ClockControl(timeSource, configuration->getControlConfiguration());
            ClockFace* clockFace = new ClockFace(configuration->getClockFaceConfiguration());

            return new Clock(configuration, timeSource, clockFace, control, nullptr);
        }

        static Clock* createNtpClockWithoutControl()
        {
            Configuration* configuration = new Configuration();
            configuration->init();
            configuration->load();

            TimeSource* timeSource = new NtpTimeSource(configuration->getNtpTimeSourceConfiguration());            
            ClockFace* clockFace = new ClockFace(configuration->getClockFaceConfiguration());

            return new Clock(configuration, timeSource, clockFace, nullptr, nullptr);
        }
};