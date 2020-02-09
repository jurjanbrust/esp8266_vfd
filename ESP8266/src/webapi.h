#ifndef WebApi_h
#define WebApi_h

#include "Arduino.h"
#include <ESP8266WiFi.h>
#include "vfd.h"
#include "time.h"
#include <ArduinoJson.h>

class Webserver
{
  public:
    Webserver(VFD& vfd);
    void update();
    void start();
    private:
    VFD* _vfd;
    TIMEDISPLAY* _time;
    String getResponse();
    String _json;
    String _feed;
    String _host;
    boolean _forecast;
};

#endif
