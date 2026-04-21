# 🌱 Smart Agriculture IoT System - Capstone Project Universitas Terbuka STSI4440.15 CP15B

An *Internet of Things* (IoT) system based on an *Edge-to-Cloud Microservices* architecture for smart agricultural pump monitoring and control. This system bridges Operational Technology (OT) in the field with Information Technology (IT) in the cloud via the MQTT protocol.

## 🏗️ System Architecture

This project is designed with a modular architecture that strictly separates hardware (Edge) and software (Backend/Frontend) logic:

1. **Edge Node (ESP32):** Responsible for reading sensors, controlling actuators (Pump Relay), displaying a local interface (HMI OLED), and communicating over a Wi-Fi network.
2. **Message Broker (Mosquitto MQTT):** Acts as a lightweight and secure (authenticated) real-time communication bridge between the edge hardware and the server.
3. **Logic & Integration (Node-RED):** Handles the data flow, stores historical records to the database, and routes commands to/from the user interface.
4. **Data Persistence (PostgreSQL):** A relational database used to store watering history, actual pump runtimes, and telemetry data.
5. **Visualization (Grafana):** Provides a comprehensive dashboard for sensor metrics and node performance, securely embedded directly into the client application.
6. **Frontend (ReactJS):** The End-User UI, hosted on Vercel.

## 🧰 Hardware Components

* **Microcontroller:** ESP32 Dev Module (NodeMCU)
* **HMI Display:** OLED 128x64 (I2C) - Displays IP address, RSSI, MQTT status, and Pump status
* **Actuator:** Relay Module (Active LOW/HIGH) & Water Pump
* **Sensors:** * DHT11/DHT22 (Temperature & Humidity)
    * BH1750 (Light Intensity - I2C)
    * Soil Moisture Sensor (Analog)

## 📡 Communication Protocol (MQTT)

The system utilizes a two-way communication architecture (*Closed-Loop Feedback*) to ensure every command is executed accurately.

### 1. Sensor Telemetry (Routine)
* **Topic:** `kebun/sensor/telemetri`
* **Interval:** Every 60 seconds (1 minute)
* **Payload (JSON):** Temperature, air humidity, light intensity, and soil moisture data.

### 2. Pump Control (Event-Driven)
* **Command Topic (Node-RED -> ESP32):** `kebun/pompa/cmd`
    * Payload: `ON` or `OFF` (Plain String)
* **Status/ACK Topic (ESP32 -> Node-RED):** `kebun/pompa/status`
    * Payload when turned ON: `{"Pump": "ON"}`
    * Payload when turned OFF: `{"Pump": "OFF", "duration_seconds": 120}` (Sends the actual duration the pump was active).

## 💻 Firmware Code Structure (C++ / PlatformIO)

The ESP32 firmware is divided into multiple independent modules (*Separation of Concerns*):

* `main.cpp`: Entry point and main loop (Non-blocking timer).
* `credentials.h`: Secret configurations (`#define` macros for SSID, Password, and MQTT Auth).
* `network.cpp` / `.h`: Handles Wi-Fi connection, MQTT reconnection, and the `mqtt_callback` function.
* `sensors.cpp` / `.h`: Handles sensor readings and JSON payload construction for the DHT, BH1750, and Soil Moisture sensors.
* `display.cpp` / `.h`: Manages the UI layout on the SSD1306 OLED display.

## 🚀 Deployment Guide (Ubuntu 24.04 VPS)

The system's backend is deployed on a single VPS (1 Core, 2GB RAM) using **Docker Compose** to ensure proper service isolation.

1. **Clone the repository and navigate to the server directory:**
   ```bash
   git clone <YOUR_REPO_URL>
   cd iot_backend
2. **Set up Mosquitto security permissions:**
   ```bash
   sudo chmod 644 mosquitto/config/pwfile
3. **Run Microservices stack:**
   ```bash
   docker compose up -d
3. **Running containers: **
   ```bash
   * iot_mosquitto (Port 1883)
   * iot_postgres (Port 5432)
   * iot_nodered (Port 1880)
   * iot_grafana (Port 3000 - Iframe embedding enabled)

## 🔒 Security Measures
Firmware: Utilizes #define macros in a separate credentials.h file, which is added to .gitignore to prevent credential leaks on GitHub.

MQTT Broker: Anonymous access is disabled (allow_anonymous false), requiring hashed username and password authentication.

Node-RED: The UI Editor is protected using a bcrypt hashed password inside settings.js.

Server Access: Root password login is disabled, entirely replaced by asymmetric authentication (SSH Public Key ed25519).