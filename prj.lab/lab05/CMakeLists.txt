project(lab05)

add_executable(lab05 lab05.cpp)

target_link_libraries(lab05 ${OpenCV_LIBS} semcv)

target_compile_definitions(lab05 PRIVATE 
    DATA_DIR="${DATA_DIR}"
)

set_target_properties(lab05 PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
