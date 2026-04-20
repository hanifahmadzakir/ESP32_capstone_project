#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "globals.h"
#include "network.h"
#include "sensors.h"
#include "credentials.h"

// --- DEFINISI VARIABEL GLOBAL (Hanya ditulis di sini) ---
const char* topic_cmd = "kebun/pompa/cmd";
const char* topic_status = "kebun/pompa/status";
const char* topic_telemetry = "kebun/sensor/telemetri";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long startTime = 0;
bool isPumpRunning = false;
// --------------------------------------------------------

unsigned long lastTelemetryTime = 0;
const long telemetryInterval = 60000; // 1 Menit atau 60 detik

void setup() {
  Serial.begin(115200);
  
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, RELAY_OFF);

  setup_wifi();
  
  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(mqtt_callback); // Hook callback ke modul network

  setup_sensors();
}

void loop() {
  if (!client.connected()) {
    reconnect_mqtt();
  }
  client.loop(); 

  unsigned long currentMillis = millis();
  if (currentMillis - lastTelemetryTime >= telemetryInterval) {
    lastTelemetryTime = currentMillis;
    publish_telemetry();
  }
}