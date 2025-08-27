 // =================
  //  File Name: graph.h
  //  Author: Jack Holdsworth
  //  ID: 20023933
  //  Create Time: 2024-01-25 17:38:14
  //  Modified time: 2024-04-24 12:05:00
  //  Description: a graph builder the for GFXcanvas16 libary
 // =================
#pragma once
#include <Arduino.h>
#include "../lifo_queue/lifo_queue.hpp"  // Include LifoQueue if it's used in the Graph class
#include <Adafruit_ST7789.h>  // Make sure to include or define GFXcanvas16


class Graph {
private: 
    int height;
    int width;
    int offsetX,offsetY;
    LifoQueue stack;

public:
    Graph(int widthX, int heightY, int offsetx, int offsety);
    void addValue(int value);
    GFXcanvas16 makeGraph();
    GFXcanvas16 makeLineGraph();
    void makeLineGraph(GFXcanvas16* canvas);
    LifoQueue& getStack();
};