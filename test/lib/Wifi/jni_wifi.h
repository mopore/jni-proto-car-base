#pragma once
#include <Arduino.h>
#include <string>

#define NO_IP "0.0.0.0"
#define MAX_IP_LENGTH 16

void connect_wifi(char out_connectIp[MAX_IP_LENGTH], const char* ssid, const char* password);