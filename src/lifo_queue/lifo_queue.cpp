 // =================
  //  File Name: lifo_queue.cpp
  //  Author: Jack Holdsworth
  //  ID: 20023933
  //  Create Time: 2024-01-25 17:15:27
  //  Modified time: 2024-04-24 12:05:10
  //  Description: Last In First Out Queue
 // =================

#include "lifo_queue.hpp" 
#include "node.hpp"

// class Node {
// public:
//     int data; 
//     Node* next; 

//     Node(int data) : data(data), next(nullptr) {}
// };

void LifoQueue::removeBottom() {
    if (top == nullptr) return;

    Node* current = top;
    Node* prev = nullptr;

    while (current->next != nullptr) {
        prev = current;
        current = current->next;
    }

    if (prev != nullptr) {
        prev->next = nullptr;
    }

    delete current;
    size--;
}

LifoQueue::LifoQueue(int maxSize) : top(nullptr), size(0), maxSize(maxSize) {}

LifoQueue::LifoQueue(const LifoQueue& other) : top(nullptr) {
    if (other.top == nullptr) {
        return;
    }

    top = new Node(other.top->data);
    Node* current = top;
    Node* otherCurrent = other.top->next;

    while (otherCurrent != nullptr) {
        current->next = new Node(otherCurrent->data);
        current = current->next;
        otherCurrent = otherCurrent->next;
    }
}

LifoQueue::~LifoQueue() {
    while (top != nullptr) {
        Node* temp = top;
        top = top->next;
        delete temp;
    }
}

void LifoQueue::push(int data) {
    Node* newNode = new Node(data);
    newNode->next = top;
    top = newNode;
    size++;

    if (size > maxSize) {
        removeBottom();
    }
}

int LifoQueue::pop() {
    if (isEmpty()) {
        return NULL;
    }

    Node* temp = top;
    int data = top->data;
    top = top->next;
    delete temp;
    size--;
    return data;
}

bool LifoQueue::isEmpty() const {
    return top == nullptr;
}

int LifoQueue::peek() const {
    if (isEmpty()) {
        return NULL;
    }

    return top->data;
}

void LifoQueue::dumpQueueToArray(int* arr, int arrSize) const {
    if (arr == nullptr) return; // Safety check for null pointer

    Node* current = top;
    int index = 0;
    while (current != nullptr && index < arrSize) {
        arr[index++] = current->data;
        current = current->next;
    }
}
