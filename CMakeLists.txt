cmake_minimum_required(VERSION 3.10)
project(image_processing)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Поиск OpenCV
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# Добавление semcv
add_subdirectory(libs/semcv)

# Добавление лабораторных работ
add_subdirectory(prj.lab/lab01)
add_subdirectory(prj.lab/lab02)
add_subdirectory(prj.lab/lab03)
add_subdirectory(prj.lab/lab04)
add_subdirectory(prj.lab/lab05)
add_subdirectory(prj.lab/lab06)
add_subdirectory(prj.lab/lab07)
add_subdirectory(prj.lab/lab08)
