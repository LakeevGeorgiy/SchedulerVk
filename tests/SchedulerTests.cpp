#include <gtest/gtest.h>
#include <cmath>

#include "../src/Scheduler.h"
#include "../src/ThreadPool.h"

std::function<void()> TaskFactory(char letter) {
    return [letter](){ std::cout << "Task: " << letter << "\n"; };
}

class SchedulerTests : public testing::Test {
public:

    std::unique_ptr<ThreadPool> pool_;
    Scheduler<ThreadPool> scheduler_;

public:

    SchedulerTests()
        : pool_(std::make_unique<ThreadPool>(std::thread::hardware_concurrency()))
        , scheduler_(std::move(pool_))
    {}

protected:

    void SetUp() override {
        testing::internal::CaptureStdout();
    }

    std::string GetOutput() {
        return testing::internal::GetCapturedStdout();
    }
};

TEST_F(SchedulerTests, OneTask) {

    auto task_a = TaskFactory('A');
    scheduler_.Add(task_a, std::time(nullptr));
    scheduler_.Join();
    
    auto output = GetOutput();
    ASSERT_EQ(output, "Task: A\n");

}

TEST_F(SchedulerTests, OneTaskWithDelay) {

    auto start_time = std::chrono::system_clock::now();
    auto task_time = std::time(nullptr) + 2;

    auto task_a = TaskFactory('A');
    scheduler_.Add(task_a, task_time);
    scheduler_.Join();
    
    auto end_time = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
    
    auto output = GetOutput();
    ASSERT_EQ(output, "Task: A\n");
    ASSERT_TRUE(duration > 1000);

}

TEST_F(SchedulerTests, TwoTaskCompleteAtOneTime) {

    scheduler_.Add(TaskFactory('A'), std::time(nullptr));
    scheduler_.Add(TaskFactory('B'), std::time(nullptr));
    scheduler_.Join();
    
    auto output = GetOutput();
    ASSERT_TRUE(output == "Task: B\nTask: A\n" || output == "Task: A\nTask: B\n");

}

TEST_F(SchedulerTests, TwoTaskInOrder) {

    auto start_time = std::chrono::system_clock::now();

    scheduler_.Add(TaskFactory('A'), std::time(nullptr));
    scheduler_.Add(TaskFactory('B'), std::time(nullptr) + 1);
    scheduler_.Join();
    
    auto end_time = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
    
    auto output = GetOutput();
    ASSERT_EQ(output, "Task: A\nTask: B\n");
    ASSERT_TRUE(duration > 100);

}

TEST_F(SchedulerTests, TwoTaskInBackwardOrder) {

    auto start_time = std::chrono::system_clock::now();

    scheduler_.Add(TaskFactory('A'), std::time(nullptr) + 2);
    scheduler_.Add(TaskFactory('B'), std::time(nullptr) + 1);
    scheduler_.Join();
    
    auto end_time = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
    
    auto output = GetOutput();
    ASSERT_EQ(output, "Task: B\nTask: A\n");
    ASSERT_TRUE(duration > 1000);

}

TEST_F(SchedulerTests, SeveralTask) {

    auto start_time = std::chrono::system_clock::now();

    scheduler_.Add(TaskFactory('A'), std::time(nullptr) + 2);
    scheduler_.Add(TaskFactory('B'), std::time(nullptr) + 1);
    scheduler_.Add(TaskFactory('C'), std::time(nullptr) + 2);
    scheduler_.Add(TaskFactory('D'), std::time(nullptr) + 3);
    scheduler_.Join();
    
    auto end_time = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
    
    auto output = GetOutput();
    ASSERT_TRUE(output == "Task: B\nTask: A\nTask: C\nTask: D\n" || output == "Task: B\nTask: C\nTask: A\nTask: D\n");
    ASSERT_TRUE(duration > 2000);

}