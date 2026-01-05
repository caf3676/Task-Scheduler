#pragma once
#include <functional>
#include <vector>
#include <string>
#include <atomic>
#include "task.h"
using namespace std;

int Task::getID() const {
    return this->id;
}

void Task::setID(int newID) {
    this->id = newID;
}

TaskState Task::getState() const {
    return this->state;
}

string Task::getName() const {
    return this->name;
}

void Task::setState(TaskState newState) {
    this->state = newState;
}

using TaskFunction = function<void()>; 
TaskFunction Task::getWork() const {
    return this->work;
}