cmake_minimum_required(VERSION 3.5.0)
project(SchedulerVk)

add_subdirectory(src)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC src)