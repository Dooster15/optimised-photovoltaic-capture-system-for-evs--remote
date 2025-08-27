// =================
  //  File Name: lifo_queue.h
  //  Author: Jack Holdsworth
  //  ID: 20023933
  //  Create Time: 2024-01-25 16:33:33
  //  Modified time: 2024-04-24 12:05:13
  //  Description: Last In First Out Queue
 // =================

#pragma once 
#include <Arduino.h>
#include "node.hpp"
class LifoQueue {
private:
    Node* top;
    int size; // current size
    int maxSize; // size before items get knocked off

    
    void removeBottom();

public:
    LifoQueue(int maxSize);
    LifoQueue(const LifoQueue& other); // deep copy constructor
    ~LifoQueue(); // Destructor 

    void push(int data);
    int pop();
    bool isEmpty() const;
    int peek() const;
    void dumpQueueToArray(int* arr, int arrSize) const;
};
