project(lab04)

add_executable(task04-01 task04-01.cpp)
add_executable(task04-02 task04-02.cpp)
add_executable(task04-03 task04-03.cpp)

target_link_libraries(task04-01 ${OpenCV_LIBS} semcv)
target_link_libraries(task04-02 ${OpenCV_LIBS} semcv)
target_link_libraries(task04-03 ${OpenCV_LIBS} semcv)

target_compile_definitions(task04-01 PRIVATE DATA_DIR="${DATA_DIR}")
target_compile_definitions(task04-02 PRIVATE DATA_DIR="${DATA_DIR}")
target_compile_definitions(task04-03 PRIVATE DATA_DIR="${DATA_DIR}")

set_target_properties(task04-01 task04-02 task04-03 PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
