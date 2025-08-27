 // =================
  //  File Name:
  //  Author: Jack Holdsworth
  //  ID: 20023933
  //  Create Time: 2024-04-13 08:59:26
  //  Modified time: 2024-04-24 12:06:39
  //  Description:
 // =================

#pragma once
#include <Arduino.h>
#include <Fonts/FreeSans9pt7b.h>
#include "../spsgrf/spsgrf.hpp"
#include <esp_clk.h>
#include <soc/rtc.h>
 
#define MAX_PACKETS 10
struct GraphConfig {
    int broadcastPeriod = 0;
    int listenPeriod = 0;
    int pollPeriod = 0;
    int volts[10] = {0};
    int amps[10] = {0};
    int packetHeader = 5;
    unsigned long lastGraphUpdateTime = 0;
    unsigned long lastGraphUpdateNumber = 0;
};

class PacketHandler {
private:
    GraphConfig config[MAX_PACKETS];
    int current_packet = 0;
    Spirit spirit;
public:
    PacketHandler(){}

    void get_ordered_config(GraphConfig* ordered_config) {
        
        int index = current_packet; // Start from the current packet

        // Fill the array from most recent to oldest data
        for (int i = 0; i < MAX_PACKETS; i++) {
            ordered_config[i] = config[index];
            index--; // Move to the previous packet
            if (index < 0) { // Wrap around if necessary
                index = MAX_PACKETS - 1;
            }
        }
    }
    GraphConfig* get_current_config() {
        return &config[current_packet];
    }
    
    void set_current_packet(){
        for (int i =  0; i < MAX_PACKETS ; i++) {
            Serial.println(i);
            Serial.println("ok");
            if (config[i].broadcastPeriod == 0 || config[i].pollPeriod == 0){
                current_packet = i-1;
                return;
            }

        }
        current_packet = 9;
    }
    void get_packet(){
        uint8_t* rxBuffer = spirit.SpiritGet();
        current_packet = (current_packet + 1) % MAX_PACKETS;
        config[current_packet].broadcastPeriod = rxBuffer[2];
        config[current_packet].listenPeriod = rxBuffer[3];
        config[current_packet].pollPeriod = rxBuffer[4];
        int num_bytes = config[current_packet].broadcastPeriod / config[current_packet].pollPeriod;

        for (int x = config[current_packet].packetHeader, y = 0, z = (config[current_packet].packetHeader + num_bytes); 
             x <= (config[current_packet].packetHeader + num_bytes) && y < num_bytes && z <= (config[current_packet].packetHeader + num_bytes * 2); 
             x++, y++, z++) {
            Serial.println(rxBuffer[x]);
            Serial.println(rxBuffer[z]);
            // config[current_packet].volts[y] = rxBuffer[x];
            // config[current_packet].amps[y] = rxBuffer[z];
            config[current_packet].volts[y] = random(0, 10);
            config[current_packet].amps[y] = random(0, 10);
        }
        config[current_packet].lastGraphUpdateTime = rtc_time_slowclk_to_us(rtc_time_get(), esp_clk_slowclk_cal_get())/1000;
        config[current_packet].lastGraphUpdateNumber = 0;
    }

    GraphConfig& getConfig() {
        return config[current_packet];
    }
};