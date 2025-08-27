 // =================
  //  File Name: graph.cpp
  //  Author: Jack Holdsworth
  //  ID: 20023933
  //  Create Time: 2024-01-25 17:38:07
  //  Modified time: 2024-04-24 12:04:51
  //  Description: a graph builder the for GFXcanvas16 libary
 // =================

#include "graph.hpp" 

Graph::Graph(int widthX, int heightY, int offsetx, int offsety) : stack(widthX) {
    height = heightY;
    width = widthX;
    offsetX = offsetx;
    offsetY = offsety;
}

void Graph::addValue(int value) {
    if (value <= height) {
        stack.push(value);
    }
}

GFXcanvas16 Graph::makeGraph() {
    LifoQueue stackCopy = stack;
    GFXcanvas16 canvas(width, height);
    int i = 0;
    while (!stackCopy.isEmpty()) {
        int value = stackCopy.pop();
        canvas.drawLine(width-i, height, width-i, (height-value), 0xF800);
        i++;
    }
    return canvas;
}

GFXcanvas16 Graph::makeLineGraph() {
    LifoQueue stackCopy = stack; 
    GFXcanvas16 canvas(width, height);
    
    canvas.drawLine(width-10,height,width-10,0,0xFFFF);
    canvas.drawLine(width-40,height,width-40,0,0xFFFF);
    canvas.drawLine(width-70,height,width-70,0,0xFFFF);
    canvas.drawLine(width-100,height,width-100,0,0xFFFF);
    canvas.drawLine(width-130,height,width-130,0,0xFFFF);
    canvas.drawLine(width-160,height,width-160,0,0xFFFF);
    Serial.println("here95");
    int i = 10;
    while (!stackCopy.isEmpty()) {
        int valuePrev = stackCopy.pop();
        if (!stackCopy.isEmpty()){
            int valuePres = stackCopy.peek();
            canvas.drawLine(width-i, (height-valuePrev), width-i-10, (height-valuePres), 0xF800);
        }
        i += 10;
    }
    return canvas;
}

void Graph::makeLineGraph(GFXcanvas16* canvas) {
    LifoQueue stackCopy = stack; 
    // GFXcanvas16 canvas(width, height);
    
    canvas->drawLine(width-10 + offsetX,height + offsetY,width-10 + offsetX,0 + offsetY,0xFFFF);
    canvas->drawLine(width-40 + offsetX,height + offsetY,width-40 + offsetX,0 + offsetY,0xFFFF);
    canvas->drawLine(width-70 + offsetX,height + offsetY,width-70 + offsetX,0 + offsetY,0xFFFF);
    canvas->drawLine(width-100 + offsetX,height + offsetY,width-100 + offsetX,0 + offsetY,0xFFFF);
    canvas->drawLine(width-130 + offsetX,height + offsetY,width-130 + offsetX,0 + offsetY,0xFFFF);
    canvas->drawLine(width-160 + offsetX,height + offsetY,width-160 + offsetX,0 + offsetY,0xFFFF);
    // Serial.println("here95");
    int i = 10;
    while (!stackCopy.isEmpty()) {
        int valuePrev = stackCopy.pop();
        if (!stackCopy.isEmpty()){
            int valuePres = stackCopy.peek();
            canvas->drawLine(width-i + offsetX, (height-valuePrev + offsetY), width-i-10 + offsetX, (height-valuePres + offsetY), 0xF800);
        }
        i += 10;
        // Serial.println(i);
    }
    // Serial.println("here100");
    
}

LifoQueue& Graph::getStack() {
    return stack;
}
