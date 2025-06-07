#pragma once

#include <iostream>
#include <queue>
#include <condition_variable>
#include <thread>
#include <mutex>
#include <memory>

#include "Task.h"

template <typename T>
concept ThreadPoolConcept = requires(T pool, const std::function<void()>& arg) {
    pool.Post(arg);
    pool.Join();
    pool.Stop();
};

template <ThreadPoolConcept ThreadPool>
class Scheduler {
private:

    bool stop_;
    bool enough_;
    std::unique_ptr<ThreadPool> pool_;
    std::mutex mutex_;
    std::thread main_thread_;
    std::priority_queue<Task, std::vector<Task>, std::greater<Task>> queue_;

public:

    Scheduler(std::unique_ptr<ThreadPool>&& thread_pool)
        : pool_(std::move(thread_pool))
        , main_thread_(&Scheduler::Execute, this)
        , stop_(false)
        , enough_(false)
        , queue_()
    {}  

    template <typename ...Types>
    void Add(std::function<Types...> func, std::time_t time) {
        if (enough_) {
            throw std::runtime_error("Can't add task after calling join method");
        }
        std::unique_lock lock(mutex_);
        queue_.emplace(func, time);
    }

    ~Scheduler() = default;

    void Join(){
        enough_ = true;
        if (main_thread_.joinable()) {
            main_thread_.join();
        }
        pool_->Join();
        pool_->Stop();
    }


private:

    void Execute() {
        while (!stop_) {

            std::unique_lock<std::mutex> lock(mutex_);
            
            if (queue_.empty()) {
                continue;
            }

            auto task = queue_.top();
            auto now = std::chrono::system_clock::now();

            if (task.GetTime() > now) {
                continue;
            }

            queue_.pop();
            
            if (enough_ && queue_.empty()) {
                stop_ = true;
            }

            lock.unlock();
            
            try {
                pool_->Post(task.GetTask());
            } catch (const std::exception& ex) {
                std::cerr << "Task error: " << ex.what() << "\n";
            }
        }
    }

};