#include <Arduino.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include "vfd.h"
#include "test.h"
#include "timedisplay.h"
#include "webapi.h"

VFD vfd;
TESTDISPLAY test(vfd);
TIMEDISPLAY tijd(vfd);
WEBAPI api(vfd);

//#define TEST
void loop() {

    #ifdef TEST
        test.start();
    #endif

    #ifndef TEST
    for(int i=0; i<=5; i++) {      // update API every n itterations
        if(i==0) {
            api.update();
        }
        api.start();
        tijd.start();
    }
    #endif
}

void configModeCallback (WiFiManager *myWiFiManager) {
  vfd.typeWriteHorizontal("Entered config mode");
  vfd.typeWriteHorizontal(myWiFiManager->getConfigPortalSSID());
}

void setup() {
    Serial.begin(19200);
    vfd.clear();
    vfd.command(vfd_cursorOff);
    WiFiManager wifiManager;
    wifiManager.setDebugOutput(false);
    wifiManager.setAPCallback(configModeCallback);
    wifiManager.autoConnect("AutoConnectAP");

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        vfd.typeWriteHorizontal(".");
    }

    if(WiFi.isConnected()) {
        vfd.clear();
        vfd.typeWriteHorizontal("Connected to WiFi");
        vfd.enter();
        vfd.typeWriteHorizontal(WiFi.localIP().toString());
    }
}
