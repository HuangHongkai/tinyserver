cmake_minimum_required(VERSION 3.0)
project(tinytemplate)

add_subdirectory(deps/ejson)
include_directories(deps/ejson/include)

add_library(${PROJECT_NAME} cjinja.cpp)