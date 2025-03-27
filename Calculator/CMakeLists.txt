cmake_minimum_required(VERSION 2.25)
project(CalculatorApp)

add_executable(CalculatorApp Expression.h Parser.h main.cpp tests.cpp)
