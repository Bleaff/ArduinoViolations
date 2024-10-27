#include "InitializeSettingsServer.hpp"

InitializeSettingsServer::InitializeSettingsServer(int port, const char *ssid, const char *password)
    : port(port), ssid(ssid), password(password) {
    this->server = AsyncWebServer(port);
    Serial.begin(115200);
    WiFi.softAP(ssid, password);
    Serial.print("Access Point IP address: ");
    Serial.println(WiFi.softAPIP());
}

InitializeSettingsServer::ISettingsServer(const ISettingsServer &other)
    : port(other.port), ssid(other.ssid), password(other.password) {
    // Additional deep copy steps, if necessary
}

void InitializeSettingsServer::start(void) {
    this->server.on("/", HTTP_GET, [this](AsyncWebServerRequest *request) {
        request->send(200, "text/html", this->get_wifi_option());
    });
    
    this->server.begin();
}


String InitializeSettingsServer::get_wifi_option() {
    String wifiOptions = "<option value=\"none\">No networks found</option>";
    int n = WiFi.scanNetworks();
    if (n == 0) {
        wifiOptions = "<option value=\"none\">No networks found</option>";
    } else {
        for (int i = 0; i < n; ++i) {
            wifiOptions += "<option value=\"" + WiFi.SSID(i) + "\">" + WiFi.SSID(i) + "</option>";
        }
    }
    return wifiOptions;
}

void 

void ISettingsServer::start_monitor_thread() {
    xTaskCreatePinnedToCore(
        monitor_network_connection,   // Task function
        "monitor_task",               // Name of task
        30000,                        // Stack size of task
        this,                         // Pass the instance as the task parameter
        1,                            // Priority of the task
        &Task1,                       // Task handle to keep track of created task
        0                             // Core where the task should run
    );
}
