# lab4_binarization_detection CMakeLists.txt
# Object detection using binarization

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Add executable
add_executable(lab4_binarization_detection
    src/main.cpp
    src/lab4_binarization_detection.cpp
)

# Link libraries
target_link_libraries(lab4_binarization_detection semcv ${OpenCV_LIBS})

# Add test executable
add_executable(lab4_binarization_detection_test
    test/test_lab4_binarization_detection.cpp
    src/lab4_binarization_detection.cpp
)

target_link_libraries(lab4_binarization_detection_test semcv ${OpenCV_LIBS})

# Set output directory
set_target_properties(lab4_binarization_detection PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

set_target_properties(lab4_binarization_detection_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/test
)
