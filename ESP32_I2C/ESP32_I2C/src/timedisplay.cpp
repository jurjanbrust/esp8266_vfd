#include "timedisplay.h"
#include "display.h"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "nl.pool.ntp.org", 3600 * 2, 600000);
const String space = " ";
const String monthNames[] = {"Januari", "Februari", "Maart", "April", "Mei", "Juni", "Juli", "Augustus", "September", "Oktober", "November", "December"};

TIMEDISPLAY::TIMEDISPLAY(Display& display)
{
    Serial.println("TimeDisplay started");
    this->_display = &display;
    // timeClient.begin();
    // this->_display->clear();
    // delay(10000);
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
    this->_display->clear();
    
    for (int i = 0; i < 50; i++)
    {
        timeClient.update();
        this->_display->home();
        
        String time = timeClient.getFormattedTime();
        time_t rawtime = timeClient.getEpochTime();
        struct tm* ti = localtime(&rawtime);

        int day = timeClient.getDay();
        uint16_t year = ti->tm_year + 1900;
        uint8_t month = ti->tm_mon + 1;

        String yearStr = String(year);
        String monthStr = month < 10 ? "0" + String(month) : String(month);
        int multiplier = isSummerTime(ti->tm_mday, month, day);
        
        timeClient.setTimeOffset(3600 * multiplier);

        monthStr = monthNames[month - 1];
        String dayStr = ti->tm_mday < 10 ? "0" + String(ti->tm_mday) : String(ti->tm_mday);

        String dayAsText = getDayAsText(day, time);

        this->_display->send(dayAsText);
        this->_display->home();
        this->_display->linefeed();
        this->_display->send(dayStr + " " + monthStr + " " + yearStr);
        delay(400); // update x times per second
    }
}

String TIMEDISPLAY::getDayAsText(int day, const String& time)
{
    String dayAsText;
    switch (day)
    {
        case 0:
            dayAsText = "Zondag" + space + time + space + space + space;
            break;
        case 1:
            dayAsText = "Maandag" + space + time + space + space;
            break;
        case 2:
            dayAsText = "Dinsdag" + space + time + space + space;
            break;
        case 3:
            dayAsText = "Woensdag" + space + time + space + space;
            break;
        case 4:
            dayAsText = "Donderdag" + space + time + space;
            break;
        case 5:
            dayAsText = "Vrijdag" + space + time + space + space;
            break;
        case 6:
            dayAsText = "Zaterdag" + space + time + space + space;
            break;
    }
    return dayAsText;
}
