#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include "vfd.h"
#include "timedisplay.h"

class WEBAPI
{
  enum mode {Normal,
            FadeInOut,
            Scroll,
            KnightRider,
            VerticalScroll,
            HorizontalScroll};

  public:
    WEBAPI(VFD& vfd);
    void start();
    void update();

  private:
    VFD* _vfd;
    TIMEDISPLAY* _time;
    String getResponse();
    String _json;
    String _feed;
    String _host;
    boolean _forecast;
};
