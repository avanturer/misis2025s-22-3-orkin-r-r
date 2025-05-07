project(lab06)

add_executable(lab06 lab06.cpp)

target_link_libraries(lab06 ${OpenCV_LIBS} semcv)

target_compile_definitions(lab06 PRIVATE 
    DATA_DIR="${DATA_DIR}"
)

set_target_properties(lab06 PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
