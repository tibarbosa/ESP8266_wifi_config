#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <Arduino.h>

class WifiMan
{
public:
    class Observer
    {
    public:
        virtual void enteredApMode(const String &ssid, const String &pwd, const String &ip) = 0;
    };

    WifiMan();
    ~WifiMan() = default;

    void init(String ssid, String password);
    void init(String ssid);
    bool autoConnect();

    void checkClientConnection();
    String getLocalIP();

    static void apModeCallback(WiFiManager *wifi_manager);
    static void connectedCallback(void);

    void registerObserver(Observer *obs);
    void unregisterObserver();

private:
    String m_ssid;
    String m_password;

    WiFiServer server;
    String header;
    WiFiManager wifi_manager;

    static WifiMan *g_instance;
    Observer *m_observer;
};
static const constexpr uint16_t server_port = 80;
