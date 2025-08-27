 // =================
  //  File Name: screens.hpp
  //  Author: Jack Holdsworth
  //  ID: 20023933
  //  Create Time: 2024-03-06 14:32:10
  //  Modified time: 2024-04-24 12:06:26
  //  Description: screens data structure for arduino displays
 // =================

#pragma once
#include <Arduino.h>
#include <Adafruit_ST7789.h>
#include "screen.hpp"
 
class Screens {
private:
Adafruit_ST7789 display = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
Screen* screens[10];
int widthX, heightY, rotationR;
int currentScreenIndex = 0;
int numberOfScreens = 0;

public:

    Screens(int widthX, int heightY, int rotationR) : widthX(widthX), heightY(heightY), rotationR(rotationR){};
    
    void screenbegin() {
        Serial.print(widthX);
        Serial.print("\nheightY: ");
        Serial.print(heightY);
        display.init(widthX,heightY);
        Serial.print("screen3");
        display.setRotation(rotationR);
        Serial.print("screen4");
        
    }
    
    void addScreen(Screen* screen) {
        if (numberOfScreens < 10) {
            screens[numberOfScreens++] = screen;
        }

    }
    void nextScreen() {
        if (numberOfScreens > 0) {
            currentScreenIndex = (currentScreenIndex + 1) % numberOfScreens;
            screens[currentScreenIndex]->display();
            render(screens[currentScreenIndex]->getCanvas());
        }
    }

    void render(GFXcanvas16* canvas) {
        display.drawRGBBitmap(0, 0, canvas->getBuffer(), 240, 135);
    }

    void displayCurrentScreen() {
        if (numberOfScreens > 0) {
            // screens[currentScreenIndex]->update();
            screens[currentScreenIndex]->display();
            render(screens[currentScreenIndex]->getCanvas());
        }
    }

    void updateCurrentScreen() {
        if (numberOfScreens > 0) {
            screens[currentScreenIndex]->update();
        }
    }

    Screens& operator=(const Screens& other) {
    if (this != &other) {
        // Handle Adafruit_ST7789 copy logic, if feasible
        widthX = other.widthX;
        heightY = other.heightY;
        rotationR = other.rotationR;

        currentScreenIndex = other.currentScreenIndex;
        numberOfScreens = other.numberOfScreens;
    }
    return *this;
}
};