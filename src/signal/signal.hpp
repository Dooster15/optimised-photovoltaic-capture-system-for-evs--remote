#pragma once
#include <Arduino.h>
#include <Adafruit_ST7789.h> // Assuming the same display library is used

class Signal {
private:
    int centerX, centerY; // Center position of the WiFi icon
    int maxRadius;        // Maximum radius of the WiFi signal arcs
    int signalLevel;      // Signal level, ranging from 0 to 2

public:
    Signal(int centerX, int centerY, int maxRadius) 
    : centerX(centerX), centerY(centerY), maxRadius(maxRadius), signalLevel(1) {
        // Default signal level is set to 1 (medium)
    }

    void setSignalLevel(int value) {
        signalLevel = (value >= 0 && value <= 4) ? value : signalLevel;
    }

    void drawSignal(GFXcanvas16 *canvas) {
        // Draw a dot for all levels except 0
        if (signalLevel >= 1) {
            canvas->fillCircle(centerX, centerY, maxRadius / 15, ST77XX_WHITE); // Adjust dot size as needed
        }

        if (signalLevel > 1) {
            int lineSpacing = maxRadius / 5; // Adjust spacing between arcs
            int startRadius = maxRadius / 15 + lineSpacing; // Start drawing arcs outside the dot

            // Start from level 2 since level 1 is just a dot
            for (int level = 2; level <= signalLevel; ++level) {
                int radius = startRadius + lineSpacing * (level - 2);
                drawSemiCircle(canvas, centerX, centerY, radius);
            }
        }
    }

private:
    void drawSemiCircle(GFXcanvas16 *canvas, int x, int y, int radius) {
        // Line thickness factor, determines how many lines to draw for thickness
        int thickness = 3; // Increase for thicker lines
        for (double angle = -M_PI / 4; angle <= M_PI / 4; angle += 0.01) { // Finer step for smoothness
            for (int t = -thickness / 2; t <= thickness / 2; ++t) {
                // Calculate the offset for the thickness
                double offsetX = t * cos(angle + M_PI / 2);
                double offsetY = t * sin(angle + M_PI / 2);

                int x0 = x + (radius + offsetX) * cos(angle);
                int y0 = y - (radius + offsetY) * sin(angle);
                int x1 = x + (radius + offsetX) * cos(angle + 0.01);
                int y1 = y - (radius + offsetY) * sin(angle + 0.01);
                canvas->drawLine(x0, y0, x1, y1, ST77XX_WHITE);
            }
        }
    }
};