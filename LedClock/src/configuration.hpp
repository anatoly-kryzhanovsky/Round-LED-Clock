#pragma once

#include <FastLED.h>
#include <EEPROM.h>

#define DEBUG

class Configuration {
    public:
    //configuration for remote control module
    class RemoteControlConfiguration {
        public:
            char* SSID;
            char* Password;

            static const int IncomingPort = 1024; 
            
        public:
            void load(int &offset) {               
            }

            void save(int &offset) {                
            }

            void loadDefaults(int &offset) {                
            }

            size_t size() const {
                return sizeof(int);
            }
    };

    //configuration for control module
    class ControlConfiguration {
        public:
            //pin number for mode button (none/clock/minutes control)
            static const int ModeButtonPin           = D6;

            //pin number for increase button (add hour or minute)
            static const int IncreaseButtonPin       = D5;

            //pin number for decrease button (substract hour or minute)
            static const int DecreaseButtonPin       = D0;

            //pin number for set hours mode led
            static const int HourModeLedPin          = D4;

            //pin number for set minutes mode led
            static const int MinuteModeLedPin        = D3;

            //timeout (in millis) for autoexit from set time mode
            unsigned int EditModeTimeout;

            //edbounce interval for Bounce2 (millis)
            int ButtonDebounceInterval;

        private:
            static const unsigned int EditModeTimeoutDefault  = 5000;
            static const int ButtonDebounceIntervalDefault  = 5;

        public:
            void load(int &offset) {
                EEPROM.get(offset, EditModeTimeout);
                offset += sizeof(unsigned int);

                EEPROM.get(offset, ButtonDebounceInterval);
                offset += sizeof(int);
            }

            void save(int &offset) {
                EEPROM.put(offset, EditModeTimeout);
                offset += sizeof(unsigned int);

                EEPROM.put(offset, ButtonDebounceInterval);
                offset += sizeof(int);
            }

            void loadDefaults(int &offset) {
                EditModeTimeout = EditModeTimeoutDefault;
                ButtonDebounceInterval = ButtonDebounceIntervalDefault;

                save(offset);
            }

            size_t size() const {
                return sizeof(unsigned int) + sizeof(int);
            }
    };

    //configuration for clockface
    class ClockFaceConfiguration {
        public:        
            //pin for led strip data
            static const int LedDataPin              = D7;

            //led number for zero count
            int8_t ZeroLed;
            
            //direction for leds numbering (+1 for clockwise, -1 for counterclockwise)
            int8_t Direction;
            
            //enable move hour led between interger parts (i.e. if true then at 20 minutes hour led should be at second led in hour group, if false then hour led move only after 59 minutes 59 seconds reached)
            bool HourSmoothMove;

            //use different brightness for night hours
            bool UseNightMode;

            //hours when night node should be deactivated
            int8_t NightEndAt;

            //hours when night node should be activated
            int8_t NightStartAt;

            //brightness for nightmod
            int8_t NightModeBrightness;

            //leds count in strip
            int8_t LedsCount;            

            //color for hour led
            CRGB ColorHour;

            //color for minute led
            CRGB ColorMinute;

            //color for second led
            CRGB ColorSecond;

            //color for led if hours and minutes in same position
            CRGB ColorHourMinute;

            //color for led if hours and seconds in same position
            CRGB ColorHourSecond;

            //color for led if minutes and seconds in same position
            CRGB ColorMinuteSecond;

            //color for led if hours, minutes and seconds in same position
            CRGB ColorAll;

        private:
            static const CRGB ColorAllDefault;
            static const CRGB ColorMinuteSecondDefault;
            static const CRGB ColorHourSecondDefault;
            static const CRGB ColorHourMinuteDefault;
            static const CRGB ColorSecondDefault;
            static const CRGB ColorMinuteDefault;
            static const CRGB ColorHourDefault;
            static const int8_t LedsCountDefault                = 60;
            static const int8_t NightModeBrightnessDefault      = 40;
            static const int8_t NightStartAtDefault             = 22;
            static const byte NightEndAtDefault                 = 6;
            static const bool UseNightModeDefault               = true;
            static const bool HourSmoothMoveDefault             = true;
            static const int8_t DirectionDefault                = -1;
            static const int8_t ZeroLedDefault                  = 30;

        public:
           void load(int &offset) {
                EEPROM.get(offset, ZeroLed);
                offset += sizeof(int8_t);

                EEPROM.get(offset, Direction);
                offset += sizeof(int8_t);

                EEPROM.get(offset, HourSmoothMove);
                offset += sizeof(bool);

                EEPROM.get(offset, UseNightMode);
                offset += sizeof(bool);

                EEPROM.get(offset, NightEndAt);
                offset += sizeof(int8_t);

                EEPROM.get(offset, NightStartAt);
                offset += sizeof(int8_t);

                EEPROM.get(offset, NightModeBrightness);
                offset += sizeof(int8_t);

                EEPROM.get(offset, LedsCount);
                offset += sizeof(int8_t);
             
                loadCRGBColor(offset, ColorHour);
                loadCRGBColor(offset, ColorMinute);
                loadCRGBColor(offset, ColorSecond);
                loadCRGBColor(offset, ColorHourMinute);
                loadCRGBColor(offset, ColorHourSecond);
                loadCRGBColor(offset, ColorMinuteSecond);
                loadCRGBColor(offset, ColorAll);
            }

            void save(int &offset) {
                EEPROM.put(offset, ZeroLed);
                offset += sizeof(int8_t);

                EEPROM.put(offset, Direction);
                offset += sizeof(int8_t);

                EEPROM.put(offset, HourSmoothMove);
                offset += sizeof(bool);

                EEPROM.put(offset, UseNightMode);
                offset += sizeof(bool);

                EEPROM.put(offset, NightEndAt);
                offset += sizeof(int8_t);

                EEPROM.put(offset, NightStartAt);
                offset += sizeof(int8_t);

                EEPROM.put(offset, NightModeBrightness);
                offset += sizeof(int8_t);

                EEPROM.put(offset, LedsCount);
                offset += sizeof(int8_t);
                
                saveCRGBColor(offset, ColorHour);
                saveCRGBColor(offset, ColorMinute);
                saveCRGBColor(offset, ColorSecond);
                saveCRGBColor(offset, ColorHourMinute);
                saveCRGBColor(offset, ColorHourSecond);
                saveCRGBColor(offset, ColorMinuteSecond);
                saveCRGBColor(offset, ColorAll);
            }

            void loadDefaults(int &offset) {
                ZeroLed = ZeroLed;
                Direction = DirectionDefault;
                HourSmoothMove = HourSmoothMoveDefault;
                UseNightMode= UseNightModeDefault;
                NightEndAt = NightEndAtDefault;
                NightStartAt = NightStartAt;
                NightModeBrightness = NightModeBrightnessDefault;
                LedsCount = LedsCountDefault;
                ColorHour = ColorHourDefault;
                ColorMinute = ColorMinuteDefault;
                ColorSecond = ColorSecondDefault;
                ColorHourMinute = ColorHourMinuteDefault;
                ColorHourSecond = ColorHourSecondDefault;
                ColorMinuteSecond = ColorMinuteSecondDefault;
                ColorAll = ColorAllDefault;

                save(offset);
            }

            size_t size() const {
                return sizeof(int) + sizeof(int);
            }

        private:
            void loadCRGBColor(int &offset, CRGB& target) const {
                int8_t r, g, b;

                EEPROM.get(offset, r);
                offset += sizeof(int8_t);

                EEPROM.get(offset, g);
                offset += sizeof(int8_t);

                EEPROM.get(offset, b);
                offset += sizeof(int8_t);

                target.r = r;
                target.g = g;
                target.b = b;
            }

            void saveCRGBColor(int &offset, const CRGB& target) const {
                EEPROM.put(offset, target.r);
                offset += sizeof(int8_t);

                EEPROM.get(offset, target.g);
                offset += sizeof(int8_t);

                EEPROM.get(offset, target.b);
                offset += sizeof(int8_t);
            }
    };

    //configuration for RTC timesource
    class RtcTimeSourceConfiguration {
        private:
            static const unsigned long SyncIntervalDefault = 60000L;

        public:
            //sync internal timer with timesource (millis)
            unsigned long SyncInterval;

        public:
            void load(int &offset) {
                EEPROM.get(offset, SyncInterval);
                offset += sizeof(unsigned long);
            }

            void save(int &offset) {
                EEPROM.put(offset, SyncInterval);
                offset += sizeof(unsigned long);
            }

            void loadDefaults(int &offset) {
                SyncInterval = SyncIntervalDefault;
                save(offset);
            }

            size_t size() const {
                return sizeof(unsigned long);
            }
    };

    //config for NTP timesource
    class NtpTimeSourceConfiguration {
        private:
            static const char* NTPServerNameDefault;
            static const char* SSIDDefault;
            static const char* PasswordDefault;
            static const unsigned long TimeZoneDefault = 4;
            static const unsigned long SyncIntervalDefault = 60000L;

            static const uint8_t MaxStringLength = 50;

        public:
            char* NTPServerName;
            char* SSID;
            char* Password;
            unsigned long TimeZone;

            //sync internal timer with timesource (millis)
            unsigned long SyncInterval;

        public:
            NtpTimeSourceConfiguration()
                : NTPServerName(nullptr), SSID(nullptr), Password(nullptr) {
            }

            void load(int &offset) {
                NTPServerName = loadString(offset);
                SSID = loadString(offset);
                Password = loadString(offset);
                
                EEPROM.get(offset, TimeZone);
                offset += sizeof(unsigned long);

                EEPROM.get(offset, SyncInterval);
                offset += sizeof(unsigned long);
            }

            void save(int &offset) {
                saveString(offset, NTPServerName);
                saveString(offset, SSID);
                saveString(offset, Password);

                EEPROM.put(offset, TimeZone);
                offset += sizeof(unsigned long);

                EEPROM.put(offset, SyncInterval);
                offset += sizeof(unsigned long);
            }

            void loadDefaults(int &offset) {
                if(NTPServerName)
                    delete[] NTPServerName;

                if(SSID)
                    delete[] SSID;

                if(Password)
                    delete[] Password;

                NTPServerName = new char[strlen(NTPServerNameDefault)];
                strcpy(NTPServerName, NTPServerNameDefault);

                SSID = new char[strlen(SSIDDefault)];
                strcpy(SSID, SSIDDefault);

                Password = new char[strlen(PasswordDefault)];
                strcpy(Password, PasswordDefault);

                TimeZone = TimeZoneDefault;
                SyncInterval = SyncIntervalDefault;

                save(offset);
            }

            size_t size() const {
                return MaxStringLength * (char) 
                    + MaxStringLength * (char) 
                    + MaxStringLength * (char) 
                    + sizeof(unsigned long) 
                    + sizeof(unsigned long);
            }

        private:
            char* loadString(int &offset) {
                uint8_t strLen = 0;
                EEPROM.get(offset, strLen);
                offset += sizeof(uint8_t);

                char* result = new char[strLen];
                for(int i = 0; i < strLen; i++) {
                    result[i] = EEPROM.read(offset);
                    offset += sizeof(char);
                }

                return result;
            }

             void saveString(int &offset, const char* str) {                
                uint8_t strLen = strlen(str);

                EEPROM.get(offset, strLen);
                offset += sizeof(uint8_t);
                
                for(int i = 0; i < strLen; i++) {
                    EEPROM.write(offset, str[i]);
                    offset += sizeof(char);
                }
            }
    };
        
    //global configuration
    class SystemConfiguration {
        private:
            static const int TickTimeDefault = 20;

        public:
            //delay (millis) between update iteration
            int TickTime;

        public:
            void load(int &offset) {
                EEPROM.get(offset, TickTime);
                offset += sizeof(int);
            }

            void save(int &offset) {
                EEPROM.put(offset, TickTime);
                offset += sizeof(int);
            }

            void loadDefaults(int &offset) {
                TickTime = TickTimeDefault;
                save(offset);
            }

            size_t size() const {
                return sizeof(int);
            }
    };

    private:
        ControlConfiguration* _controlConfiguration;
        ClockFaceConfiguration* _clockFaceConfiguration;
        RtcTimeSourceConfiguration* _rtcTimeSourceConfiguration;
        NtpTimeSourceConfiguration* _ntpTimeSourceConfiguration;
        SystemConfiguration* _systemConfiguration;
        RemoteControlConfiguration* _remoteControlConfiguration;

    public:
        ControlConfiguration* getControlConfiguration() {
            return _controlConfiguration;
        }

        ClockFaceConfiguration* getClockFaceConfiguration() {
            return _clockFaceConfiguration;
        }

        RtcTimeSourceConfiguration* getRtcTimeSourceConfiguration() {
            return _rtcTimeSourceConfiguration;
        }

        NtpTimeSourceConfiguration* getNtpTimeSourceConfiguration() {
            return _ntpTimeSourceConfiguration;
        }

        SystemConfiguration* getSystemConfiguration() {
            return _systemConfiguration;
        }

        RemoteControlConfiguration* getRemoteControlConfiguration() {
            return _remoteControlConfiguration;
        }

        void init() {
            size_t totalSize = _controlConfiguration->size()
                + _clockFaceConfiguration->size()
                + _rtcTimeSourceConfiguration->size()
                + _ntpTimeSourceConfiguration->size()
                + _systemConfiguration->size()
                + _remoteControlConfiguration->size();

            EEPROM.begin(totalSize);
        }

        void load() {
            int offset = 0;
            _controlConfiguration->load(offset);
            _clockFaceConfiguration->load(offset);
            _rtcTimeSourceConfiguration->load(offset);
            _ntpTimeSourceConfiguration->load(offset);
            _systemConfiguration->load(offset);
            _remoteControlConfiguration->load(offset);
        }

        void save() {
            int offset = 0;
            _controlConfiguration->save(offset);
            _clockFaceConfiguration->save(offset);
            _rtcTimeSourceConfiguration->save(offset);
            _ntpTimeSourceConfiguration->save(offset);
            _systemConfiguration->save(offset);
            _remoteControlConfiguration->save(offset);
        }

        void loadDefaults() {
            int offset = 0;
            _controlConfiguration->loadDefaults(offset);
            _clockFaceConfiguration->loadDefaults(offset);
            _rtcTimeSourceConfiguration->loadDefaults(offset);
            _ntpTimeSourceConfiguration->loadDefaults(offset);
            _systemConfiguration->loadDefaults(offset);
            _remoteControlConfiguration->loadDefaults(offset);
        }
};

const CRGB Configuration::ClockFaceConfiguration::ColorHourDefault              = CRGB::Red;
const CRGB Configuration::ClockFaceConfiguration::ColorMinuteDefault            = CRGB::Green;
const CRGB Configuration::ClockFaceConfiguration::ColorSecondDefault            = CRGB::Blue;
const CRGB Configuration::ClockFaceConfiguration::ColorHourMinuteDefault        = CRGB::Yellow;
const CRGB Configuration::ClockFaceConfiguration::ColorHourSecondDefault        = CRGB::Magenta;
const CRGB Configuration::ClockFaceConfiguration::ColorMinuteSecondDefault      = CRGB::Cyan;
const CRGB Configuration::ClockFaceConfiguration::ColorAllDefault               = CRGB::White;

const char* Configuration::NtpTimeSourceConfiguration::NTPServerNameDefault = "ntp1.stratum2.ru";
const char* Configuration::NtpTimeSourceConfiguration::SSIDDefault = "";
const char* Configuration::NtpTimeSourceConfiguration::PasswordDefault = "";