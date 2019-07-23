#pragma once

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiUdp.h>

#include <timeSource.h>
#include <configuration.hpp>

const uint8_t NtpTimeSource::MonthDays[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

struct DateTime {
  int  Year;
  byte Month;
  byte Day;
  byte Hour;
  byte Minute;
  byte Second;
  byte DayOfWeek;
};

class NtpTimeSource: public TimeSource
{
    private: 
        const uint32_t UnixEpochStart = 2208988800UL;
        static const int NtpPacketSize = 48;
        static const uint8_t MonthDays[];        
        
    private:
        ESP8266WiFiMulti _wifi;                     
        WiFiUDP _udp;                                    
        IPAddress _timeServerIp;                         
        byte _ntpBuffer[NtpPacketSize];                
        unsigned long _lastNtpRequest;
        unsigned long _lastNtpResponse;        
        unsigned long _prevActualTime;
        uint32_t _unixTime;
        DateTime _currentDateTime;

    public:
        NtpTimeSource(): 
            _lastNtpRequest(0), _lastNtpResponse(millis()), _prevActualTime(0), _unixTime(0)
        {
        }

        virtual void init()
        {
            startWiFi();
            startUdp();

            if(!WiFi.hostByName(Configuration::NtpTimeSourceConfiguration::NTPServerName, _timeServerIp)) 
            { 
                Serial.println("DNS lookup failed. Rebooting.");
                Serial.flush();
                ESP.reset();
            }
  
            Serial.print("Time server IP:\t");
            Serial.println(_timeServerIp);
  
            Serial.println("Sending NTP request ...");
            sendNtpRequest();  
        }

        virtual void updateTime()
        {
            unsigned long currentMillis = millis();

            if (currentMillis - _lastNtpRequest > Configuration::NtpTimeSourceConfiguration::SyncInterval) 
            { 
                _lastNtpRequest = currentMillis;
                Serial.println("Sending NTP request ...");
                sendNtpRequest();
            }

            uint32_t time = readNtpResponse();
            if (time)
            {
                _unixTime = time;
                Serial.print("NTP response:\t");
                Serial.println(_unixTime);
                _lastNtpResponse = currentMillis;
            } 
            else if ((currentMillis - _lastNtpResponse) > 3600000) 
            {
                Serial.println("More than 1 hour since last NTP response. Rebooting.");
                Serial.flush();
                ESP.reset();
            }

            uint32_t actualTime = _unixTime + (currentMillis - _lastNtpResponse) / 1000;
            if (actualTime != _prevActualTime && _unixTime != 0) 
            { 
                _prevActualTime = actualTime;
                updateCurrentTime(actualTime);
            }
        }

        virtual Time getCurrentTime() {
            return Time(_currentDateTime.Hour, _currentDateTime.Minute, _currentDateTime.Second);
        }

        virtual void adjustTime(int dHour, int dMinute) {      
                  
        }

    private:
        void startWiFi() 
        { 
            _wifi.addAP(Configuration::NtpTimeSourceConfiguration::SSID, Configuration::NtpTimeSourceConfiguration::Password);   

            Serial.println("Connecting");
            byte i = 0;
            while (_wifi.run() != WL_CONNECTED) 
            {
                delay(250);
                Serial.print('.');
            }

            Serial.print("Connected to ");
            Serial.println(WiFi.SSID());             
            Serial.print("IP address:\t");
            Serial.print(WiFi.localIP());
        }

        void startUdp() 
        {
            Serial.println("Starting UDP");
            _udp.begin(123);
            Serial.print("Local port:\t");
            Serial.println(_udp.localPort());
            Serial.println();
        }

        uint32_t readNtpResponse() 
        {
            if (_udp.parsePacket() == 0) 
                return 0;
            
            // read the packet into the buffer
            _udp.read(_ntpBuffer, NtpPacketSize); 
            
            // Combine the 4 timestamp bytes into one 32-bit number
            uint32_t ntpTime = (_ntpBuffer[40] << 24) | (_ntpBuffer[41] << 16) | (_ntpBuffer[42] << 8) | _ntpBuffer[43];
            
            // Convert NTP time to a UNIX timestamp:
            // subtract Jan 1 1970
            uint32_t unixTime = ntpTime - UnixEpochStart;

            return unixTime;
        }

        void sendNtpRequest() 
        {
            // set all bytes in the buffer to 0
            memset(_ntpBuffer, 0, NtpPacketSize); 
            
            // Initialize values needed to form NTP request
            // LI, Version, Mode
            _ntpBuffer[0] = 0b11100011;   
            
            // send a packet requesting a timestamp:
            // NTP requests are to port 123
            _udp.beginPacket(_timeServerIp, 123); 
            _udp.write(_ntpBuffer, NtpPacketSize);
            _udp.endPacket();
        }    

        void updateCurrentTime(uint32_t time) 
        {
            // Correct time zone
            time += (3600 * Configuration::NtpTimeSourceConfiguration::TimeZone);
  
            _currentDateTime.Second = time % 60;
            _currentDateTime.Minute = time / 60 % 60;
            _currentDateTime.Hour   = time / 3600 % 24;

            time  /= 60;  // To minutes
            time  /= 60;  // To hours
            time  /= 24;  // To days

            _currentDateTime.DayOfWeek = ((time + 4) % 7) + 1;

            int year = 0;
            int days = 0;
            while ((unsigned)(days += (isLeapYear(year) ? 366 : 365)) <= time) 
            {
                year++;
            }

            days -= isLeapYear(year) ? 366 : 365;
            time  -= days; // To days in this year, starting at 0  
            days = 0;

            byte month = 0;
            byte monthLength = 0;
            for (month = 0; month < 12; month++) 
            {
                if (month == 1) 
                { 
                    // February
                    if (isLeapYear(year)) 
                        monthLength = 29;
                    else 
                        monthLength = 28;                    
                } 
                else 
                    monthLength = MonthDays[month];
                  
                if (time >= monthLength) 
                    time -= monthLength;
                else 
                    break;                
            }
 
            _currentDateTime.Day = time + 1;
            _currentDateTime.Year = year + 1970;
            _currentDateTime.Month = month + 1;  

            // Correct Summer time
            if (summerTime()) 
                _currentDateTime.Hour += 1;
            
#ifdef DEBUG_ON
            Serial.print(_currentDateTime.Year);
            Serial.print(" ");
            Serial.print(_currentDateTime.Month);
            Serial.print(" ");
            Serial.print(_currentDateTime.Day);
            Serial.print(" ");
            Serial.print(_currentDateTime.Hour);
            Serial.print(" ");
            Serial.print(_currentDateTime.Minute);
            Serial.print(" ");
            Serial.print(_currentDateTime.Second);
            Serial.print(" day of week: ");
            Serial.print(_currentDateTime.DayOfWeek);
            Serial.print(" summer time: ");
            Serial.print(summerTime());
            Serial.print(" night time: ");
            Serial.print(night());  
            Serial.println();
#endif  
        }

        bool summerTime() 
        {
            // No summer time in Jan, Feb, Nov, Dec
            if (_currentDateTime.Month < 3 || _currentDateTime.Month > 10) 
                return false;  

            // Summer time in Apr, May, Jun, Jul, Aug, Sep
            if (_currentDateTime.Month > 3 && _currentDateTime.Month < 10) 
                return true;

            if (_currentDateTime.Month == 3 && (_currentDateTime.Hour + 24 * _currentDateTime.Day) >= (3 +  24 * (31 - (5 * _currentDateTime.Year / 4 + 4) % 7)) 
                || _currentDateTime.Month == 10 && (_currentDateTime.Hour + 24 * _currentDateTime.Day) < (3 +  24 * (31 - (5 * _currentDateTime.Year / 4 + 1) % 7)))
                return true;
            else
                return false;
        }
        
        bool isLeapYear(int year)
        {
            return (((1970 + year) > 0) && !((1970 + year) % 4) && (((1970 + year) % 100) || !((1970 + year) % 400)));
        }
};
#endif