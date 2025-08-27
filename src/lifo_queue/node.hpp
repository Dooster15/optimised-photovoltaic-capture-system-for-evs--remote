 // =================
  //  File Name: node.h
  //  Author: Jack Holdsworth
  //  ID: 20023933
  //  Create Time: 2024-01-25 17:25:30
  //  Modified time: 2024-04-24 12:05:27
  //  Description: Holds data for lifo
 // =================
#pragma once
#include <Arduino.h>
class Node { 
public:
    int data;
    Node* next;

    Node(int data);
};
