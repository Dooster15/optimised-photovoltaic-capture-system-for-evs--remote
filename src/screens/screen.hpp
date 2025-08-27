 // =================
  //  File Name:
  //  Author: Jack Holdsworth
  //  ID: 20023933
  //  Create Time: 2024-03-06 16:03:29
  //  Modified time: 2024-04-24 12:06:20
  //  Description:
 // =================
#pragma once
#include <Arduino.h>
#include <Adafruit_ST7789.h>
 
class Screen {
protected:
    
public: 
    GFXcanvas16* canvas;
    Screen(int widthX, int heightY) {
        canvas = new GFXcanvas16(widthX, heightY);
    }  
    virtual void display() = 0; // Pure virtual function for displaying the screen
    virtual void update() = 0;  // Pure virtual function for updating the screen's data
    
    // Getter for the canvas
    GFXcanvas16* getCanvas() {
        return canvas;
    }

    // If you want to provide read-only access, you can return a const reference
    // const GFXcanvas16& getCanvas() const {
    //     return canvas;
    // }
    ~Screen() {
        delete canvas;
    }
};