 // =================
  //  File Name: graph_screen.hpp
  //  Author: Jack Holdsworth
  //  ID: 20023933
  //  Create Time: 2024-03-06 15:57:25
  //  Modified time: 2024-05-13 22:36:24
  //  Description:
  //  Modified time: 2024-04-24 12:06:11
#pragma once
#include <Arduino.h>
#include "screen.hpp"
#include <Fonts/FreeSans9pt7b.h>
#include "../graph/graph.hpp"
#include "../battery/battery.hpp"
#include "../signal/signal.hpp"
#include "../spsgrf/spsgrf.hpp"
#include "../spsgrf/packet_handler.hpp"
#include <esp_clk.h>
#include <soc/rtc.h>
#include <Adafruit_MAX1704X.h>
 

class GraphScreen : public Screen {
private:
    Graph graph = Graph(180,95, 40, 50);
    Battery battery = Battery(30, 15, 200, 5);
    Signal signalIcon = Signal(180, 13, 20);
    Spirit spirit;
    GraphConfig config;
    PacketHandler& handler;
    Adafruit_MAX17048 lipo;
    
public:
    GraphScreen(int widthX, int heightY, Spirit& spirit, PacketHandler& handler) : Screen(widthX, heightY), handler(handler)  {
        this->spirit = spirit;
    } // Pass the initial value to the base class constructor

    Graph& getGraph() {
        return graph;
    }

    GraphConfig& getConfig() {
        return config;
    }

    void display() override {

        uint8_t rawRssi = spirit.spiritLastRSSI();
        float signalStrength = (rawRssi / 2.0) - 130.0;
        canvas->fillRect(0, 0, 240, 135, ST77XX_YELLOW);
        canvas->setFont(&FreeSans9pt7b);
        canvas->fillRect(0, 0, 100, 30, ST77XX_WHITE);
        canvas->setTextColor(ST77XX_RED);
        canvas->setCursor(0, 17);
        canvas->print("Signal: ");
        canvas->println(String(signalStrength));
        battery.setBatteryLevel(lipo.cellPercent());
        battery.makeBattery(canvas);
        signalIcon.setSignalLevel(3);
        signalIcon.drawSignal(canvas);
        graph.makeLineGraph(canvas);
    }


    void update() override {
        GraphConfig ordered_config[MAX_PACKETS];
        handler.get_ordered_config(ordered_config);
        for (int i = MAX_PACKETS - 1; i >= 0 ; i--) {
            if (ordered_config[i].broadcastPeriod == 0 || ordered_config[i].pollPeriod == 0){
                Serial.println("No broadcast");
                continue;
            }
            Serial.println(ordered_config[i].broadcastPeriod / ordered_config[i].pollPeriod);
            for (int z = 0; z < (ordered_config[i].broadcastPeriod / ordered_config[i].pollPeriod); z++){
                int power = (ordered_config[i].volts[z] * ordered_config[i].amps[z]) ;
                graph.addValue(power);
            }
        }
    }
};