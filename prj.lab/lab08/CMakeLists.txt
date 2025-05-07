project(lab08)

add_executable(lab08 lab08.cpp)

target_link_libraries(lab08 ${OpenCV_LIBS} semcv)

target_compile_definitions(lab08 PRIVATE 
    DATA_DIR="${DATA_DIR}"
)

set_target_properties(lab08 PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
