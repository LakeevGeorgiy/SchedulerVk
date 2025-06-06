#pragma once

#include <atomic>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <queue>

#include "Task.h"

class ThreadPool {
public:
    ThreadPool();
    ThreadPool(unsigned int threadNum);
    ~ThreadPool();

    void Post(const std::function<void()>& job);
    void Stop();
    void Join();
    
private:
    void threadLoop();

    std::atomic<bool> shouldTerminate_ = false;
    std::mutex queueMutex_;
    std::mutex joinMutex_;
    std::condition_variable queueMutexCondition_;
    std::condition_variable joinMutexCondition_;
    std::vector<std::thread> threads_;
    std::queue<std::function<void()>> jobs_;
};