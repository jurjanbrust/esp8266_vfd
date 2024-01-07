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

void loop2( void * parameter) {
  for(;;) {
      delay(10*60*1000);
      api.update();
  }
}

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
  display.clear();
  WiFiManager wifiManager;
  wifiManager.setDebugOutput(false);
  wifiManager.setAPCallback(configModeCallback);
  wifiManager.autoConnect("AutoConnectAP");

  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
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
  xTaskCreatePinnedToCore(loop2, "API", 10000, NULL, 0 , &Task1, 1);
}
