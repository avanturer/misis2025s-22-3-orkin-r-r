# lab5_linear_filtering CMakeLists.txt
# Linear image filtering techniques

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Add executable
add_executable(lab5_linear_filtering
    src/main.cpp
    src/lab5_linear_filtering.cpp
)

# Link libraries
target_link_libraries(lab5_linear_filtering semcv ${OpenCV_LIBS})

# Add test executable
add_executable(lab5_linear_filtering_test
    test/test_lab5_linear_filtering.cpp
    src/lab5_linear_filtering.cpp
)

target_link_libraries(lab5_linear_filtering_test semcv ${OpenCV_LIBS})

# Set output directory
set_target_properties(lab5_linear_filtering PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

set_target_properties(lab5_linear_filtering_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/test
)
