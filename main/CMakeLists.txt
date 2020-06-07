cmake_minimum_required(VERSION 3.16)
project(main)

set(CMAKE_CXX_STANDARD 14)

find_package( OpenCV REQUIRED )
set(SOURCE_FILES main.cpp source/histo.cpp headers/histo.h source/imopen.cpp headers/imopen.h headers/hitmiss.h source/hitmiss.cpp headers/strel.h source/strel.cpp headers/entropyfilt.h source/entropyfilt.cpp)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS} )