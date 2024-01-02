#include <Arduino.h>
#include "display.h"
#include "test.h"
#include "timedisplay.h"
#include "webapi.h"

Display display;
TESTDISPLAY test(display);
TIMEDISPLAY tijd(display);
WEBAPI api(display);
TaskHandle_t Task1;
#define STACK_SIZE (ESP_TASK_MAIN_STACK) // Stack size for each new thread

void loop() {
    tijd.start();
    api.start();
} 

void apiUpdateLoop( void * parameter ) {
    Serial.println("Starting apiUpdateLoop, pause");
    delay(60*1000);
    Serial.println("Starting apiUpdateLoop, pause ended");
    api.update();
} 

void configModeCallback (WiFiManager *myWiFiManager) {
  display.fixed("Entered config mode");
  display.fixed(myWiFiManager->getConfigPortalSSID());
}

void setup() {
  Serial.begin(115200);
  Serial.println("Starting");
  delay(1000); 
  display.begin();
  display.fixed("Starting");  
  delay(1000); 
  display.clear();
  WiFiManager wifiManager;
  wifiManager.setDebugOutput(false);
  wifiManager.setAPCallback(configModeCallback);
  wifiManager.autoConnect("AutoConnectAP");

  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      display.fixed(".");
  }

  if(WiFi.isConnected()) {
      display.clear();
      display.fixed("Connected WiFi");
      display.enter();
      display.fixed(WiFi.localIP().toString());
      Serial.println("Connected WiFi");
      Serial.println(WiFi.localIP().toString());
  }
  api.update();
  // Initialize your task (2nd loop)
  // xTaskCreatePinnedToCore(apiUpdateLoop, "API", STACK_SIZE, nullptr, tskIDLE_PRIORITY+3 , &Task1, 1);
}
