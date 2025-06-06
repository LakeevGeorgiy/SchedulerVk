project(src)

add_library(${PROJECT_NAME}
    Scheduler.h 
    Scheduler.cpp 
    Task.cpp 
    Task.h
    ThreadPool.h
    ThreadPool.cpp 
)