#include "InitializeSettingsServer.hpp"

void setup() {
	InitializeSettingsServer server(80, "ESP32", "12345678");
	server.start();
}

void loop() {
  delay(1000);
}
