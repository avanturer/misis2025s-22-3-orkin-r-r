# lab1_raster_formats_gamma CMakeLists.txt
# Raster formats and gamma correction

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Add executable
add_executable(lab1_raster_formats_gamma
    src/main.cpp
    src/lab1_raster_formats_gamma.cpp
)

# Link libraries
target_link_libraries(lab1_raster_formats_gamma semcv ${OpenCV_LIBS})

# Add test executable
add_executable(lab1_raster_formats_gamma_test
    test/test_lab1_raster_formats_gamma.cpp
    src/lab1_raster_formats_gamma.cpp
)

target_link_libraries(lab1_raster_formats_gamma_test semcv ${OpenCV_LIBS})

# Set output directory
set_target_properties(lab1_raster_formats_gamma PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

set_target_properties(lab1_raster_formats_gamma_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/test
)
