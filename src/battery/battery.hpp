// =================
//  File Name: battery.hpp
//  Author: Jack Holdsworth
//  ID: 20023933
//  Create Time: 2024-03-19 14:45:33
//  Modified time: 2024-03-19 14:50:07
//  Description: class for generating a battery icon with a provided value.
// =================
#pragma once
#include <Arduino.h>
#include "../lifo_queue/lifo_queue.hpp" // Include LifoQueue if it's used in the Graph class
#include <Adafruit_ST7789.h>            // Make sure to include or define GFXcanvas16

class Battery
{
private:
    int height;
    int width;
    int offsetX, offsetY;
    int batteryLevel;

public:
    Battery(int widthX, int heightY, int offsetx, int offsety) : height(heightY), width(widthX), offsetX(offsetx), offsetY(offsety)
    {
        batteryLevel = 50;
    }
    void setBatteryLevel(int value)
    {
        if (value < 0){
            batteryLevel = 0; // Set to minimum if below 0
        } else if (value > 100) {
            batteryLevel = 100; // Set to maximum if above 100
        } else {
            batteryLevel = value; // Keep the value as is if it's within the range
        }
    }
    GFXcanvas16 makeBattery();
    void makeBattery(GFXcanvas16 *canvas)
    {
        // Adjust the dimensions and position according to the class attributes
        canvas->drawRect(offsetX, offsetY, width, height, ST77XX_WHITE);                        // Draw the battery outline
        canvas->fillRect(offsetX + width, offsetY + (height / 4), width / 15, height / 2, ST77XX_WHITE); // Battery tip

        // Calculate the width of the battery's filled portion inside the outline
        int fillableWidth = width - 8;                        // Assuming 4 pixels padding on each side
        int fillWidth = (batteryLevel * fillableWidth) / 100; // Calculate fill width based on battery level

        // Set the fill color based on the battery level
        uint16_t fillColor;
        if (batteryLevel < 20) {
            fillColor = ST77XX_RED; // Red for low battery
        } else if (batteryLevel < 50) {
            fillColor = ST77XX_YELLOW; // Yellow for medium battery
        } else {
            fillColor = ST77XX_GREEN; // Green for high battery
        }

        // Draw the filled portion inside the battery outline
        
        if (height >= 50) {
            canvas->fillRect(offsetX + 4, offsetY + 2, fillWidth, height - 4, fillColor);
            // Display the value in the center of the battery
            String batteryLevelText = String(batteryLevel) + "%";
            int16_t textX = offsetX + width / 2; // Center X position of the text
            int16_t textY = offsetY + height / 2; // Center Y position of the text
            canvas->setTextColor(ST77XX_WHITE); // Choose a text color that will stand out
            canvas->setTextSize(1); // You may need to adjust the text size based on your display's capabilities
            // Center the text both horizontally and vertically
            int16_t x1, y1;
            uint16_t w, h;
            canvas->getTextBounds(batteryLevelText, 0, 0, &x1, &y1, &w, &h); // Calculate text bounds (may vary by library)
            textX -= w / 2;
            textY = textY;

            canvas->setCursor(textX, textY);
            canvas->print(batteryLevelText);
        } else {
            canvas->fillRect(offsetX + 1, offsetY + 1, fillWidth, height - 2, fillColor);
        }
    }
};
