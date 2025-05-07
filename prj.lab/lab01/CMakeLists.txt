project(lab01)

add_executable(lab01 lab01.cpp)

target_link_libraries(lab01 ${OpenCV_LIBS} semcv)

target_compile_definitions(lab01 PRIVATE 
    DATA_DIR="${DATA_DIR}"
)

set_target_properties(lab01 PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
