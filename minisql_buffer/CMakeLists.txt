cmake_minimum_required(VERSION 3.10)
project(minisql)

set(CMAKE_CXX_STANDARD 11)

add_executable(minisql main.cpp buffermanager.cpp buffermanager.h
                DBfile.cpp DBfile.h Attribute.cpp Attribute.h buffer_test.cpp)