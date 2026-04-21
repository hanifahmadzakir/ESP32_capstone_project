#ifndef NETWORK_H
#define NETWORK_H

#include<Arduino.h>

extern bool isPumpRunning; 

void setup_wifi();
void reconnect_mqtt();
void mqtt_callback(char* topic, byte* payload, unsigned int length);

#endif