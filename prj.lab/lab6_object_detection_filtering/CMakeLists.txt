# lab6_object_detection_filtering CMakeLists.txt
# Object detection using filtering methods

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Add executable
add_executable(lab6_object_detection_filtering
    src/main.cpp
    src/lab6_object_detection_filtering.cpp
)

# Link libraries
target_link_libraries(lab6_object_detection_filtering semcv ${OpenCV_LIBS})

# Add test executable
add_executable(lab6_object_detection_filtering_test
    test/test_lab6_object_detection_filtering.cpp
    src/lab6_object_detection_filtering.cpp
)

target_link_libraries(lab6_object_detection_filtering_test semcv ${OpenCV_LIBS})

# Set output directory
set_target_properties(lab6_object_detection_filtering PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

set_target_properties(lab6_object_detection_filtering_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/test
)
