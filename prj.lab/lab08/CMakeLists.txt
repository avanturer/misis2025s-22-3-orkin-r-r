project(lab08)

add_executable(task08 task08.cpp)
target_link_libraries(task08 ${OpenCV_LIBS} semcv)
target_compile_definitions(task08 PRIVATE DATA_DIR="${DATA_DIR}")
set_target_properties(task08 PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
