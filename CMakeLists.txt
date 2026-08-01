cmake_minimum_required(VERSION 3.16)

project(CN)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System)

add_executable(CN sfml.cpp)

target_link_libraries(CN PRIVATE SFML::Graphics SFML::Window SFML::System)