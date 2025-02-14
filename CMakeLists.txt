cmake_minimum_required(VERSION 3.10)
project(semcv)

find_package(OpenCV REQUIRED)

add_library(semcv STATIC
    src/semsv.cpp
    include/semsv.hpp
)

target_link_libraries(semcv ${OpenCV_LIBS})
