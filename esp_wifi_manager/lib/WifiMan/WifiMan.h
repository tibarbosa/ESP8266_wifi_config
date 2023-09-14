#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <Arduino.h>

class WifiMan
{
public:
    WifiMan();
    ~WifiMan() = default;

    void init(String ssid, String password);
    void init(String ssid);
    void autoConnect();

    String getSsid();
    String getPassword();

    void checkClientConnection();

private:
    String m_ssid;
    String m_password;

    WiFiServer server;
    String header;
    WiFiManager wifi_manager;
};
static const constexpr uint16_t server_port = 80;
