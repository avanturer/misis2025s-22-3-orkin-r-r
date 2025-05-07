project(lab03)

add_executable(task03 task03.cpp)
target_link_libraries(task03 ${OpenCV_LIBS} semcv)
target_compile_definitions(task03 PRIVATE DATA_DIR="${DATA_DIR}")
set_target_properties(task03 PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
