#include "WifiMan.h"

WifiMan::WifiMan()
    : server(server_port), m_observer(nullptr)
{
    g_instance = this;
    m_ssid.clear();
    m_password.clear();
    wifi_manager.setAPCallback(apModeCallback);
    wifi_manager.setDebugOutput(false);
}

void WifiMan::init(String ssid, String password)
{
    m_ssid = ssid;
    m_password = password;
}

void WifiMan::init(String ssid)
{
    m_ssid = ssid;
}

bool WifiMan::autoConnect()
{
    if (m_ssid.isEmpty())
        m_ssid = "AP";

    bool ret = wifi_manager.autoConnect(m_ssid.c_str(), NULL);
    server.begin();

    return ret;
}

void WifiMan::checkClientConnection()
{
    WiFiClient client = server.accept();

    if (client)
    {
        String currentLine = "";
        while (client.connected())
        {
            if (client.available())
            {
                char c = client.read();
                header += c;
                if (c == '\n')
                {
                    if (currentLine.length() == 0)
                    {
                        if (header.indexOf("GET /btn/erase") >= 0)
                        {
                            Serial.println("Resetting Settings...");
                            ESP.eraseConfig();
                            delay(2000);
                            ESP.reset();
                        }

                        // Display the HTML web page
                        client.println("<!DOCTYPE html><html>");
                        client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
                        client.println("<link rel=\"icon\" href=\"data:,\">");
                        // CSS to style buttons
                        client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
                        client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
                        client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
                        client.println(".button2 {background-color: #77878A;}</style></head>");

                        // Web Page Heading
                        // client.println("<body><h1>ESP Web Server</h1>");
                        client.print("<body><h1>");
                        client.print("IOT Module");
                        client.println("</h1>");
                        client.println("<p><a href=\"/btn/erase\"><button class=\"button\">Erase Wifi Config</button></a></p>");
                        client.println("</body></html>");

                        client.println();
                        break;
                    }
                    else
                        currentLine = "";
                }
                else if (c != '\r')
                {
                    currentLine += c;
                }
            }
        }
        header = "";
        client.stop();
    }
}

String WifiMan::getLocalIP()
{
    return WiFi.localIP().toString();
}

void WifiMan::apModeCallback(WiFiManager *wifi_manager)
{
    if (WifiMan::g_instance && WifiMan::g_instance->m_observer)
        g_instance->m_observer->enteredApMode(wifi_manager->getConfigPortalSSID(), "", "192.168.4.1");
}

void WifiMan::registerObserver(Observer *obs)
{
    m_observer = obs;
}

void WifiMan::unregisterObserver()
{
    m_observer = nullptr;
}
