project(ComputerClubYadroTests)
include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.16.0
)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
    ${PROJECT_NAME}
    SchedulerTests.cpp
)

target_link_libraries(${PROJECT_NAME} 
        PUBLIC 
            GTest::gtest_main
        PUBLIC
            src
)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})

include(GoogleTest)

gtest_discover_tests(${PROJECT_NAME})