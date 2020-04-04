#include "time.h"
#include "vfd.h"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "nl.pool.ntp.org", 3600*2, 600000);
String space = " ";

TIMEDISPLAY::TIMEDISPLAY(VFD& vfd)
{
    this->_vfd = &vfd;
    timeClient.begin();
    timeClient.update();
    this->_vfd->clear();
    this->_vfd->command(vfd_cursorOff);
}

void TIMEDISPLAY::start()
{
    for(int i=0; i<200; i++)
    {
        timeClient.update();
        this->_vfd->home();
        String time = timeClient.getFormattedTime();

        time_t rawtime = timeClient.getEpochTime();
        struct tm * ti;
        ti = localtime (&rawtime);

        int day = timeClient.getDay();

        uint16_t year = ti->tm_year + 1900;
        String yearStr = String(year);

        uint8_t month = ti->tm_mon + 1;
        String monthStr = month < 10 ? "0" + String(month) : String(month);
        
        switch(month) {
            case 1 :
                monthStr = "Januari";
                break;
            case 2 :
                monthStr = "Februari";
                break;
            case 3 :
                monthStr = "Maart";
                break;
            case 4 :
                monthStr = "April";
                break;
            case 5 :
                monthStr = "Mei";
                break;
            case 6 :
                monthStr = "Juni";
                break;
            case 7 :
                monthStr = "Juli";
                break;
            case 8 :
                monthStr = "Augustus";
                break;
            case 9 :
                monthStr = "September";
                break;
            case 10 :
                monthStr = "Oktober";
                break;
            case 11 :
                monthStr = "November";
                break;
            case 12 :
                monthStr = "December";
                break;
        }

        String dayStr = ti->tm_mday < 10 ? "0" + String(ti->tm_mday) : String(ti->tm_mday);

        String dayAsText;
        switch(day) {
            case 0 : // 6
                dayAsText = space + space + "Zondag" + space + time + space + space + space;
                break;
            case 1 : // 7
                dayAsText = space + space + "Maandag" + space + time + space + space;
                break;
            case 2 : // 7
                dayAsText = space + space + "Dinsdag" + space + time + space + space;
                break;
            case 3 : // 8
                dayAsText = space + "Woensdag" + space + time + space + space;
                break;
            case 4 : // 9
                dayAsText = space + "Donderdag" + space + time + space;
                break;
            case 5 : // 7
                dayAsText = space + space + "Vrijdag" + space + time + space + space;
                break;
            case 6 : // 8
                dayAsText = space + "Zaterdag" + space + time + space + space;
                break;
        }
        this->_vfd->fixed(dayAsText);
        this->_vfd->centered(dayStr + " " + monthStr + " " + yearStr);
        this->_vfd->command(vfd_cursorOff);
        delay(100);
    }
}