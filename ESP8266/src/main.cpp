#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsyncWiFiManager.h>
#include "vfd.h"
#include "test.h"
#include "timedisplay.h"
#include "webapi.h"
#include "webserver.h"

VFD vfd;
TESTDISPLAY test(vfd);
TIMEDISPLAY tijd(vfd);
WEBAPI api(vfd);
AsyncWebServer server(80);
WEBSERVER web(vfd, server);
DNSServer dns;

//#define TEST
void loop() {

    #ifdef TEST
        // nothing
    #endif

    #ifndef TEST
    for(int i=0; i<=10; i++) {
        if(i==0) {
            api.update();
        }
        api.start();
        tijd.start();
    }
    #endif
}

void configModeCallback (AsyncWiFiManager *myWiFiManager) {
  vfd.typeWriteHorizontal("Entered config mode");
  vfd.typeWriteHorizontal(myWiFiManager->getConfigPortalSSID());
}

void setup() {
    // setup serial and clear screen
    Serial.begin(19200);
    vfd.clear();
    vfd.command(vfd_cursorOff);

    // setup wifi
    AsyncWiFiManager wifiManager(&server,&dns);
    wifiManager.setAPCallback(configModeCallback);
    wifiManager.autoConnect("AutoConnectAP VFD");
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(100);
        vfd.typeWriteHorizontal(".");
    }

    if(WiFi.isConnected()) {
        vfd.clear();
        vfd.typeWriteHorizontal("Connected to WiFi");
        vfd.enter();
        vfd.typeWriteHorizontal(WiFi.localIP().toString());
  
        // setup webserver
        //web.setupWebServer();
        server.on("/api", HTTP_GET, [] (AsyncWebServerRequest *request) {
            String message;
            if(request->hasArg("text")) {
                message = request->arg("text");
            } else {
                message = "No message sent";
            }
            if (request->arg("command") == "typeWriteVertical") {
                vfd.typeWriteVertical(message);
            }
            if (request->arg("command") == "send") {
                vfd.send(message);
            }
            if (request->arg("command") == "vfd_bellAlarmOutput") {
                vfd.command(vfd_bellAlarmOutput);
            }        
            if(request->arg("command") == "vfd_backSpaceCursor") {
                vfd.command(vfd_backSpaceCursor);
            }
            if(request->arg("command") == "vfd_advanceCursor") {
                vfd.command(vfd_advanceCursor);
            }
            if(request->arg("command") == "vfd_lineFeed") {
                vfd.command(vfd_lineFeed);
            }
            if(request->arg("command") == "vfd_blinkBlockCursor") {
                vfd.command(vfd_blinkBlockCursor);
            }
            if(request->arg("command") == "vfd_underbarCursor") {
                vfd.command(vfd_underbarCursor);
            }
            if(request->arg("command") == "vfd_carriageReturn") {
                vfd.command(vfd_carriageReturn);
            }
            if(request->arg("command") == "vfd_cursorOff") {
                vfd.command(vfd_cursorOff);
            }
            if(request->arg("command") == "vfd_cursorOn") {
                vfd.command(vfd_cursorOn);
            }
            if(request->arg("command") == "vfd_scrollLineLock") {
                vfd.command(vfd_scrollLineLock);
            }
            if(request->arg("command") == "vfd_setVerticalScrollMode") {
                vfd.command(vfd_setVerticalScrollMode);
            }
            if(request->arg("command") == "vfd_SetHorizontalScrollMode") {
                vfd.command(vfd_SetHorizontalScrollMode);
            }
            if(request->arg("command") == "vfd_softwareReset") {
                vfd.command(vfd_softwareReset);
            }
            if(request->arg("command") == "vfd_clearDisplayAndHomeCursor") {
                vfd.command(vfd_clearDisplayAndHomeCursor);
            }
            if(request->arg("command") == "vfd_homeCursor") {
                vfd.command(vfd_homeCursor);
            }
            if(request->arg("command") == "vfd_setDataBit7High") {
                vfd.command(vfd_setDataBit7High);
            }
            if(request->arg("command") == "vfd_beginUserDefinedCharacter") {
                vfd.command(vfd_beginUserDefinedCharacter);
            }
            if(request->arg("command") == "vfd_setAddressBit0High") {
                vfd.command(vfd_setAddressBit0High);
            }
            if(request->arg("command") == "vfd_cursorUpOneLine") {
                vfd.command(vfd_cursorUpOneLine);
            }
            if(request->arg("command") == "vfd_moveCursorToDesignatedLocation") {
                vfd.command(vfd_moveCursorToDesignatedLocation);
            }
            if(request->arg("command") == "vfd_setEuropeanCharacterSet") {
                vfd.command(vfd_setEuropeanCharacterSet);
            }
            if(request->arg("command") == "vfd_setKatakanaCharacterSet") {
                vfd.command(vfd_setKatakanaCharacterSet);
            }
            if(request->arg("command") == "vfd_setCryllicCharacterSet") {
                vfd.command(vfd_setCryllicCharacterSet);
            }
            if(request->arg("command") == "vfd_setHebrewCharacterSet") {
                vfd.command(vfd_setHebrewCharacterSet);
            }
            if(request->arg("command") == "vfd_setDisplayScreenOrBrightness") {
                vfd.command(vfd_setDisplayScreenOrBrightness);
            }
            if(request->arg("command") == "vfd_beginBlinkingCharacters") {
                vfd.command(vfd_beginBlinkingCharacters);
            }
            if(request->arg("command") == "vfd_endBlinkingCharacters") {
                vfd.command(vfd_endBlinkingCharacters);
            }
            if(request->arg("command") == "vfd_blankDisplayScreen") {
                vfd.command(vfd_blankDisplayScreen);
            }
            if(request->arg("command") == "vfd_unblankDisplayScreen") {
                vfd.command(vfd_unblankDisplayScreen);
            }
            if(request->arg("command") == "vfd_commaPeriodTriangleFunction") {
                vfd.command(vfd_commaPeriodTriangleFunction);
            }
            if(request->arg("command") == "vfd_eraseLineDataWithEndBlink") {
                vfd.command(vfd_eraseLineDataWithEndBlink);
            }
            if(request->arg("command") == "vfd_setCarriageReturnAndLineFeedDefinitions") {
                vfd.command(vfd_setCarriageReturnAndLineFeedDefinitions);
            }
            if(request->arg("command") == "vfd_underbarOn") {
                vfd.command(vfd_underbarOn);
            }
            if(request->arg("command") == "vfd_underbarOff") {
                vfd.command(vfd_underbarOff);
            }
            if(request->arg("command") == "vfd_selectRightToLeftDataEntry") {
                vfd.command(vfd_selectRightToLeftDataEntry);
            }
            if(request->arg("command") == "vfd_selectLeftToRightDataEntry") {
                vfd.command(vfd_selectLeftToRightDataEntry);
            }
            if(request->arg("command") == "vfd_screenSaverOn") {
                vfd.command(vfd_screenSaverOn);
            }
            if(request->arg("command") == "vfd_screenSaverOff") {
                vfd.command(vfd_screenSaverOff);
            }
            if(request->arg("command") == "vfd_executeSelfTest") {
                vfd.command(vfd_executeSelfTest);
            }
            if(request->arg("command") == "vfd_terminateSelfTest") {
                vfd.command(vfd_terminateSelfTest);
            }
            if(request->arg("command") == "knightRider") {
                vfd.knightRider();
            }
            if(request->arg("command") == "knightRider2") {
                vfd.knightRider2();
            }
            if(request->arg("command") == "knightRider3") {
                vfd.knightRider3();
            }
            if(request->arg("command") == "L2R") {
                vfd.L2R();
            }
            if(request->arg("command") == "R2L") {
                vfd.R2L();
            }
            if(request->arg("command") == "fadeIn") {
                vfd.fadeIn();
            }
            if(request->arg("command") == "fadeOut") {
                vfd.fadeOut();
            }
            request->send(200, "text/plain", message);
        });

        server.onNotFound( [] (AsyncWebServerRequest *request) {
            request->send(404, "text/plain", "Not found");
        });

        server.begin();      
    }
}
