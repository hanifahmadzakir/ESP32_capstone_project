# 🌾 Precision Farming - IoT Edge Node (ESP32)

This repository contains the source code (C++/Arduino Framework) for the **ESP32** hardware edge node, which is a core component of the *Precision Farming Smart Irrigation System* ecosystem.

Developed using **PlatformIO**, this project serves as the edge device deployed directly in the agricultural field to read environmental data and execute watering commands.

## ✨ Key Features

1. **Edge Computing for Pump Duration:** This device does not rely on the backend to calculate watering time. The ESP32 autonomously calculates the exact duration (in seconds) using the `millis()` function while the relay is active, ensuring 100% accurate electricity cost calculations even during network latency or disconnects.
2. **Telemetry Broadcasting:** Continuously reads environmental data (Soil Moisture, Temperature, Light Intensity) and publishes it in JSON format at defined intervals.
3. **Bidirectional MQTT Communication:** Maintains a real-time connection to the MQTT broker (Mosquitto) to receive execution commands (Manual Override / DSS API) and send device status reports.
4. **Auto-Reconnect Mechanism:** Equipped with a self-healing function that automatically attempts to reconnect to both the Wi-Fi network and the MQTT Broker if the connection drops in the field.

## 🛠️ Tech Stack & Dependencies

* **Microcontroller:** ESP32 (Espressif)
* **Framework:** Arduino (via PlatformIO)
* **Dependencies / Libraries:**
  * `knolleary/PubSubClient` (MQTT Client)
  * `bblanchon/ArduinoJson` (JSON parsing and formatting)
  * *(Add other sensor libraries here, e.g., DHT sensor library)*

## 🔌 Hardware & Pin Mapping

| Component | ESP32 Pin (GPIO) | Description |
| :--- | :--- | :--- |
| **1.5HP Pump Relay** | `GPIO 4` | Active-High / Active-Low (Adjust based on module) |
| **Soil Moisture Sensor** | `GPIO 34` | Analog Input (ADC) |
| **Air Temperature Sensor**| `GPIO 14` | Digital Input |
| **LDR / Light Sensor** | `GPIO 35` | Analog Input (ADC) |

*(Note: Adjust the GPIO pin numbers above to match your actual field wiring)*

## 📡 MQTT Topic Architecture

This device uses the following topics to communicate with the MING stack (MQTT, InfluxDB, Node-RED, Grafana) and the ExpressJS backend:

### 1. Receiving Commands (Subscribe)
* **Topic:** `kebun/pompa/cmd`
* **Payload Expected:** `ON` or `OFF` (Plain string)

### 2. Sending Status & Duration (Publish)
Sent immediately after the relay is turned off so the ExpressJS backend can calculate operational costs.
* **Topic:** `kebun/pompa/status`
* **Payload JSON:**
  ```json
  {
    "status": "OFF",
    "duration_seconds": 125
  }


3. Sending Sensor Telemetry (Publish)
Sent every N minutes to be stored in InfluxDB and visualized in Grafana.

Topic: kebun/sensor/telemetri

Payload JSON:
{
  "soil_moisture": 45,
  "temperature": 28.5,
  "light_intensity": 800
}

🚀 Installation & Build Guide (PlatformIO)
Clone this repository:

Bash
git clone <your-repo-url>
Open the project folder using VS Code with the PlatformIO extension installed.

Create a src/credentials.h file (ensure this is in your .gitignore to prevent leaking credentials) and input your network configuration:

C++
#ifndef CREDENTIALS_H
#define CREDENTIALS_H

const char* WIFI_SSID = "YOUR_WIFI_NAME";
const char* WIFI_PASS = "YOUR_WIFI_PASSWORD";
const char* MQTT_SERVER = "YOUR_VPS_OR_BROKER_IP";
const int MQTT_PORT = 1883;
const char* MQTT_USER = "mqtt_username"; // Optional
const char* MQTT_PASS = "mqtt_password"; // Optional

#endif

Connect the ESP32 to your computer via a USB cable.

Click the Build button (checkmark icon) in the bottom PlatformIO toolbar to compile.

Click the Upload button (right arrow icon) to flash the firmware to the ESP32.

Open the Serial Monitor (plug icon) with a baud rate of 115200 to view the booting and connection process.

👨‍💻 Contributors
Capstone Team - [List other team members here]

Developed for the Information Systems Capstone Project.