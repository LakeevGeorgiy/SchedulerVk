#pragma once

#include <iostream>
#include <queue>
#include <condition_variable>
#include <thread>
#include <mutex>

#include "Task.h"
#include "ThreadPool.h"

class Scheduler {
private:

    bool stop_;
    bool enough_;
    ThreadPool pool_;
    std::mutex stop_mutex_;
    std::thread main_thread_;
    std::mutex add_task_mutex_;
    std::priority_queue<Task, std::vector<Task>, std::greater<Task>> queue_;
    std::condition_variable stop_conv_;  
    std::condition_variable add_task_conv_;

public:

    Scheduler();

    template <typename ...Types>
    void Add(std::function<Types...> func, std::time_t time) {
        if (enough_) {
            throw 1;
        }
        std::unique_lock lock(stop_mutex_);
        queue_.emplace(func, time);
    }

    ~Scheduler();

    void Join();


private:

    void Execute();

};