#include <iostream>

#include "ThreadPool.h"

ThreadPool::ThreadPool()
    : ThreadPool(std::thread::hardware_concurrency()) 
{}

ThreadPool::ThreadPool(unsigned int threadNum) {
    for (unsigned int i = 0; i < threadNum; ++i) {
        threads_.emplace_back(&ThreadPool::threadLoop,this);
    }
}

ThreadPool::ThreadPool(ThreadPool &&other)
    : jobs_(std::move(other.jobs_))
{
    for (size_t i = 0; i < threads_.size(); ++i) {
        threads_[i] = std::thread(&ThreadPool::threadLoop, this);
    }
}

ThreadPool::~ThreadPool() {
    Stop();
}

void ThreadPool::threadLoop() {
    while (true) {
        std::function<void()> job;

        {
            std::unique_lock<std::mutex> lock(queueMutex_);
            if (jobs_.empty()) {
                joinMutexCondition_.notify_all();
            }
            queueMutexCondition_.wait(lock, [this] {;
                return !jobs_.empty() || shouldTerminate_;
            });
            if (shouldTerminate_) {
                return;
            }
            job = jobs_.front();
            jobs_.pop();
        }

        job();
    }
}

void ThreadPool::Post(const std::function<void()>&  job) {
    {
        std::unique_lock<std::mutex> lock(queueMutex_);
        jobs_.push(job);
    }
    
    queueMutexCondition_.notify_one();
}

void ThreadPool::Stop() {
    shouldTerminate_ = true;

    queueMutexCondition_.notify_all();
    for (std::thread& active_thread : threads_) {
        active_thread.join();
    }
    threads_.clear();
}

void ThreadPool::Join() {
    std::unique_lock<std::mutex> lock(joinMutex_);
    joinMutexCondition_.wait(lock, [this] {
        return jobs_.empty() || shouldTerminate_;
    });
}