cmake_minimum_required(VERSION 3.20)
project(flow_pipe_stage_example LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Build all custom stages
add_subdirectory(stages/uppercase_transform)
