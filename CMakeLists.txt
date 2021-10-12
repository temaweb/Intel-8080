cmake_minimum_required(VERSION 3.10)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# set the project name
project(8080 VERSION 1.0)

# allow CPU use OUT instuction for logging purpose
add_definitions(-DLOGTEST)

# add the executable
add_library(8080 SHARED "src/cpu.cpp")

# add library sources
target_sources(8080 PRIVATE 
    "src/asmlog.cpp"
    "src/command.cpp"
    "src/status.cpp")

# create example target
add_executable(example "src/example.cpp")

# add output folder as link directory
target_link_directories(example PUBLIC "${PROJECT_BINARY_DIR}")

# link 8080 library
target_link_libraries(example 8080)