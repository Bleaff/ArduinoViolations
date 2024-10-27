#ifndef INITIALIZE_SETTINGS_SERVER_HPP
#define INITIALIZE_SETTINGS_SERVER_HPP

#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "Settings.hpp"


class ISettingsServer
{
public:
  Settings settings;
  ISettingsServer(int port, const char *ssid, const char *password);
  ISettingsServer(const ISettingsServer &settings);
  void start(void);
  void stop(void);
private:
  int port = 0;
  const char *ssid;
  const char *password;
  TaskHandle_t Task1;
  void start_monitor_thread(void);
  void monitor_network_connection(void *params);
  String get_wifi_option();
}

#endif