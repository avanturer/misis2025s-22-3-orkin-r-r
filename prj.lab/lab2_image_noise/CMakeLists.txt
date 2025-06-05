# lab2_image_noise CMakeLists.txt
# Image noise addition and filtering

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Add executable
add_executable(lab2_image_noise
    src/main.cpp
    src/lab2_image_noise.cpp
)

# Link libraries
target_link_libraries(lab2_image_noise semcv ${OpenCV_LIBS})

# Add test executable
add_executable(lab2_image_noise_test
    test/test_lab2_image_noise.cpp
    src/lab2_image_noise.cpp
)

target_link_libraries(lab2_image_noise_test semcv ${OpenCV_LIBS})

# Set output directory
set_target_properties(lab2_image_noise PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

set_target_properties(lab2_image_noise_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/test
)
