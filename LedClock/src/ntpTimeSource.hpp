#pragma once

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiUdp.h>

#include <timeSource.h>
#include <configuration.hpp>

const uint8_t NtpTimeSource::monthDays[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
const int NtpTimeSource::NTP_PACKET_SIZE = 48;

struct DateTime {
  int  year;
  byte month;
  byte day;
  byte hour;
  byte minute;
  byte second;
  byte dayofweek;
};

DateTime currentDateTime;

class NtpTimeSource: public TimeSource
{
    private: 
        static const uint8_t monthDays[];
        static const int NTP_PACKET_SIZE;      
        
    private:
        ESP8266WiFiMulti _wifi;                     
        WiFiUDP _udp;                                    
        IPAddress _timeServerIp;                         
        byte _ntpBuffer[NTP_PACKET_SIZE];                
        unsigned long _prevNTP = 0;
        unsigned long _lastNTPResponse = millis();
        uint32_t _timeUNIX = 0;
        unsigned long _prevActualTime = 0;

    public:
        NtpTimeSource(): 
            _prevNTP(0), _lastNTPResponse(millis()), _timeUNIX(0), _prevActualTime(0)
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
            sendNTPpacket(_timeServerIp);  
        }

        virtual void update()
        {
            unsigned long currentMillis = millis();

            if (currentMillis - _prevNTP > Configuration::TimeSourceConfiguration::SyncInterval) 
            { 
                // If a minute has passed since last NTP request
                _prevNTP = currentMillis;
                Serial.println("\r\nSending NTP request ...");
                sendNTPpacket(_timeServerIp);               // Send an NTP request
            }

            uint32_t time = getTime();                   // Check if an NTP response has arrived and get the (UNIX) time
            if (time)
            {
                // If a new timestamp has been received
                _timeUNIX = time;
                Serial.print("NTP response:\t");
                Serial.println(_timeUNIX);
                _lastNTPResponse = currentMillis;
            } 
            else if ((currentMillis - _lastNTPResponse) > 3600000) 
            {
                Serial.println("More than 1 hour since last NTP response. Rebooting.");
                Serial.flush();
                ESP.reset();
            }

            uint32_t actualTime = _timeUNIX + (currentMillis - _lastNTPResponse)/1000;
            if (actualTime != _prevActualTime && _timeUNIX != 0) 
            { 
                // If a second has passed since last update
                _prevActualTime = actualTime;
                convertTime(actualTime);
            }
        }

    private:
        void startWiFi() 
        { 
            _wifi.addAP(Configuration::NtpTimeSourceConfiguration::ssid, Configuration::NtpTimeSourceConfiguration::pass);   

            Serial.println("Connecting");
            byte i = 0;
            while (_wifi.run() != WL_CONNECTED) 
            {
                delay(250);
                Serial.print('.');                
            }

            Serial.println("\r\n");
            Serial.print("Connected to ");
            Serial.println(WiFi.SSID());             
            Serial.print("IP address:\t");
            Serial.print(WiFi.localIP());            
            Serial.println("\r\n");
        }

        void startUdp() 
        {
            Serial.println("Starting UDP");
            _udp.begin(123);                          // Start listening for UDP messages on port 123
            Serial.print("Local port:\t");
            Serial.println(_udp.localPort());
            Serial.println();
        }

        uint32_t getTime() 
        {
            if (_udp.parsePacket() == 0) 
            { // If there's no response (yet)
                return 0;
            }
            _udp.read(_ntpBuffer, NTP_PACKET_SIZE); // read the packet into the buffer
            // Combine the 4 timestamp bytes into one 32-bit number
            uint32_t NTPTime = (_ntpBuffer[40] << 24) | (_ntpBuffer[41] << 16) | (_ntpBuffer[42] << 8) | _ntpBuffer[43];
            // Convert NTP time to a UNIX timestamp:
            // Unix time starts on Jan 1 1970. That's 2208988800 seconds in NTP time:
            const uint32_t seventyYears = 2208988800UL;
            // subtract seventy years:
            uint32_t UNIXTime = NTPTime - seventyYears;
            return UNIXTime;
        }

        void sendNTPpacket(IPAddress& address) 
        {
            memset(_ntpBuffer, 0, NTP_PACKET_SIZE);  // set all bytes in the buffer to 0
            // Initialize values needed to form NTP request
            _ntpBuffer[0] = 0b11100011;   // LI, Version, Mode
            // send a packet requesting a timestamp:
            _udp.beginPacket(address, 123); // NTP requests are to port 123
            _udp.write(_ntpBuffer, NTP_PACKET_SIZE);
            _udp.endPacket();
        }    

        void convertTime(uint32_t time) 
        {
            // Correct time zone
            time += (3600 * Configuration::NtpTimeSourceConfiguration::timeZone);
  
            currentDateTime.second = time % 60;
            currentDateTime.minute = time / 60 % 60;
            currentDateTime.hour   = time / 3600 % 24;
            time  /= 60;  // To minutes
            time  /= 60;  // To hours
            time  /= 24;  // To days
            currentDateTime.dayofweek = ((time + 4) % 7) + 1;
            int year = 0;
            int days = 0;
            while ((unsigned)(days += (LEAP_YEAR(year) ? 366 : 365)) <= time) 
            {
                year++;
            }
            days -= LEAP_YEAR(year) ? 366 : 365;
            time  -= days; // To days in this year, starting at 0  
            days = 0;
            byte month = 0;
            byte monthLength = 0;
            for (month = 0; month < 12; month++) 
            {
                if (month == 1) 
                { // February
                    if (LEAP_YEAR(year)) 
                    {
                        monthLength = 29;
                    } 
                    else 
                    {
                        monthLength = 28;
                    }
                } 
                else 
                {
                    monthLength = monthDays[month];
                }
  
                if (time >= monthLength) 
                {
                    time -= monthLength;
                } 
                else 
                {
                    break;
                }
            }
 
            currentDateTime.day = time + 1;
            currentDateTime.year = year + 1970;
            currentDateTime.month = month + 1;  

            // Correct European Summer time
            if (summerTime()) 
            {
                currentDateTime.hour += 1;
            }

#ifdef DEBUG_ON
            Serial.print(currentDateTime.year);
            Serial.print(" ");
            Serial.print(currentDateTime.month);
            Serial.print(" ");
            Serial.print(currentDateTime.day);
            Serial.print(" ");
            Serial.print(currentDateTime.hour);
            Serial.print(" ");
            Serial.print(currentDateTime.minute);
            Serial.print(" ");
            Serial.print(currentDateTime.second);
            Serial.print(" day of week: ");
            Serial.print(currentDateTime.dayofweek);
            Serial.print(" summer time: ");
            Serial.print(summerTime());
            Serial.print(" night time: ");
            Serial.print(night());  
            Serial.println();
#endif  
        }

        bool summerTime() 
        {
            if (currentDateTime.month < 3 || currentDateTime.month > 10) return false;  // No summer time in Jan, Feb, Nov, Dec
            if (currentDateTime.month > 3 && currentDateTime.month < 10) return true;   // Summer time in Apr, May, Jun, Jul, Aug, Sep
            if (currentDateTime.month == 3 && (currentDateTime.hour + 24 * currentDateTime.day) >= (3 +  24 * (31 - (5 * currentDateTime.year / 4 + 4) % 7)) || currentDateTime.month == 10 && (currentDateTime.hour + 24 * currentDateTime.day) < (3 +  24 * (31 - (5 * currentDateTime.year / 4 + 1) % 7)))
                return true;
            else
                return false;
        }
        
        bool LEAP_YEAR(int year)
        {
            return (((1970 + year) > 0) && !((1970 + year) % 4) && (((1970 + year) % 100) || !((1970 + year) % 400)));
        }
};