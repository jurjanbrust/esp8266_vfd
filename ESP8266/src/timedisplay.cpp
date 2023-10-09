#include "timedisplay.h"
#include "vfd.h"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "nl.pool.ntp.org", 3600*2, 600000);
String space = " ";
const String monthNames[] = {"Januari", "Februari", "Maart", "April", "Mei", "Juni", "Juli", "Augustus", "September", "Oktober", "November", "December"};

TIMEDISPLAY::TIMEDISPLAY(VFD& vfd)
{
    this->_vfd = &vfd;
    timeClient.begin();
    this->_vfd->clear();
    this->_vfd->command(vfd_cursorOff);
}

int TIMEDISPLAY::isSummerTime(int day, int month, int dayOfWeek)
{
    // January to March and October to December are always standard time (winter)
    if (month < 4 || (month == 10 && dayOfWeek >= 4))
        return 1; // Standard time

    // April to September are always daylight saving time (summer)
    if (month > 9 || (month == 9 && dayOfWeek < 4))
        return 2; // Daylight saving time

    // From the fourth Sunday in March to the last Sunday in October, it's daylight saving time
    if ((month == 3 && dayOfWeek == 0 && day >= 25) || (month > 3 && month < 10) || (month == 10 && dayOfWeek == 0 && day < 25))
        return 2; // Daylight saving time

    return 1; // Standard time
}

void TIMEDISPLAY::start()
{
    this->_vfd->clear();
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
        
        int multiplier = isSummerTime(ti->tm_mday, month, day);
        timeClient.setTimeOffset(3600*multiplier);

        monthStr = monthNames[month - 1];
        String dayStr = ti->tm_mday < 10 ? "0" + String(ti->tm_mday) : String(ti->tm_mday);

        String dayAsText;
        switch(day) {
            case 0 :
                dayAsText = space + space + "Zondag" + space + time + space + space + space;
                break;
            case 1 :
                dayAsText = space + space + "Maandag" + space + time + space + space;
                break;
            case 2 :
                dayAsText = space + space + "Dinsdag" + space + time + space + space;
                break;
            case 3 :
                dayAsText = space + "Woensdag" + space + time + space + space;
                break;
            case 4 :
                dayAsText = space + "Donderdag" + space + time + space;
                break;
            case 5 :
                dayAsText = space + space + "Vrijdag" + space + time + space + space;
                break;
            case 6 :
                dayAsText = space + "Zaterdag" + space + time + space + space;
                break;
        }
        this->_vfd->send(dayAsText);
        this->_vfd->home();
        this->_vfd->linefeed();
        this->_vfd->send("  " + dayStr + " " + monthStr + " " + yearStr);
        this->_vfd->command(vfd_cursorOff);
        delay(100); // update 10 times per second
    }
}