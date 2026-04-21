#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>

void setup_display();
void update_display(String ip, int rssi, bool mqtt_connected, bool pump_status);

#endif