cmake_minimum_required(VERSION 3.0)
project(tinyserver)

add_subdirectory(deps/cJinja)
include_directories(include deps/ejson/include deps/cJinja)

FILE(GLOB SOURCE_FILES
        src/*.cpp
        src/core/*.cpp
        src/core/handler/*.cpp
        src/http/*.cpp
        src/http/http_parser/*.cpp
        src/html_template/*.cpp
        src/core/handler/*.cpp
        src/utils/*.cpp)
add_library(${PROJECT_NAME} ${SOURCE_FILES})

if(CMAKE_SYSTEM_NAME MATCHES "CYGWIN")
    target_link_libraries(${PROJECT_NAME} pthread dl libiconv.a)
elseif(APPLE)
    target_link_libraries(${PROJECT_NAME} pthread dl)
elseif(UNIX)
    target_link_libraries(${PROJECT_NAME} pthread dl)
else()
    MESSAGE("Unknown system")
endif()