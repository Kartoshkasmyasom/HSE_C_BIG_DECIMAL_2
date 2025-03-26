cmake_minimum_required(VERSION 2.25)
project(cmake_testapp)
add_executable(cmake_testapp_file Expression.cpp Expression.h Parser.h main.cpp tests.cpp
        Parser.cpp)
