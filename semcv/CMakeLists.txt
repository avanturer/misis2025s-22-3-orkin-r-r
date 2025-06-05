set(SEMCV_SOURCES
    src/gamma_correction.cpp
    src/noise_generation.cpp
    src/auto_contrast.cpp
    src/binarization.cpp
    src/linear_filtering.cpp
    src/object_detection.cpp
    src/edge_detection.cpp
    src/utility_functions.cpp
    src/test_functions.cpp
)

add_library(semcv STATIC ${SEMCV_SOURCES})

target_include_directories(semcv PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(semcv ${OpenCV_LIBS})

add_executable(semcv_test src/test_main.cpp)
target_link_libraries(semcv_test semcv ${OpenCV_LIBS})

set_target_properties(semcv_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/test
)
