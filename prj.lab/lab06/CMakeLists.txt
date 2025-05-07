project(lab06)

add_executable(task06 task06.cpp)
target_link_libraries(task06 ${OpenCV_LIBS} semcv)
target_compile_definitions(task06 PRIVATE DATA_DIR="${DATA_DIR}")
set_target_properties(task06 PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
