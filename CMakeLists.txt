cmake_minimum_required(VERSION 3.10)
project(misis2025s-3-improc)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/semcv/include)

add_subdirectory(semcv)
add_subdirectory(prj.lab)

message(STATUS "OpenCV version: ${OpenCV_VERSION}")
message(STATUS "OpenCV include dirs: ${OpenCV_INCLUDE_DIRS}")
message(STATUS "OpenCV libs: ${OpenCV_LIBS}")


enable_testing()

add_test(NAME lab1_test COMMAND lab1_raster_formats_gamma_test)
add_test(NAME lab2_test COMMAND lab2_image_noise_test)
add_test(NAME lab3_test COMMAND lab3_auto_contrast_test)
add_test(NAME lab4_test COMMAND lab4_binarization_detection_test)
add_test(NAME lab5_test COMMAND lab5_linear_filtering_test)
add_test(NAME lab6_test COMMAND lab6_object_detection_filtering_test)
add_test(NAME lab7_test COMMAND lab7_edge_detection_test)

add_test(NAME semcv_test COMMAND semcv_test)
