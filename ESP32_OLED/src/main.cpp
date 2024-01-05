#include <Arduino.h>
#include "display.h"
#include "test.h"
#include "timedisplay.h"
#include "webapi.h"

Display oled;
TESTDISPLAY test(oled);
TIMEDISPLAY tijd(oled);
WEBAPI api(oled);
TaskHandle_t Task1;

void loop2( void * parameter) {
  for(;;) {
      api.update();
      delay(60*1000);
  }
}

void loop() {
  tijd.start();
  oled.clear();
  api.start();
} 

void configModeCallback (WiFiManager *myWiFiManager) {
  oled.fixed("Entered config mode");
  oled.fixed(myWiFiManager->getConfigPortalSSID());
}

void setup() {
  Serial.begin(115200);
  Serial.println("Starting");
  delay(1000); 
  oled.begin();
  oled.clear();
  oled.home();
  WiFiManager wifiManager;
  wifiManager.setDebugOutput(false);
  wifiManager.setAPCallback(configModeCallback);
  wifiManager.autoConnect("AutoConnectAP");

  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      //oled.fixed(".");
  }

  if(WiFi.isConnected()) {
      //oled.clear();
      oled.fixed("Connected WiFi");
      oled.enter();
      oled.fixed(WiFi.localIP().toString());
      Serial.println("Connected WiFi");
      Serial.println(WiFi.localIP().toString());
      delay(3000);
      oled.clear();
      oled.home();
  }

  xTaskCreatePinnedToCore(loop2, "API", 10000, NULL, 0 , &Task1, 1);
}
