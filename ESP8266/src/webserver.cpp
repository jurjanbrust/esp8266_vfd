#include <Arduino.h>
#include "webserver.h"
#include "vfd.h"

const char* PARAM_MESSAGE = "message";


WEBSERVER::WEBSERVER(VFD& vfd, AsyncWebServer& server) : _vfd(&vfd), _server(&server) {
}

void WEBSERVER::start()
{
    //scrollText();
}

void WEBSERVER::notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

void WEBSERVER::setupWebServer()
{
//   server.on("/api", HTTP_GET, [](AsyncWebServerRequest *request){
//     String response = "";

//     if (request->hasParam("message", true)) {
//       response = request->getParam("message", true)->value();
//     } else {
//       response = "No message sent";
//     }    
//   });
    // Send a GET request to <IP>/get?message=<message>
    // _server->on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    //     String message;
    //     if (request->hasParam(PARAM_MESSAGE)) {
    //         message = request->getParam(PARAM_MESSAGE)->value();
    //     } else {
    //         message = "No message sent";
    //     }
    //     request->send(200, "text/plain", "Hello, GET: " + message);
    // });
    // Send a GET request to <IP>/get?message=<message>
    _server->on("/api", HTTP_GET, [this] (AsyncWebServerRequest *request) {
        String message;
        message = request->arg("command");
        request->send(200, "text/plain", "Hello, GET: " + message);
        Serial.print(message);
        this->_vfd->typeWriteVertical(message);
    });

    _server->onNotFound( [] (AsyncWebServerRequest *request) {
        request->send(404, "text/plain", "Not found");
    });

    _server->begin();
}

void WEBSERVER::scrollText(String message)
{
    this->_vfd->typeWriteVertical(message);
    delay(1000);
}