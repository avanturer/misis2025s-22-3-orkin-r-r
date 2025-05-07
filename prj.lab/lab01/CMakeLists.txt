project(lab01)

add_executable(task01-01 task01-01.cpp)
add_executable(task01-02 task01-02.cpp)

target_link_libraries(task01-01 ${OpenCV_LIBS} semcv)
target_link_libraries(task01-02 ${OpenCV_LIBS} semcv)

target_compile_definitions(task01-01 PRIVATE DATA_DIR="${DATA_DIR}")
target_compile_definitions(task01-02 PRIVATE DATA_DIR="${DATA_DIR}")

set_target_properties(task01-01 task01-02 PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
