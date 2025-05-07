project(lab02)

add_executable(task02 task02.cpp)
target_link_libraries(task02 ${OpenCV_LIBS} semcv)
target_compile_definitions(task02 PRIVATE DATA_DIR="${DATA_DIR}")
set_target_properties(task02 PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")