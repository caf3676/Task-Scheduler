#pragma once
#include <functional>
#include <vector>
#include <string>
#include <atomic>
using namespace std;

enum class TaskState {
    CREATED,
    READY,
    RUNNING,
    COMPLETED
};

class Task {
public:
    using TaskFunction = function<void()>; 
    Task(int id, const TaskFunction& func, const string& name = "", const TaskState state);
    int getID() const;
    TaskState getState() const;
    void setState(const TaskState newState);
    // For debugging / logging
    string getName() const;
    // Accessor for worker execution
    TaskFunction getWork() const;
    // Dependency metadata (scheduler modifies these)
    int remainingDependencies; 
    // ID of dependent tasks       
    vector<int> dependents;       

private:
    int id;
    string name;
    TaskFunction work;
    TaskState state;
};
