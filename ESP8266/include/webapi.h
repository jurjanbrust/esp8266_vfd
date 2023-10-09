#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include "vfd.h"
#include "timedisplay.h"

class WEBAPI
{
  public:
    WEBAPI(VFD& vfd);
    void start();
    void update();

  private:
    VFD* _vfd;
    TIMEDISPLAY* _time;
    WiFiClientSecure httpsClient;
    String getResponse();
    String _json;
    String _feed;
    String _host;
    boolean _forecast;

    enum DisplayMode {
      Normal = 0,
      FadeOut = 1,
      FadeIn = 2,
      Scroll = 3,
      KnightRider = 4,
      KnightRider2 = 5,
      KnightRider3 = 6,
      VerticalScroll = 7,
      HorizontalScroll = 8,
      ClearScreen = 9,
      SetBrightness0 = 10,
      SetBrightness1 = 11,
      SetBrightness2 = 12,
      SetBrightness3 = 13,
      SetBrightness4 = 14,
      SetBrightness5 = 15,
      SetBrightness6 = 16,
      SetBrightness7 = 17,
      FadeLeftToRight = 18,
      FadeRightToLeft = 19
    };
};
