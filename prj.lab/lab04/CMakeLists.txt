project(lab04)

add_executable(lab04 lab04.cpp)

target_link_libraries(lab04 ${OpenCV_LIBS} semcv)

target_compile_definitions(lab04 PRIVATE 
    DATA_DIR="${DATA_DIR}"
)

set_target_properties(lab04 PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
