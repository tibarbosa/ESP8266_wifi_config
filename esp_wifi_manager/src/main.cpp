#include <Arduino.h>
#include "WifiMan.h"

struct WifiManObs : public WifiMan::Observer
{
  void enteredApMode(const String &ssid, const String &pwd, const String &ip) override
  {
    Serial.print("Entered in Ap mode: ");
    Serial.print(ssid);
    Serial.print(" | ");
    Serial.print(pwd);
    Serial.print(" | ");
    Serial.println(ip);
  }
};

WifiMan *WifiMan::g_instance = nullptr;
WifiManObs m_wifi_man_obs;
WifiMan wifi_man;

void setup()
{
  Serial.begin(9600);

  // Give 3s to open a serial monitor
  delay(3000);
  Serial.println("Starting Wifi manager");

  String ssid = "Iot_" + String(ESP.getChipId());
  wifi_man.registerObserver(&m_wifi_man_obs);

  wifi_man.init(ssid);
  
  if (wifi_man.autoConnect())
  {
    Serial.print("Local Ip: ");
    Serial.println(wifi_man.getLocalIP());
  }
}

void loop()
{
  wifi_man.checkClientConnection();
}