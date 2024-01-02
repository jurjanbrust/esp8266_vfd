#include "timedisplay.h"
#include "vfd.h"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "nl.pool.ntp.org");
const String space = " ";
const String monthNames[] = {"Januari", "Februari", "Maart", "April", "Mei", "Juni", "Juli", "Augustus", "September", "Oktober", "November", "December"};

TIMEDISPLAY::TIMEDISPLAY(VFD& vfd)
{
    this->_vfd = &vfd;
    timeClient.begin();
    this->_vfd->clear();
    this->_vfd->command(vfd_cursorOff);
}

int is_leap_year(int year) {
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
        return 1;
    return 0;
}

int last_sunday_of_month(int year, int month) {
    int days_in_month[] = {0, 31, 28 + is_leap_year(year), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int day = days_in_month[month];

    while (day > 0) {
        if (day % 7 == 0) {
            return day;
        }
        day--;
    }

    return -1; // Indicates an error (should not happen for valid inputs)
}

// De zomertijd loopt van de laatste zondag van maart tot de laatste zondag van oktober
int TIMEDISPLAY::isSummerTime(int day, int month, int dayOfWeek, int year)
{
    int lastSundayOfMarch = last_sunday_of_month(year, 3);
    int lastSundayOfOctober = last_sunday_of_month(year, 10);
    if (month < 3 || month > 10 || (month == 3 && day <= lastSundayOfMarch) || (month == 10 && day > lastSundayOfOctober))
    {
        return 1; // winter time
    }

    return 2; // summer time
}

void TIMEDISPLAY::start()
{
    this->_vfd->clear();
    
    for (int i = 0; i < 200; i++)
    {
        timeClient.update();
        this->_vfd->home();
        
        String time = timeClient.getFormattedTime();
        time_t rawtime = timeClient.getEpochTime();
        struct tm* ti = localtime(&rawtime);

        int dayOfweek = timeClient.getDay();
        uint16_t year = ti->tm_year + 1900;
        uint8_t month = ti->tm_mon + 1;

        String yearStr = String(year);
        String monthStr = month < 10 ? "0" + String(month) : String(month);
        timeClient.setTimeOffset(3600 * isSummerTime(ti->tm_mday, month, dayOfweek, year));

        monthStr = monthNames[month - 1];
        String dayStr = ti->tm_mday < 10 ? "0" + String(ti->tm_mday) : String(ti->tm_mday);
        String dayAsText = getDayAsText(dayOfweek, time);

        this->_vfd->send(dayAsText);
        this->_vfd->home();
        this->_vfd->linefeed();
        this->_vfd->send("  " + dayStr + " " + monthStr + " " + yearStr);
        this->_vfd->command(vfd_cursorOff);
        delay(100); // update 10 times per second
    }
}

String TIMEDISPLAY::getDayAsText(int dayOfweek, const String& time)
{
    String dayAsText;
    switch (dayOfweek)
    {
        case 0:
            dayAsText = space + space + "Zondag" + space + time + space + space + space;
            break;
        case 1:
            dayAsText = space + space + "Maandag" + space + time + space + space;
            break;
        case 2:
            dayAsText = space + space + "Dinsdag" + space + time + space + space;
            break;
        case 3:
            dayAsText = space + "Woensdag" + space + time + space + space;
            break;
        case 4:
            dayAsText = space + "Donderdag" + space + time + space;
            break;
        case 5:
            dayAsText = space + space + "Vrijdag" + space + time + space + space;
            break;
        case 6:
            dayAsText = space + "Zaterdag" + space + time + space + space;
            break;
    }
    return dayAsText;
}
