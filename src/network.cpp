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

  // Pastikan perintah berasal dari topik yang benar
  if (String(topic) == topic_cmd) {
    
    // 🟢 SKENARIO 1: MENERIMA PERINTAH ON
    if (message == "ON" && !isPumpRunning) {
      // 1. Eksekusi perangkat keras DULU!
      digitalWrite(RELAY_PIN, RELAY_ON);
      startTime = millis();
      isPumpRunning = true;
      Serial.println("[POMPA] MENYALA. Mengirim konfirmasi (ACK)...");

      // 2. Susun JSON Feedback ON
      StaticJsonDocument<128> doc;
      doc["Pump"] = "ON";
      
      // 3. Kirim Feedback ke Topik Status
      char outMsg[128];
      serializeJson(doc, outMsg);
      client.publish(topic_status, outMsg);
    } 
    
    // 🔴 SKENARIO 2: MENERIMA PERINTAH OFF
    else if (message == "OFF" && isPumpRunning) {
      // 1. Eksekusi perangkat keras DULU!
      digitalWrite(RELAY_PIN, RELAY_OFF);
      
      // 2. Kalkulasi Durasi
      unsigned long durationSec = (millis() - startTime) / 1000;
      isPumpRunning = false;
      Serial.printf("[POMPA] MATI. Durasi: %lu detik. Mengirim konfirmasi (ACK)...\n", durationSec);

      // 3. Susun JSON Feedback OFF beserta data biayanya
      StaticJsonDocument<128> doc;
      doc["Pump"] = "OFF";
      doc["duration_seconds"] = durationSec;
      
      // 4. Kirim Feedback ke Topik Status
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