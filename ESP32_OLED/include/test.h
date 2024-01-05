#pragma once

#include "display.h"
#include <Arduino.h>
#include <WiFiManager.h>

class TESTDISPLAY {
  private:
    Display* _display;

  public:
    TESTDISPLAY(Display& display);
    void write();
};
