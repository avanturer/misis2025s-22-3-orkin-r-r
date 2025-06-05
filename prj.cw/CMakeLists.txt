cmake_minimum_required(VERSION 3.10)
project(ShadowSegmentation)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/shadowdetector.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${OpenCV_INCLUDE_DIRS}
    include/
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${OpenCV_LIBS}
)
