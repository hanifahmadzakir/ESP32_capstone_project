#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include <PubSubClient.h>

// --- PIN DEFINITIONS ---
#define RELAY_PIN 4
#define DHT_PIN 14
#define SOIL_PIN 34
#define I2C_SDA 21
#define I2C_SCL 22

#define RELAY_ON LOW
#define RELAY_OFF HIGH

// --- MQTT TOPICS ---
extern const char* topic_cmd;
extern const char* topic_status;
extern const char* topic_telemetry;

// --- GLOBAL VARIABLES (Deklarasi extern) ---
extern PubSubClient client;
extern unsigned long startTime;
extern bool isPumpRunning;

#endif