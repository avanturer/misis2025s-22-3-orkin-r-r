# lab3_auto_contrast CMakeLists.txt
# Automatic contrast enhancement

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Add executable
add_executable(lab3_auto_contrast
    src/main.cpp
    src/lab3_auto_contrast.cpp
)

# Link libraries
target_link_libraries(lab3_auto_contrast semcv ${OpenCV_LIBS})

# Add test executable
add_executable(lab3_auto_contrast_test
    test/test_lab3_auto_contrast.cpp
    src/lab3_auto_contrast.cpp
)

target_link_libraries(lab3_auto_contrast_test semcv ${OpenCV_LIBS})

# Set output directory
set_target_properties(lab3_auto_contrast PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

set_target_properties(lab3_auto_contrast_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/test
)
