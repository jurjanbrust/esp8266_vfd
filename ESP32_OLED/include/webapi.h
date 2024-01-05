#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include "display.h"
#include "timedisplay.h"
#include "WiFiClientSecure.h"

class WEBAPI
{
  public:
    WEBAPI(Display& display);
    void start();
    void update();

  private:
    Display* _display;
    TIMEDISPLAY* _time;
    WiFiClientSecure httpsClient;
    String getResponse();
    String _json;
    String _feed;
    String _host;
    boolean _forecast;
};