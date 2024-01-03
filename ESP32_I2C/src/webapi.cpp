#include "Arduino.h"
#include <ArduinoJson.h>
#include "webapi.h"
#include "display.h"
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

StaticJsonDocument<15000> _doc;

String extractJsonContent(const String &input) {
    int start = input.indexOf("[{");
    int end = input.lastIndexOf("}]");
    DEBUG_PRINT("start: " + String(start));
    DEBUG_PRINT("end: " + String(end));
    if (start != -1 && end != -1 && start < end) {
      DEBUG_PRINT("input: " + input.substring(start, end + 2));
        return input.substring(start, end + 2);
    } else {
        DEBUG_PRINT("empty");
        return "";
    }
}

WEBAPI::WEBAPI(Display &display)
{
  this->_display = &display;
  _host = azureWebAPI;
  _feed = "/display";

  httpsClient.setInsecure();
  httpsClient.setTimeout(10000); // 10 Seconds
}

void WEBAPI::update()
{
  try
  {
    int retry = 0;
    DEBUG_PRINT("Connecting to: " + _host);
    while ((!httpsClient.connect(_host.c_str(), 443)) && (retry < 20))
    {
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
    while (httpsClient.connected())
    {
      _json = httpsClient.readStringUntil('\n');
      DEBUG_PRINT(_json);
      if (_json == "\r")
      {
        httpsClient.readStringUntil('\n'); // The API sends an extra line with just a number. This breaks the JSON parsing; hence, an extra read
        break;
      }
    }

    String extractedJson = extractJsonContent(httpsClient.readString());

    _doc.clear(); // Clears the reserved memory; see solution 2: https://arduinojson.org/v6/issues/memory-leak
    DeserializationError err = deserializeJson(_doc, extractedJson);
    if (err)
    {
      _display->clear();
      _display->fixed("deserializeJson failed: " + String(err.c_str()));
      delay(5000);
    }

    //httpsClient.stop();
    DEBUG_PRINT("JSON Document Size: " + _doc.size());
  }
  catch (std::exception &e)
  {
    _display->clear();
    _display->fixed("Exception: " + String(e.what()));
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

    DEBUG_PRINT("Display mode: " + String(displaymode));
    DEBUG_PRINT("Line1: " + line1);
    DEBUG_PRINT("Line2: " + line2); 

    switch (displaymode) {
      case HorizontalScroll:
        _display->begin();
        _display->fixed(line1);
        _display->carriagereturn();
        _display->fixed(line2);
        break;
      case Normal:
        _display->fixed(line1);
        _display->carriagereturn();
        _display->fixed(line2);
        break;
      case ClearScreen:
        _display->clear();
        break;
      default:
        break;
    }

    delay(_doc[i]["delay"].as<int>());
  }
}
