#pragma once
#include <functional>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <vector>
#include <queue>
using namespace std;

class ThreadPool {
public:
    ThreadPool() = default;
    ~ThreadPool();
    // Start the threads in the pool
    void Start();
    // Queue a job to be executed by a worker thread
    void QueueJob(const function<void()>& job);
    // Stop all threads and wait for completion
    void Stop();
    // Check if the pool has pending jobs
    bool busy();

private:
    void ThreadLoop();

    // Flag to tell threads to stop
    bool should_terminate = false;
    // Mutex to protect the job queue and state
    mutex queue_mutex;
    // Condition variable to wake up sleeping threads
    condition_variable mutex_condition;
    // The pool of threads
    vector<thread> threads;
    // Queue of jobs to be executed
    queue<function<void()>> jobs;
};
