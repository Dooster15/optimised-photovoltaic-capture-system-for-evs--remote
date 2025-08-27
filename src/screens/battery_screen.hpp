// =================
  //  File Name: graph_screen.hpp
  //  Author: Jack Holdsworth
  //  ID: 20023933
  //  Create Time: 2024-03-06 15:57:25
  //  Modified time: 2024-04-24 12:05:35
  //  Description: 
 // ================= 
#pragma once
#include <Arduino.h>
#include "screen.hpp"
#include <Fonts/FreeSans9pt7b.h>
#include "../battery/battery.hpp"
#include "../signal/signal.hpp"
#include "../spsgrf/spsgrf.hpp"
#include "../spsgrf/packet_handler.hpp"
class BatteryScreen : public Screen {
private:
    Battery battery = Battery(180, 80, 50, 50);
    Signal signalIcon = Signal(180, 13, 20);
    Spirit spirit;
    PacketHandler& handler;
public:
    BatteryScreen(int widthX, int heightY, Spirit& spirit, PacketHandler& handler) : Screen(widthX, heightY), handler(handler) {
        this->spirit = spirit;
    } // Pass the initial value to the base class constructor

    void display() override {
        uint8_t rawRssi = spirit.spiritLastRSSI();
        float signalStrength = (rawRssi/2.0)-130.0;
        canvas->fillRect(0,0, 240 , 135, ST77XX_BLACK);
        canvas->setFont(&FreeSans9pt7b);
        canvas->fillRect(0,0,100 , 30, ST77XX_WHITE);
        canvas->setTextColor(ST77XX_RED);
        canvas->setCursor(0, 17);
        canvas->print("Signal:");
        canvas->println(String(signalStrength));
        battery.setBatteryLevel(handler.get_current_config()->volts[0]);
        battery.makeBattery(canvas);
        signalIcon.setSignalLevel(4);
        signalIcon.drawSignal(canvas);
    }

    void update() override {
        // Implementation specific to Screen1
    }
};