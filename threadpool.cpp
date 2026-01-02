#include "threadpool.h"
#include <iostream>
using namespace std;

void ThreadPool::Start() {
    const uint32_t num_threads = thread::hardware_concurrency();
    for (uint32_t i = 0; i < num_threads; ++i) {
        threads.emplace_back(&ThreadPool::ThreadLoop, this);
    }
}

void ThreadPool::ThreadLoop() {
    while (true) {
        function<void()> job;
        {
            unique_lock lock(queue_mutex);
            mutex_condition.wait(lock, [this] { return !jobs.empty() || should_terminate; });
            if (should_terminate && jobs.empty()) {
                return;
            }
            job = jobs.front();
            jobs.pop();
        }
        job();
    }
}

void ThreadPool::QueueJob(const function<void()>& job) {
    {
        unique_lock lock(queue_mutex);
        jobs.push(job);
    }
    mutex_condition.notify_one();
}

bool ThreadPool::busy() {
    bool poolbusy;
    {
        unique_lock lock(queue_mutex);
        poolbusy = !jobs.empty();
    }
    return poolbusy;
}

void ThreadPool::Stop() {
    {
        unique_lock lock(queue_mutex);
        should_terminate = true;
        mutex_condition.notify_all();
    }
    for (thread& active_thread : threads) {
        if (active_thread.joinable()) {
            active_thread.join();
        }
    }
    threads.clear();
}

ThreadPool::~ThreadPool() {
    Stop();
}
