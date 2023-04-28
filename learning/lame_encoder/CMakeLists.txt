cmake_minimum_required(VERSION 3.14)
project(Lame_Encoder)
set(CMAKE_CXX_STANDARD 11)            # Enable c++11 standard

set(CMAKE_BUILD_TYPE Debug)
set(SOURCE_FILES LameEncoder.cpp
    Mp3Encoder.h
    Mp3Encoder.cpp
)

add_executable(Lame_Encoder ${SOURCE_FILES})
target_include_directories( ${PROJECT_NAME} PRIVATE "/usr/local/include")
target_link_directories(${PROJECT_NAME} PRIVATE "/usr/local/lib")
target_link_libraries(${PROJECT_NAME} PRIVATE "mp3lame")