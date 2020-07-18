cmake_minimum_required(VERSION 3.5)

project(whale_email LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(
    -Werror

    -Wall
    -Wextra
    -Wpedantic

    -Wcast-align
    -Wcast-qual
    -Wconversion
    -Wctor-dtor-privacy
    -Wenum-compare
    -Wfloat-equal
    -Wnon-virtual-dtor
    -Wold-style-cast
    -Woverloaded-virtual
    -Wredundant-decls
    -Wsign-conversion
    -Wsign-promo
)

if(NOT CMAKE_CXX_EXTENSIONS)
    set(CMAKE_CXX_EXTENSIONS OFF)
endif()

find_package(CURL)

link_directories(${CURL_INCLUDE_DIRS})
link_libraries(${CURL_LIBRARIES})

add_executable(whale_email main.cpp smtp_client.hpp smtp_client.cpp whale_email.hpp whale_email.cpp defines.hpp)
