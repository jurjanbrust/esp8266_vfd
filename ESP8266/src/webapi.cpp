#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include "webapi.h"
#include "vfd.h"
#include "time.h"
#include "secret.h"
//#define DEBUG

#ifndef DEBUG_PRINT
  #ifdef DEBUG
    #define DEBUG_PRINT(x)  Serial.println(x); delay(1000);
  #else
    #define DEBUG_PRINT(x)
  #endif
#endif

StaticJsonDocument<5000> _doc;

String extractJsonContent(const String &input) {
    int start = input.indexOf('[');
    int end = input.lastIndexOf(']');

    if (start != -1 && end != -1 && start < end) {
        return input.substring(start, end + 1);
    } else {
        return "";
    }
}

WEBAPI::WEBAPI(VFD& vfd) {
    this->_vfd = &vfd;
    _host = azureWebAPI;
    _feed = "/display";
    httpsClient.setInsecure();
    httpsClient.setTimeout(10000); // 10 Seconds
}

void WEBAPI::update() {
  try {
    int retry = 0;
    while ((!httpsClient.connect(_host, 443)) && (retry < 20)) {
      delay(100);
      DEBUG_PRINT(".");
      retry++;
    }

    DEBUG_PRINT("Requesting URL: ");
    DEBUG_PRINT(_host + _feed);
    httpsClient.print(String("GET ") + _feed + " HTTP/1.1\r\n" +
                      "Host: " + _host + "\r\n" +
                      "Connection: close\r\n\r\n");

    DEBUG_PRINT("Headers:");
    while (httpsClient.connected()) {
      _json = httpsClient.readStringUntil('\n');
      DEBUG_PRINT(_json);
      if (_json == "\r") {
        httpsClient.readStringUntil('\n'); // The API sends an extra line with just a number. This breaks the JSON parsing; hence, an extra read
        break;
      }
    }

    DEBUG_PRINT("Body:");
    String extractedJson = extractJsonContent(httpsClient.readString());
    DEBUG_PRINT(extractedJson);
    
    _doc.clear(); // Clears the reserved memory; see solution 2: https://arduinojson.org/v6/issues/memory-leak
    DeserializationError err = deserializeJson(_doc, extractedJson);
    if (err) {
      _vfd->clear();
      _vfd->typeWriteHorizontal("deserializeJson failed: " + String(err.c_str()));
      delay(5000);
    }

    httpsClient.stop();

    DEBUG_PRINT("JSON Document Size: " + _doc.size());

  } catch (std::exception &e) {
    _vfd->clear();
    _vfd->typeWriteHorizontal("Exception: " + String(e.what()));
    delay(5000);
  }
}

void WEBAPI::start() {
  enum mode {
    Normal,          // 0
    FadeOut,         // 1
    FadeIn,          // 2
    Scroll,          // 3
    KnightRider,     // 4
    KnightRider2,    // 5
    KnightRider3,    // 6
    VerticalScroll,  // 7
    HorizontalScroll,// 8
    ClearScreen,     // 9
    SetBrightness0,  // 10
    SetBrightness1,  // 11
    SetBrightness2,  // 12
    SetBrightness3,  // 13
    SetBrightness4,  // 14
    SetBrightness5,  // 15
    SetBrightness6,  // 16
    SetBrightness7,  // 17
    FadeLeftToRight, // 18
    FadeRightToLeft  // 19
  };

  int displaymode = 0;
  String line1;
  String line2;

  DEBUG_PRINT("Display data, size: ");
  DEBUG_PRINT(_doc.size());

  for (unsigned int i = 0; i < _doc.size(); i++) {
    displaymode = _doc[i]["displayMode"].as<int>();
    line1 = _doc[i]["line1"].as<String>();
    line2 = _doc[i]["line2"].as<String>();

    switch (displaymode) {
      case Normal:
        _vfd->clear();
        _vfd->fixed(line1);
        _vfd->fixed(line2);
        break;
      case VerticalScroll:
        _vfd->clear();
        _vfd->typeWriteVertical(line1 + line2);
        break;
      case HorizontalScroll:
        _vfd->clear();
        _vfd->fixed(line1);
        _vfd->typeWriteHorizontal(line2);
        break;
      case KnightRider:
        _vfd->knightRider();
        break;
      case KnightRider2:
        _vfd->knightRider2();
        break;
      case KnightRider3:
        _vfd->knightRider3();
        break;
      case FadeOut:
        _vfd->fadeOut();
        break;
      case FadeIn:
        _vfd->fadeIn();
        break;
      case ClearScreen:
        _vfd->clear();
        break;
      case SetBrightness0:
        _vfd->setBrightness0();
        break;
      case SetBrightness1:
        _vfd->setBrightness1();
        break;
      case SetBrightness2:
        _vfd->setBrightness2();
        break;
      case SetBrightness3:
        _vfd->setBrightness3();
        break;
      case SetBrightness4:
        _vfd->setBrightness4();
        break;
      case SetBrightness5:
        _vfd->setBrightness5();
        break;
      case SetBrightness6:
        _vfd->setBrightness6();
        break;
      case SetBrightness7:
        _vfd->setBrightness7();
        break;
      case FadeLeftToRight:
        _vfd->fadeLeftToRight();
        break;
      case FadeRightToLeft:
        _vfd->fadeRightToLeft();
        break;
      default:
        break;
    }

    delay(_doc[i]["delay"].as<int>());
  }
}
