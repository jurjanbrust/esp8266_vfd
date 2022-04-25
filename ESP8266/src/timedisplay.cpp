#include "timedisplay.h"
#include "vfd.h"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "nl.pool.ntp.org", 3600*2, 600000);
String space = " ";

TIMEDISPLAY::TIMEDISPLAY(VFD& vfd)
{
    this->_vfd = &vfd;
    timeClient.begin();
    this->_vfd->clear();
    this->_vfd->command(vfd_cursorOff);
}

int TIMEDISPLAY::isSummerTime(int day, int month, int dayofweek)
{
  if ((month >=4) &&( month <= 9)) 
    return 2;  // april-september is altijd zomertijd
  if ((month <=2) || (month >= 10))
    return 1; // jan-feb, nov-dec altijd winter
    
    // day minus dayofweek is datum van vorige zondag.
    // Dat is de laatste v/d maand als dat >= 25
    // (day-dayofweek is negatief als vorige zondag in vorige maand viel.
  if ((day-dayofweek) >= 25) {
    if (month == 3)
      return 2;  // Maart, zomertijd is ingegeaan
    else
      return 1;  // October, zomertijd is voorbij
  } else {
    if (month == 3)
      return 1;  // Maart, zomertijd is nog niet ingegeaan
    else
      return 2;  // October, zomertijd is nog niet voorbij
  }
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
        
        int multiplier = isSummerTime(ti->tm_mday, month, day);
        timeClient.setTimeOffset(3600*multiplier);

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
        delay(100);
    }
}