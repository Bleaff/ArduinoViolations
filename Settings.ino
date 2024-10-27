#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "Settings.hpp"

// Access point credentials

AsyncWebServer server(80);

void monitor_network_connection(void *monitor_structure)
{
	Settings *monitor_settings = (Settings *)monitor_structure;
	Serial.println("Waiting for the password...");
	while (true){
		if (monitor_settings->is_set != 0){
		  server.end();
		  WiFi.disconnect();
		  WiFi.begin(monitor_settings->ssid, monitor_settings->password);
		  Serial.print("Connecting to ");
      Serial.print(monitor_settings->ssid);
		  Serial.print(" Password: ");
      Serial.print(monitor_settings->password);
		  while (WiFi.status() != WL_CONNECTED) {
			  delay(500);
			  Serial.print(".");
		  }
		  Serial.println("WiFi connected successfuly!");
		  Serial.println("IP address: ");
		  Serial.println(WiFi.localIP());
		  break;
		}
		delay(10);
	}
	vTaskDelete( NULL );
}

void setup() {
	TaskHandle_t Task1;
	Settings network_setup;
	Serial.begin(115200);
	xTaskCreatePinnedToCore(
					monitor_network_connection,   /* Task function. */
					"Task1",     /* name of task. */
					30000,       /* Stack size of task */
					(void *)&network_setup,        /* parameter of the task */
					1,           /* priority of the task */
					&Task1,      /* Task handle to keep track of created task */
					0);
  // Set ESP32 as an access point
	WiFi.softAP("ESP32-Setup", "12345678");
	Serial.print("Access Point IP address: ");
	Serial.println(WiFi.softAPIP());

  // Scan for available networks
	int n = WiFi.scanNetworks();
	String wifiOptions = "";
	if (n == 0) {
		wifiOptions = "<option value=\"none\">No networks found</option>";
	} else {
	for (int i = 0; i < n; ++i) {
	  wifiOptions += "<option value=\"" + WiFi.SSID(i) + "\">" + WiFi.SSID(i) + "</option>";
	}
	}

  // Main settings page
	server.on("/", HTTP_GET, [wifiOptions](AsyncWebServerRequest *request) {
	String html = "<!DOCTYPE html><html>\
	<head><title>ESP32 Setup</title></head>\
	<body>\
	  <h1>WiFi Configuration</h1>\
	  <form action=\"/connect\" method=\"post\">\
		<label for=\"ssid\">Select WiFi Network:</label>\
		<select name=\"ssid\" id=\"ssid\">" + wifiOptions + "</select><br><br>\
		<label for=\"password\">Password:</label>\
		<input type=\"password\" id=\"password\" name=\"password\"><br><br>\
		<input type=\"submit\" value=\"Connect\">\
	  </form>\
	</body>\
	</html>";
	request->send(200, "text/html", html);
	});

  // Handle WiFi connection details submission
	server.on("/connect", HTTP_POST, [&](AsyncWebServerRequest *request) {
	int status = 0;

	// Get SSID and password from request
	if (request->hasParam("ssid", true)) {
	  network_setup.set_ssid(request->getParam("ssid", true)->value().c_str());
	  status++;
	}
	if (request->hasParam("password", true)) {
	  network_setup.set_password(request->getParam("password", true)->value().c_str());
	  status++;
	}
	if (status == 2)
	  network_setup.is_set = 1;

	// Print received values
	Serial.print("Connecting to SSID: ");
  Serial.println(network_setup.ssid);
	Serial.print("Password: ");
  Serial.println(network_setup.password);
	// Send response back to client
	request->send(200, "text/html", "<h1>Trying to connect to WiFi...</h1>");
	// Optional: Add logic to handle successful or failed connection
	});
  
	server.begin();
}

void loop() {
  delay(1000);
}
