cmake_minimum_required(VERSION 3.10)
project(minisql)

set(CMAKE_CXX_STANDARD 11)

add_executable(minisql main.cpp buffermanager.cpp buffermanager.h
                DBfile.cpp DBfile.h Attribute.cpp Attribute.h  buffermanager.cpp buffermanager.h  block.cpp block.h test/testCM.cpp test/testCM.h recordmanager.cpp recordmanager.h test/block_test.cpp test/recordtest.cpp)