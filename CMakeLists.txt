cmake_minimum_required(VERSION 3.0)
project(MyWebServer)

add_subdirectory(tinyserver)
include_directories(include tinyserver/include tinyserver/deps/ejson/include)

add_executable(${PROJECT_NAME} example/main.cpp)
target_link_libraries(${PROJECT_NAME} tinyserver ejson tinytemplate)