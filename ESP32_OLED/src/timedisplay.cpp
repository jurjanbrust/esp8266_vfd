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
    
    for (int i = 0; i < 500; i++)
    {
        timeClient.update();
        
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

        String time = timeClient.getFormattedTime();
        String dayAsText = getDayAsText(day, time);

        this->_display->home();
        this->_display->send("");
        this->_display->send("");
        this->_display->send(dayAsText);
        this->_display->send("");
        this->_display->send("   " + dayStr + " " + monthStr + " " + yearStr);
        delay(100);
    }
}

String TIMEDISPLAY::getDayAsText(int day, const String& time)
{
    String dayAsText;
    switch (day)
    {
        case 0:
            dayAsText = space + space + "Zondag" + space + time;
            break;
        case 1:
            dayAsText = space + space + "Maandag" + space + time;
            break;
        case 2:
            dayAsText = space + space + "Dinsdag" + space + time;
            break;
        case 3:
            dayAsText = space + space + "Woensdag" + space + time;
            break;
        case 4:
            dayAsText = space + space + "Donderdag" + space + time;
            break;
        case 5:
            dayAsText = space + space + "Vrijdag" + space + time;
            break;
        case 6:
            dayAsText = space + space + "Zaterdag" + space + time;
            break;
    }
    return dayAsText;
}
