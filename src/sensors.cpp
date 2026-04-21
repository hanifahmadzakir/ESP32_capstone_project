#include <Arduino.h>
#include <DHT.h>
#include <Wire.h>
#include <BH1750.h>
#include <ArduinoJson.h>
#include "sensors.h"
#include "globals.h"

#define DHTTYPE DHT22
DHT dht(DHT_PIN, DHTTYPE);
BH1750 lightMeter;

void setup_sensors() {
  dht.begin();
  Wire.begin(I2C_SDA, I2C_SCL);
  if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    Serial.println("[SENSOR] BH1750 Siap");
  } else {
    Serial.println("[SENSOR] BH1750 Gagal!");
  }
}

void publish_telemetry() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println("[SENSOR] Gagal baca DHT22!");
    t = 0.0; h = 0.0;
  }

  float lux = lightMeter.readLightLevel();
  int rawSoil = analogRead(SOIL_PIN);
  int soilPercent = map(rawSoil, 4095, 0, 0, 100);
  soilPercent = constrain(soilPercent, 0, 100);

  StaticJsonDocument<200> doc;
  doc["temperature"] = t;
  doc["air_humidity"] = h;
  doc["light_intensity"] = lux;
  doc["soil_moisture"] = soilPercent;

  char outMsg[200];
  serializeJson(doc, outMsg);

  client.publish(topic_telemetry, outMsg);
  Serial.println("[TELEMETRY] Terkirim: " + String(outMsg));
}