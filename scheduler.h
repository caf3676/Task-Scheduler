#pragma once
#include "task.h"
#include <queue>
#include <unordered_map>
#include <mutex>
#include <condition_variable>
#include "threadpool.h"
using namespace std;

class Scheduler {
public:
    Scheduler(ThreadPool& pool);

    // Add a new task (with optional dependencies)
    void addTask(const Task::TaskFunction& work, 
                 const string& name = "", 
                 const vector<int>& dependencies = {});

    // Notify scheduler that a task finished (called by wrapper)
    void notifyTaskCompleted(int taskID);

    // Start scheduling loop (pushes tasks to thread pool)
    void start();

    // Wait until all tasks are completed
    void waitAll();

    // Stop scheduler and thread pool
    void stop();

private:
    ThreadPool& pool;                         // reference to your thread pool
    unordered_map<int, Task> tasks;      // all tasks
    queue<int> readyQueue;               // IDs of READY tasks

    mutex mtx;                            // protects scheduler state
    condition_variable cv;               // worker signals task completion

    int nextTaskID = 0;                       // generates unique IDs

    // Internal helpers
    void pushReadyTask(int taskID);           // mark task READY and submit wrapper
    void submitTask(int taskID);              // wrap task and push to ThreadPool
};
