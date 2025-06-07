cmake_minimum_required(VERSION 3.5.0)
project(SchedulerVk)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(src)
add_subdirectory(tests)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC src)
enable_testing()