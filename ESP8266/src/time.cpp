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
        int day = timeClient.getDay();

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
        this->_vfd->send(dayAsText);
        this->_vfd->command(vfd_cursorOff);
        delay(100);
    }
}