#pragma once

#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "vfd.h"

class WEBSERVER {
  private:
    VFD* _vfd;
    AsyncWebServer* _server;
    void notFound(AsyncWebServerRequest *request);
  public:
    WEBSERVER(VFD& vfd, AsyncWebServer& server);
    void setupWebServer();
    void start();
    void scrollText(String message);
};
