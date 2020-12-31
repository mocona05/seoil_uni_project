#pragma once
#include <stdbool.h>





void setup_wifi(void);
void reconnect(void);
void callback(char* topic, byte* message, unsigned int length);
void mqtt_init(void);
void mqtt_handler(float humi, float temp);
