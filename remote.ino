// ===============
//  File: remote.ino
//  Author: Jack Holdsworth
//  Refactored: 2025-08-27
//  Description: ESP32-based remote display with SPIRIT1 radio, EEPROM config,
//               RTC sleep/wake, and screen manager.
// ===============

#include <Arduino.h>
#include <Adafruit_ST7789.h>
#include <Fonts/FreeSans9pt7b.h>
#include <SPI.h>
#include <EEPROM.h>
#include <esp_clk.h>
#include <soc/rtc.h>

#include "./src/graph/graph.hpp"
#include "./src/battery/battery.hpp"
#include "./src/spsgrf/spsgrf.hpp"
#include "./src/screens/screens.hpp"
#include "./src/screens/graph_screen.hpp"
#include "./src/screens/battery_screen.hpp"

// Constants
#define EEPROM_SIZE 3000
#define EEPROM_ADDR_BROADCAST 1000
#define EEPROM_ADDR_CONFIG 1100
#define SCREEN_WAKE_TIME 30  // seconds
#define uS_TO_MS 1000

// Globals
EEPROMClass epm_array("eeprom0");
Spirit spirit;
PacketHandler handler;
Screens manager(135, 240, 3);
GraphScreen graphScreen(240, 135, spirit, handler);
BatteryScreen batteryScreen(240, 135, spirit, handler);

RTC_DATA_ATTR int bootCount = 0;
RTC_DATA_ATTR unsigned long LAST_BROADCAST_TIME = 0;
unsigned long LAST_INTERACTION_TIME = 0;

int broadcastPeriod = 0;
int listenPeriod = 0;
int pollPeriod = 0;

const int buttonPin1 = 0;
const int buttonPin2 = 1;
const int buttonPin3 = 2;

volatile bool button1Pressed = false;
volatile bool button2Pressed = false;
volatile bool button3Pressed = false;

unsigned long now_ms() {
    return rtc_time_slowclk_to_us(rtc_time_get(), esp_clk_slowclk_cal_get()) / 1000;
}

void IRAM_ATTR handleButton1Press() { button1Pressed = true; }
void IRAM_ATTR handleButton2Press() { button2Pressed = true; }
void IRAM_ATTR handleButton3Press() { button3Pressed = true; }

void initEEPROM() {
    if (!EEPROM.begin(EEPROM_SIZE) || !epm_array.begin(EEPROM_SIZE)) {
        Serial.println("[EEPROM] Init failed. Restarting...");
        delay(1000);
        ESP.restart();
    }
}

void initButtons() {
    pinMode(buttonPin1, INPUT_PULLUP);
    pinMode(buttonPin2, INPUT_PULLDOWN);
    pinMode(buttonPin3, INPUT_PULLDOWN);
    attachInterrupt(digitalPinToInterrupt(buttonPin1), handleButton1Press, FALLING);
    attachInterrupt(digitalPinToInterrupt(buttonPin2), handleButton2Press, FALLING);
    attachInterrupt(digitalPinToInterrupt(buttonPin3), handleButton3Press, FALLING);
}

void initScreens() {
    manager.addScreen(&graphScreen);
    manager.addScreen(&batteryScreen);
    manager.screenbegin();
    manager.updateCurrentScreen();
    manager.displayCurrentScreen();
    pinMode(TFT_BACKLITE, OUTPUT);
    digitalWrite(TFT_BACKLITE, HIGH);
}

void loadOrInitConfig() {
    if (bootCount == 0) {
        // First boot - initialize EEPROM
        int zero = 0;
        for (int addr = 0; addr < 179 * sizeof(int); addr += sizeof(int)) {
            EEPROM.put(addr, zero);
        }

        handler.get_packet();
        GraphConfig* cfg = handler.get_current_config();
        broadcastPeriod = cfg->broadcastPeriod;
        listenPeriod = cfg->listenPeriod;
        pollPeriod = cfg->pollPeriod;

        EEPROM.put(EEPROM_ADDR_BROADCAST, broadcastPeriod);
        LAST_BROADCAST_TIME = now_ms();
        LAST_INTERACTION_TIME = now_ms();
        EEPROM.commit();
    } else {
        EEPROM.get(EEPROM_ADDR_BROADCAST, broadcastPeriod);
        EEPROM.get(EEPROM_ADDR_CONFIG, handler.getConfig());
        handler.set_current_packet();

        if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_TIMER) {
            handler.get_packet();
            LAST_BROADCAST_TIME = now_ms();
        } else {
            LAST_INTERACTION_TIME = now_ms();
        }
    }
    ++bootCount;
}

void handleSleep() {
    if (now_ms() - LAST_INTERACTION_TIME >= SCREEN_WAKE_TIME * 1000) {
        unsigned long timeSinceBroadcast = now_ms() - LAST_BROADCAST_TIME;
        unsigned long sleepTime = max((broadcastPeriod * 60 * 1000) - timeSinceBroadcast - 80000, 10000UL);

        esp_sleep_enable_timer_wakeup(sleepTime * uS_TO_MS);

        GraphConfig ordered[MAX_PACKETS];
        handler.get_ordered_config(ordered);
        EEPROM.put(EEPROM_ADDR_CONFIG, ordered);
        EEPROM.commit();

        esp_deep_sleep_start();
    }
}

void setup() {
    Serial.begin(115200);
    delay(300);

    initEEPROM();
    spirit.SpiritSetup();
    initScreens();
    initButtons();
    loadOrInitConfig();

    esp_sleep_enable_ext0_wakeup(GPIO_NUM_2, 1);
}

void loop() {
    if (now_ms() - LAST_BROADCAST_TIME >= broadcastPeriod * 60 * 1000 * 0.9) {
        handler.get_packet();
        LAST_BROADCAST_TIME = now_ms();
    }

    handleSleep();

    if (button1Pressed) {
        manager.nextScreen();
        LAST_INTERACTION_TIME = now_ms();
        button1Pressed = false;
    }
    if (button2Pressed) {
        LAST_INTERACTION_TIME = now_ms();
        button2Pressed = false;
    }
    if (button3Pressed) {
        LAST_INTERACTION_TIME = now_ms();
        button3Pressed = false;
    }

    manager.displayCurrentScreen();
    delay(100);
}
