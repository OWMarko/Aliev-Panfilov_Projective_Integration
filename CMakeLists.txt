cmake_minimum_required(VERSION 3.16)
project(Cardiac LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(cardiac
    app/main.cpp
    src/laplacian.cpp
    src/model.cpp
    src/pfe.cpp
    src/simulation.cpp
    src/output.cpp
)

target_include_directories(cardiac PRIVATE include)

if(MSVC)
    target_compile_options(cardiac PRIVATE /W4 /utf-8)
else()
    target_compile_options(cardiac PRIVATE -Wall -Wextra -Wpedantic)
endif()
