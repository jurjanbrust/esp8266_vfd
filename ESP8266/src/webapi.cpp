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
    #define DEBUG_PRINT(x)  Serial.println (x); delay(1000);
  #else
    #define DEBUG_PRINT(x)
  #endif
#endif

DynamicJsonDocument _doc(4000);

WEBAPI::WEBAPI(VFD& vfd) {

    this->_vfd = &vfd;
    _host = azureWebAPI;
    _feed = "/display";
}

void WEBAPI::update() {

  WiFiClientSecure httpsClient;
  httpsClient.setInsecure();
  httpsClient.setTimeout(15000); // 15 Seconds
  delay(1000);

  int retry=0;
  while((!httpsClient.connect(_host, 443)) && (retry < 20)){
      delay(100);
      DEBUG_PRINT(".");
      retry++;
  }

  DEBUG_PRINT("requesting URL: ");
  DEBUG_PRINT(_host + _feed);
  httpsClient.print(String("GET ") + _feed + " HTTP/1.1\r\n" +
               "Host: " + _host + "\r\n" +
               "Connection: close\r\n\r\n");


  DEBUG_PRINT("headers:");
  while (httpsClient.connected()) {
    _json = httpsClient.readStringUntil('\n');
    DEBUG_PRINT(_json);
    if (_json == "\r") {
      break;
    }
  }
  
  httpsClient.readStringUntil('\n'); // The API sends an extra line with just a number. This breaks the JSON parsing, hence an extra read
  while(httpsClient.connected()){
    _json = httpsClient.readString();
    DEBUG_PRINT(_json);
  }

  DeserializationError err = deserializeJson(_doc,_json);
  if (err) {
    _vfd->clear();
    _vfd->typeWriteHorizontal("deserializeJson failed");
    delay(5000);
  }
  DEBUG_PRINT("size: " + _doc.size());
}

void WEBAPI::start() {

  enum mode {Normal,
            FadeInOut,
            Scroll,
            KnightRider,
            VerticalScroll,
            HorizontalScroll};

  int displaymode = 0;
  String line1;
  String line2;

  DEBUG_PRINT("display data, size: ");
  DEBUG_PRINT(_doc.size());
  for(unsigned int i=0; i<_doc.size();i++) {
    _vfd->clear();
    
    displaymode = _doc[i]["displayMode"].as<int>();
    line1 = _doc[i]["line1"].as<String>();
    line2 = _doc[i]["line2"].as<String>();

    if(displaymode == VerticalScroll) {
        _vfd->typeWriteVertical(line1 + line2);
    } else if(displaymode == HorizontalScroll) {
        _vfd->fixed(line1);
        _vfd->typeWriteHorizontal(line2);
    } else {
      _vfd->fixed(line1);
      _vfd->fixed(line2);
    }

    delay(_doc[i]["delay"].as<int>());
  }
  _vfd->clear();
}
