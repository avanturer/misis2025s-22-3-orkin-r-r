project(lab07)

add_executable(lab07 lab07.cpp)

target_link_libraries(lab07 ${OpenCV_LIBS} semcv)

target_compile_definitions(lab07 PRIVATE 
    DATA_DIR="${DATA_DIR}"
)

set_target_properties(lab07 PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
