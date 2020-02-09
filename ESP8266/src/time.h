#ifndef TIME_h
#define TIME_h

#include "vfd.h"
#include <NTPClient.h>
#include <WiFiManager.h>

class TIMEDISPLAY {
  private:
    VFD* _vfd;

  public:
    TIMEDISPLAY(VFD& vfd);
    void start();
};

#endif
