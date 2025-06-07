#include "Task.h"

bool operator<(const Task& lhs, const Task& rhs) {
    return lhs.GetTime() < rhs.GetTime();
}

bool operator>(const Task& lhs, const Task& rhs) {
    return lhs.GetTime() > rhs.GetTime();
}

Task::time_point Task::GetTime() const {
    return time_;
}

std::function<void()> Task::GetTask() const {
    return task_;
}