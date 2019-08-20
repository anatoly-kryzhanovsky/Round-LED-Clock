#pragma once

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiUdp.h>

#include <timeSource.h>
#include <configuration.hpp>

class RemoteControl {
    private:
        static const int GetPlatformInfoCommandId               = 1;
        static const int GetCurrentTimeCommandId                = 2;
        static const int GetClockFaceConfigurationCommandId     = 3;
        static const int GetControlConfigurationCommandId       = 4;
        static const int GetRtcTimeSourceConfigurationCommandId = 5;
        static const int GetNtpTimeSourceConfigurationCommandId = 6;
        static const int GetSystemConfigurationCommandId        = 7;
        static const int GetRemoteControlConfigurationCommandId = 8;
        static const int SetClockFaceConfigurationCommandId     = 9;
        static const int SetControlConfigurationCommandId       = 10;
        static const int SetRtcTimeSourceConfigurationCommandId = 11;
        static const int SetNtpTimeSourceConfigurationCommandId = 12;
        static const int SetSystemConfigurationCommandId        = 13;
        static const int SetRemoteControlConfigurationCommandId = 14;
        static const int ResetClockCommandId                    = 15;
        static const int SetCurrentTimeCommandId                = 16;

    private:
        Configuration* _configuration;
        TimeSource* _timeSource;
        WiFiServer _server;
        WiFiClient _client;        

        void(* resetFunc) (void) = 0;

    public:
        RemoteControl(
            TimeSource* timeSource,
            Configuration* configuration)
            : _configuration(configuration), _timeSource(timeSource), _server(Configuration::RemoteControlConfiguration::IncomingPort) {
        }

        void init() {
            WiFi.softAPConfig(IPAddress(192, 168, 0, 1), IPAddress(192, 168, 0, 1), IPAddress(255, 255, 255, 0));
            WiFi.softAP(_configuration->getRemoteControlConfiguration()->SSID, _configuration->getRemoteControlConfiguration()->Password);
            
            _server.begin();           
        }

        void update() {
            if(!_client.connected())
                _client = _server.available();
            else {
                if(_client.available() > 0) {
                    int commandId = _client.read();
                    switch(commandId) {
                        case ResetClockCommandId:
                            handleResetClockCommand();
                            break;

                        case GetPlatformInfoCommandId:
                            handleGetCurrentTimeCommand();
                            break;

                        case SetCurrentTimeCommandId:
                            handleSetCurrentTimeCommand();
                            break;

                        case GetCurrentTimeCommandId:
                            handleGetCurrentTimeCommand();
                            break;

                        case GetClockFaceConfigurationCommandId:
                            handleGetClockFaceConfigurationCommand();
                            break;

                        case GetControlConfigurationCommandId:
                            handleGetControlConfigurationCommand();
                            break;

                        case GetRtcTimeSourceConfigurationCommandId:
                            handleGetRtcTimeSourceConfigurationCommand();
                            break;

                        case GetNtpTimeSourceConfigurationCommandId:
                            handleGetNtpTimeSourceConfigurationCommand();
                            break;

                        case GetSystemConfigurationCommandId:
                            handleGetSystemConfigurationCommand();
                            break;

                        case GetRemoteControlConfigurationCommandId:
                            handleGetRemoteControlConfigurationCommand();
                            break;

                        case SetClockFaceConfigurationCommandId:
                            handleSetClockFaceConfigurationCommand();
                            break;

                        case SetControlConfigurationCommandId:
                            handleSetControlConfigurationCommand();
                            break;

                        case SetRtcTimeSourceConfigurationCommandId:
                            handleSetRtcTimeSourceConfigurationCommand();
                            break;

                        case SetNtpTimeSourceConfigurationCommandId:
                            handleSetNtpTimeSourceConfigurationCommand();
                            break;

                        case SetSystemConfigurationCommandId:
                            handleSetSystemConfigurationCommand();
                            break;
                        
                        case SetRemoteControlConfigurationCommandId:
                            handleSetRemoteControlConfigurationCommand();
                            break;
                    }
                }
            }                
        }

    private:
        void handleResetClockCommand() {
            resetFunc();
        }

        void handleGetPlatformInfoCommand() {
            _client.write((uint8_t)sizeof(int));
            _client.write((uint8_t)sizeof(long));
        }

        void handleGetCurrentTimeCommand() {
            Time time = _timeSource->getCurrentTime();
            _client.write(time.getHours());
            _client.write(time.getMinutes());
            _client.write(time.getSeconds());
        }

        void handleSetCurrentTimeCommand() {
            int hour = _client.read();
            int minutes = _client.read();
            int seconds = _client.read();

            _timeSource->setTime(hour, minutes, seconds);
        }

        void handleGetClockFaceConfigurationCommand() {
            Configuration::ClockFaceConfiguration* cfg = _configuration->getClockFaceConfiguration();
            _client.write(cfg->ZeroLed);
            _client.write(cfg->Direction);
            _client.write(cfg->HourSmoothMove);
            _client.write(cfg->UseNightMode);
            _client.write(cfg->NightEndAt);
            _client.write(cfg->NightStartAt);
            _client.write(cfg->NightModeBrightness);
            _client.write(cfg->LedsCount);
            writeColor(cfg->ColorHour);
            writeColor(cfg->ColorMinute);
            writeColor(cfg->ColorSecond);
            writeColor(cfg->ColorHourMinute);
            writeColor(cfg->ColorHourSecond);
            writeColor(cfg->ColorMinuteSecond);
            writeColor(cfg->ColorAll);
        }

        void handleGetControlConfigurationCommand() {
            Configuration::ControlConfiguration* cfg = _configuration->getControlConfiguration();
            _client.write(cfg->EditModeTimeout);
            _client.write(cfg->ButtonDebounceInterval);
        }

        void handleGetRtcTimeSourceConfigurationCommand() {
            Configuration::RtcTimeSourceConfiguration* cfg = _configuration->getRtcTimeSourceConfiguration();
            _client.write(cfg->SyncInterval);
        }

        void handleGetNtpTimeSourceConfigurationCommand() {
            Configuration::NtpTimeSourceConfiguration* cfg = _configuration->getNtpTimeSourceConfiguration();
            writeString(cfg->NTPServerName);
            writeString(cfg->SSID);
            writeString(cfg->Password);
            _client.write(cfg->TimeZone);
        }

        void handleGetSystemConfigurationCommand() {
            Configuration::SystemConfiguration* cfg = _configuration->getSystemConfiguration();
            _client.write(cfg->TickTime);
        }

        void handleGetRemoteControlConfigurationCommand() {
            Configuration::RemoteControlConfiguration* cfg = _configuration->getRemoteControlConfiguration();
            writeString(cfg->SSID);
            writeString(cfg->Password);
        }

        void handleSetClockFaceConfigurationCommand() {
            Configuration::ClockFaceConfiguration* cfg = _configuration->getClockFaceConfiguration();
            cfg->ZeroLed = _client.read();
            cfg->Direction = _client.read();
            cfg->HourSmoothMove = _client.read();
            cfg->UseNightMode = _client.read();
            cfg->NightEndAt = _client.read();
            cfg->NightStartAt = _client.read();
            cfg->NightModeBrightness = _client.read();
            cfg->LedsCount = _client.read();
            readColor(cfg->ColorHour);
            readColor(cfg->ColorMinute);
            readColor(cfg->ColorSecond);
            readColor(cfg->ColorHourMinute);
            readColor(cfg->ColorHourSecond);
            readColor(cfg->ColorMinuteSecond);
            readColor(cfg->ColorAll);

            _configuration->save();
        }

        void handleSetControlConfigurationCommand() {
            Configuration::ControlConfiguration* cfg = _configuration->getControlConfiguration();
            cfg->EditModeTimeout = readInt();
            cfg->ButtonDebounceInterval = readInt();

            _configuration->save();
        }

        void handleSetRtcTimeSourceConfigurationCommand() {
            Configuration::RtcTimeSourceConfiguration* cfg = _configuration->getRtcTimeSourceConfiguration();
            cfg->SyncInterval = readLong();

            _configuration->save();
        }
        
        void handleSetNtpTimeSourceConfigurationCommand() {
            Configuration::NtpTimeSourceConfiguration* cfg = _configuration->getNtpTimeSourceConfiguration();
            delete[] cfg->NTPServerName;
            delete[] cfg->SSID;
            delete[] cfg->Password;
            
            cfg->NTPServerName = readString();
            cfg->SSID = readString();
            cfg->Password = readString();
            cfg->TimeZone = readLong();

            _configuration->save();
        }

        void handleSetSystemConfigurationCommand() {
            Configuration::SystemConfiguration* cfg = _configuration->getSystemConfiguration();
            cfg->TickTime = readInt();

            _configuration->save();
        }

        void handleSetRemoteControlConfigurationCommand() {
            Configuration::RemoteControlConfiguration* cfg = _configuration->getRemoteControlConfiguration();
            delete[] cfg->SSID;
            delete[] cfg->Password;
            
            cfg->SSID = readString();
            cfg->Password = readString();

            _configuration->save();
        }

        void writeColor(const CRGB& color) {
            _client.write(color.r);
            _client.write(color.g);
            _client.write(color.b);
        }

        void readColor(CRGB& color) {
            color.r = _client.read();
            color.g = _client.read();
            color.b = _client.read();
        }
      
        int readInt() {
            int value = 0;
            for (size_t i = 0; i < sizeof(int); i++)
                value |= _client.read() << (i * 8);
            
            return value;
        }
      
        long readLong() {
            int value = 0;
            for (size_t i = 0; i < sizeof(long); i++)
                value |= _client.read() << (i * 8);

            return value;
        }

        void writeString(const char* str) {
            int strLen = strlen(str);
            _client.write(strLen);
            for(int i = 0; i < strLen; i++)
                _client.write(str[i]);
        }
 
        char* readString() {
            int strLen = readInt();            
            char* result = new char[strLen];
            for(int i = 0; i < strLen; i++)
                result[i] = _client.read();

            return result;
        }
};

#endif