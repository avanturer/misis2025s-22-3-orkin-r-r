cmake_minimum_required(VERSION 3.10)
project(ShadowSegmentation)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find required packages
find_package(OpenCV REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

# Include directories
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Add executable
file(GLOB SOURCES "src/*.cpp")
file(GLOB HEADERS "include/*.h")

add_executable(ShadowSegmentation ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(ShadowSegmentation ${OpenCV_LIBS} Qt5::Widgets)
