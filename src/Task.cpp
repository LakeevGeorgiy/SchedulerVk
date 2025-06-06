#include "Task.h"

bool operator<(const Task& lhs, const Task& rhs) {
    return lhs.GetTime() < rhs.GetTime();
}

bool operator>(const Task& lhs, const Task& rhs) {
    return lhs.GetTime() > rhs.GetTime();
}