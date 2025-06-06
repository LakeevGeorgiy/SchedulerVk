#pragma once

#include <functional>
#include <ctime>
#include <chrono>

class Task {
public:

    using time_point = std::chrono::time_point<std::chrono::system_clock>;

private:
    
    time_point time_;
    std::function<void()> task_;

public:

    template<typename ...Types>
    Task(std::function<Types...> func, std::time_t time): time_(std::chrono::system_clock::from_time_t(time)) {
        task_= [f = std::move(func)](){
            f();
        };
    }

    time_point GetTime() const {
        return time_;
    }

    std::function<void()> GetTask() const {
        return task_;
    }
    
};

bool operator<(const Task& lhs, const Task& rhs);
bool operator>(const Task& lhs, const Task& rhs);

template<>
struct std::greater<Task> {
    bool operator()(const Task& lhs, const Task& rhs) const {
        return lhs > rhs; 
    }
};