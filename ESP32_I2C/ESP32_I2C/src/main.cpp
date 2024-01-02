#include <Arduino.h>
#include "display.h"
#include "test.h"
#include "timedisplay.h"
#include "webapi.h"

Display display;
TESTDISPLAY test(display);
TIMEDISPLAY tijd(display);
WEBAPI api(display);

void loop() {

    api.update();
    tijd.start();

    delay(500);
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
}
