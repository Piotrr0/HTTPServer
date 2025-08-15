cmake_minimum_required(VERSION 3.31)
project(HTTPServer)

set(CMAKE_CXX_STANDARD 20)

configure_file(${CMAKE_SOURCE_DIR}/src/messages.txt ${CMAKE_BINARY_DIR}/src/messages.txt COPYONLY)

add_executable(HTTPServer src/main.cpp
        src/HTTPRequest.cpp
        src/HTTPRequest.h)

add_executable(Client src/client.cpp
        src/HTTPRequest.cpp
        src/HTTPRequest.h)