#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "network.h"
#include "globals.h"
#include "credentials.h"

void setup_wifi() {
  delay(10);
  Serial.println("\n[WIFI] Menghubungkan ke " + String(WIFI_SSID));
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n[WIFI] Terhubung! IP: " + WiFi.localIP().toString());
}

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (int i = 0; i < length; i++) message += (char)payload[i];
  
  Serial.printf("[MQTT] Pesan masuk [%s]: %s\n", topic, message.c_str());

  if (String(topic) == topic_cmd) {
    
    if (message == "ON" && !isPumpRunning) {
      digitalWrite(RELAY_PIN, RELAY_ON);
      startTime = millis();
      isPumpRunning = true;
      Serial.println("[POMPA] MENYALA. Mengirim konfirmasi (ACK)...");

      StaticJsonDocument<128> doc;
      doc["Pump"] = "ON";
      
      char outMsg[128];
      serializeJson(doc, outMsg);
      client.publish(topic_status, outMsg);
    } 
    
    else if (message == "OFF" && isPumpRunning) {
      digitalWrite(RELAY_PIN, RELAY_OFF);
      
      unsigned long durationSec = (millis() - startTime) / 1000;
      isPumpRunning = false;
      Serial.printf("[POMPA] MATI. Durasi: %lu detik. Mengirim konfirmasi (ACK)...\n", durationSec);

      StaticJsonDocument<128> doc;
      doc["Pump"] = "OFF";
      doc["duration_seconds"] = durationSec;
      
      char outMsg[128];
      serializeJson(doc, outMsg);
      client.publish(topic_status, outMsg);
    }
  }
}

void reconnect_mqtt() {
  while (!client.connected()) {
    Serial.print("[MQTT] Menghubungkan ke broker...");
    String clientId = "ESP32_Edge_" + String(random(0xffff), HEX);
    
    if (client.connect(clientId.c_str(), MQTT_USER, MQTT_PASS)) {
      Serial.println(" Terhubung!");
      client.subscribe(topic_cmd);
    } else {
      Serial.print(" Gagal, rc=");
      Serial.print(client.state());
      Serial.println(". Coba lagi dalam 5 detik.");
      delay(5000);
    }
  }
}