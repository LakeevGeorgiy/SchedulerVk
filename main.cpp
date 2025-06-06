#include <ctime>
#include "src/Scheduler.h"

std::function<void()> TaskFactory(char letter) {
    return [letter](){ std::cout << "Task: " << letter << " is completed\n"; };
}

int main() {
    Scheduler scheduler;

    scheduler.Add(TaskFactory('A'), std::time(nullptr) + 2);
    scheduler.Add(TaskFactory('B'), std::time(nullptr));
    scheduler.Add(TaskFactory('C'), std::time(nullptr) + 3);
    scheduler.Add(TaskFactory('D'), std::time(nullptr) + 3);
    scheduler.Join();

    return 0;
}