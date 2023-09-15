#include <Arduino.h>
#include  "WifiMan.h"

WifiMan wifi_man;

void setup()
{
  Serial.begin(9600);
  delay(100);
  Serial.println("Starting Wifi manager");

  String ssid = "Iot_" + String(ESP.getChipId());

  wifi_man.init(ssid);
  wifi_man.autoConnect();
}

void loop()
{
  wifi_man.checkClientConnection();  
}