#pragma once

#include "vfd.h"
#include <NTPClient.h>
#include <WiFiManager.h>

class TIMEDISPLAY {
  private:
    VFD* _vfd;

  public:
    TIMEDISPLAY(VFD& vfd);
    void start();
    int isSummerTime(int day, int month, int dayofweek);
};