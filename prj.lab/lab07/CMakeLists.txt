project(lab07)

add_executable(task07-01 task07-01.cpp)
add_executable(task07-02 task07-02.cpp)

target_link_libraries(task07-01 ${OpenCV_LIBS} semcv)
target_link_libraries(task07-02 ${OpenCV_LIBS} semcv)

target_compile_definitions(task07-01 PRIVATE DATA_DIR="${DATA_DIR}")
target_compile_definitions(task07-02 PRIVATE DATA_DIR="${DATA_DIR}")

set_target_properties(task07-01 task07-02 PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
