#include "display.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1 
#define SCREEN_ADDRESS 0x3C // Alamat I2C umum untuk OLED 128x64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup_display() {
  // Inisialisasi OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("OLED SSD1306 gagal diinisialisasi"));
    for(;;);
  }
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  
  display.setCursor(10, 25);
  display.println("MEMULAI SISTEM...");
  display.display();
  delay(2000);
}

void update_display(String ip, int rssi, bool mqtt_connected, bool pump_status) {
  display.clearDisplay();
  
  // Baris 1: Judul HMI
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("=== IoT END NODE ===");

  // Baris 2: IP Address
  display.setCursor(0, 15);
  display.print("IP  : ");
  display.println(ip);

  // Baris 3: RSSI (Sinyal WiFi)
  display.setCursor(0, 27);
  display.print("WiFi: ");
  display.print(rssi);
  display.println(" dBm");

  // Baris 4: Status MQTT
  display.setCursor(0, 39);
  display.print("MQTT: ");
  if (mqtt_connected) {
    display.println("CONNECTED");
  } else {
    display.println("DISCONNECTED");
  }

  // Baris 5: Status Pompa
  display.setCursor(0, 51);
  display.print("PUMP: ");
  if (pump_status) {
    display.println("[ ON ] >>>");
  } else {
    display.println("[ OFF ]");
  }

  display.display();
}