project(lab05)

add_executable(task05 task05.cpp)
target_link_libraries(task05 ${OpenCV_LIBS} semcv)
target_compile_definitions(task05 PRIVATE DATA_DIR="${DATA_DIR}")
set_target_properties(task05 PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
