project(lab02)

add_executable(lab02 lab02.cpp)

target_link_libraries(lab02 ${OpenCV_LIBS} semcv)

target_compile_definitions(lab02 PRIVATE 
    DATA_DIR="${DATA_DIR}"
)

set_target_properties(lab02 PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
