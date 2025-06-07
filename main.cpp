#include <ctime>

#include "src/Scheduler.h"
#include "src/ThreadPool.h"

std::function<void()> TaskFactory(char letter) {
    return [letter](){ std::cout << "Task: " << letter << " is completed\n"; };
}

int main() {
    auto thread_pool = std::make_unique<ThreadPool>(std::thread::hardware_concurrency());
    Scheduler<ThreadPool> scheduler(std::move(thread_pool));

    scheduler.Add(TaskFactory('A'), std::time(nullptr) + 1);
    // scheduler.Add(TaskFactory('B'), std::time(nullptr));
    // scheduler.Add(TaskFactory('C'), std::time(nullptr) + 3);
    // scheduler.Add(TaskFactory('D'), std::time(nullptr) + 3);
    scheduler.Join();

    return 0;
}