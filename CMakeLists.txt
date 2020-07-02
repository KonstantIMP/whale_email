cmake_minimum_required(VERSION 3.5)

project(whale_email LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(CURL)

link_directories(${CURL_INCLUDE_DIRS})
link_libraries(${CURL_LIBRARIES})

add_executable(whale_email main.cpp smtp.hpp smtp.cpp)
