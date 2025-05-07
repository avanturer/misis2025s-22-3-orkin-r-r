cmake_minimum_required(VERSION 3.10)
project(misis_image_processing)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

set(DATA_DIR "${CMAKE_SOURCE_DIR}/data")

add_subdirectory(libs/semcv)

add_subdirectory(prj.lab/lab01)
add_subdirectory(prj.lab/lab02)
add_subdirectory(prj.lab/lab03)
add_subdirectory(prj.lab/lab04)
add_subdirectory(prj.lab/lab05)
add_subdirectory(prj.lab/lab06)
add_subdirectory(prj.lab/lab07)
add_subdirectory(prj.lab/lab08)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
