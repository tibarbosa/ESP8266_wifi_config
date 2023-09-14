#include "WifiMan.h"

WifiMan::WifiMan()
    : server(server_port)
{
    m_ssid.clear();
    m_password.clear();
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

void WifiMan::autoConnect()
{
    if (m_ssid.isEmpty())
        m_ssid = "AP";

    wifi_manager.autoConnect(m_ssid.c_str(), NULL);
    server.begin();
}

String WifiMan::getSsid()
{
    return m_ssid;
}

String WifiMan::getPassword()
{
    return m_password;
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
                        client.print(wifi_manager.getConfigPortalSSID());
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
