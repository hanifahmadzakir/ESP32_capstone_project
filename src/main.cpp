#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "globals.h"
#include "network.h"
#include "sensors.h"
#include "credentials.h"
#include "display.h"

// --- VARIABEL GLOBAL ---
const char *topic_cmd = "kebun/pompa/cmd";
const char *topic_status = "kebun/pompa/status";
const char *topic_telemetry = "kebun/sensor/telemetri";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long startTime = 0;
bool isPumpRunning = false;
// --------------------------------------------------------

unsigned long lastTelemetryTime = 0;
const long telemetryInterval = 60000; // 1 Menit atau 60 detik

extern WiFiClient espClient;
extern PubSubClient client;
extern bool isPumpRunning;

unsigned long lastDisplayUpdate = 0;
const long displayInterval = 2000;

void setup()
{
  Serial.begin(115200);

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, RELAY_OFF);

  setup_wifi();
  setup_display();

  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(mqtt_callback); // Hook callback

  setup_sensors();
}

void loop()
{
  if (!client.connected())
  {
    reconnect_mqtt();
  }
  client.loop();

  // milis calc
  unsigned long currentMillis = millis();

  // telemetry routine
  if (currentMillis - lastTelemetryTime >= telemetryInterval)
  {
    lastTelemetryTime = currentMillis;
    publish_telemetry();
  }

  // display routine
  if (currentMillis - lastDisplayUpdate >= displayInterval)
  {
    lastDisplayUpdate = currentMillis;

    String currentIP = WiFi.localIP().toString();
    int currentRSSI = WiFi.RSSI();
    bool mqttStatus = client.connected();

    update_display(currentIP, currentRSSI, mqttStatus, isPumpRunning);
  }
}