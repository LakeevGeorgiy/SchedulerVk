#include "Scheduler.h"

Scheduler::Scheduler()
    : pool_(std::thread::hardware_concurrency())
    , main_thread_(&Scheduler::Execute, this)
    , stop_(false)
    , queue_() {}   

Scheduler::~Scheduler() {
    stop_ = true;
    stop_conv_.notify_one();
    if (main_thread_.joinable()) {
        main_thread_.join();
    }
}

void Scheduler::Execute() {
    while (!stop_) {

        std::unique_lock<std::mutex> lock(stop_mutex_);
        
        // std::cout << "begin wait\n";
        if (queue_.empty()) {
            // std::unique_lock<std::mutex> add_lock(add_task_mutex_);
            // add_task_conv_.wait(add_lock, [this](){
            //     return  stop_ || !queue_.empty();
            // });
            continue;
        }
        // std::cout << "end wait\n";

        auto next_time = queue_.top().GetTime();
        auto now = std::chrono::system_clock::now();

        // auto next_time_mil = std::chrono::duration_cast<std::chrono::milliseconds>(next_time.time_since_epoch());
        // auto now_mil = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
        // std::cout << "time: " << next_time_mil.count() << " " << now_mil.count() << "\n";

        if (next_time > now) {
            continue;
        }

        auto task = queue_.top().GetTask();
        queue_.pop();
        
        if (enough_ && queue_.empty()) {
            stop_ = true;
        }

        lock.unlock();
        
        try {
            task();
            // pool_.Post(task);
        } catch (const std::exception& ex) {
            std::cerr << "Task error: " << ex.what() << "\n";
        }
    }
}

void Scheduler::Join(){
    enough_ = true;
    if (main_thread_.joinable()) {
        pool_.Join();
        main_thread_.join();
    }
}