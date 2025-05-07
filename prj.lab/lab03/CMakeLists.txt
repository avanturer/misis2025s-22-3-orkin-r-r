project(lab03)

add_executable(lab03 lab03.cpp)

target_link_libraries(lab03 ${OpenCV_LIBS} semcv)

target_compile_definitions(lab03 PRIVATE 
    DATA_DIR="${DATA_DIR}"
)

set_target_properties(lab03 PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
