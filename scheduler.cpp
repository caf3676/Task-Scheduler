#include <iostream>
#include <functional>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <vector>
#include <queue>
#include <atomic>
#include <chrono>
#include "scheduler.h"
#include "task.h"
using namespace std;

// Add a new task (with optional dependencies)
// This is missing mutex stuff
void Scheduler::addTask(const Task::TaskFunction& work, const string& name, 
    const vector<int>& dependencies) {
    constexpr int INVALID_TASK_ID = -1;
    Task newTask = Task(INVALID_TASK_ID, work, name, TaskState::CREATED);
    tasks[nextTaskID] = make_shared<Task>(newTask);
    if (dependencies.size() == 0) {
        newTask.setState(TaskState::READY);
        newTask.setID(nextTaskID);
        submitTask(newTask.getID());
        nextTaskID += 1;
    }
    else {
        //Introduce logic for a waiting task
    }
        
}       

// Notify scheduler that a task finished (called by wrapper)
void Scheduler::notifyTaskCompleted(int taskID) {

}

// Start scheduling loop (pushes tasks to thread pool)
void Scheduler::start() {

}

// Wait until all tasks are completed
void Scheduler::waitAll() {

}

// Stop scheduler and thread pool
void Scheduler::stop() {

}

void Scheduler::pushReadyTask(int taskID) {

}

void Scheduler::submitTask(int taskID) {
    readyQueue.push(taskID);
}