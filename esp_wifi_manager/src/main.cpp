#include <Arduino.h>
#include  "WifiMan.h"

WifiMan wifi_man;

void setup()
{
  Serial.begin(115200);
  delay(100);
  Serial.println("Starting Wifi manager");

  String ssid = "DisplaxIotHH_" + String(ESP.getChipId());

  wifi_man.init(ssid);
  wifi_man.autoConnect();
}

void loop()
{
  wifi_man.checkClientConnection();  
}