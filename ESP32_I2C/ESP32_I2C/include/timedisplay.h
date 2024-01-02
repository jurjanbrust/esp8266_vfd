#pragma once

#include "display.h"
#include <NTPClient.h>
#include <WiFiManager.h>

class TIMEDISPLAY {
  private:
    Display* _display;

  public:
    TIMEDISPLAY(Display& display);
    void start();
    int isSummerTime(int day, int month, int dayofweek);
    String getDayAsText(int day, const String& time);
    String getFormattedTime(unsigned long rawTime);
};