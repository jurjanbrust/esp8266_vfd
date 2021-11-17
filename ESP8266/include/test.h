#pragma once

#include "vfd.h"
#include <Arduino.h>
#include <WiFiManager.h>

class TESTDISPLAY {
  private:
    VFD* _vfd;

  public:
    TESTDISPLAY(VFD& vfd);
    void start();
    void scrollText();
    void knightRider();
};
