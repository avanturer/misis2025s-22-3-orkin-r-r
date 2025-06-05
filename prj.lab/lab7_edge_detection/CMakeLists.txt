# lab7_edge_detection CMakeLists.txt
# Edge detection algorithms

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Add executable
add_executable(lab7_edge_detection
    src/main.cpp
    src/lab7_edge_detection.cpp
)

# Link libraries
target_link_libraries(lab7_edge_detection semcv ${OpenCV_LIBS})

# Add test executable
add_executable(lab7_edge_detection_test
    test/test_lab7_edge_detection.cpp
    src/lab7_edge_detection.cpp
)

target_link_libraries(lab7_edge_detection_test semcv ${OpenCV_LIBS})

# Set output directory
set_target_properties(lab7_edge_detection PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

set_target_properties(lab7_edge_detection_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/test
)
