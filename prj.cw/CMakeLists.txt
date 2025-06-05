cmake_minimum_required(VERSION 3.10)
project(ShadowSegmentation)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/shadowledentifier.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${OpenCV_INCLUDE_DIRS}
    include/
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${OpenCV_LIBS}
)

if(WIN32)
    string(REPLACE "/lib" "/bin" OPENCV_BIN_DIR "${OpenCV_DIR}")
    
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${OPENCV_BIN_DIR}/opencv_world4110.dll"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMENT "Copying OpenCV Release DLL"
    )
    
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${OPENCV_BIN_DIR}/opencv_world4110d.dll"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMENT "Copying OpenCV Debug DLL"
    )
endif()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/examples"
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/examples"
    COMMENT "Copying example images"
)
