#include <iostream>
#include <functional>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <vector>
#include <queue>
#include <atomic>
#include <chrono>
using namespace std;

class ThreadPool {
public:
    void Start();
    void QueueJob(const function<void()>& job);
    void Stop();
    bool busy();


private:
    void ThreadLoop();
    // Instructs a thread to stop looking for jobs
    bool should_terminate = false;
    // Prevents race conditions
    mutex queue_mutex;
    // Ensures threads wait on new jobs or termination
    condition_variable mutex_condition;
    vector<thread> threads;
    queue<function<void()>> jobs;

};

void ThreadPool::Start() {
    // Max number of threads supported by the system
    const uint32_t num_threads = thread::hardware_concurrency();
    for (uint32_t i = 0; i < num_threads; ++i) {
        threads.emplace_back(thread(&ThreadPool::ThreadLoop, this));
    }
}

void ThreadPool::ThreadLoop() {
    while (true) {
        function<void()> job;
        {
        unique_lock lock(queue_mutex);
        // Sleep until there is work or a shutdown begins
        mutex_condition.wait(lock, [this] {return !jobs.empty() || should_terminate;});
        if (should_terminate && jobs.empty()) {
            return;
        }
        // Accesses the job at the front of the queue then removes it from the queue
        job = jobs.front();
        jobs.pop();
        }
    job();
    }
}
void ThreadPool::QueueJob(const function<void()>& job) {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
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
        active_thread.join();
    }
    threads.clear();
}

int main() {
    ThreadPool pool;
    pool.Start();
    atomic<int> counter{0};
    for (int i = 0; i < 100; ++i) {
        pool.QueueJob([i, &counter]() {
            this_thread::sleep_for(std::chrono::milliseconds(100));
            ++counter;
            std::cout << "Job " << i
                      << " executed on thread "
                      << std::this_thread::get_id() << "\n";
        });
    }
    pool.Stop();
    std::cout << "Completed jobs: " << counter << "\n";
}


    