cmake_minimum_required(VERSION 3.10)
project(ShadowSegmentation VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W3)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall)
endif()

if(WIN32 AND OpenCV_DIR)
    string(REPLACE "/lib" "/bin" OPENCV_BIN_DIR "${OpenCV_DIR}")
    
    file(GLOB OPENCV_DLLS "${OPENCV_BIN_DIR}/opencv_world*.dll")
    
    foreach(dll ${OPENCV_DLLS})
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${dll}" $<TARGET_FILE_DIR:${PROJECT_NAME}>
            COMMENT "Copying ${dll}"
        )
    endforeach()
endif()

if(EXISTS "${CMAKE_SOURCE_DIR}/examples")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/examples"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/examples"
        COMMENT "Copying example images"
    )
endif()

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    COMPONENT Runtime
)

install(DIRECTORY examples/
    DESTINATION share/${PROJECT_NAME}/examples
    COMPONENT Examples
    FILES_MATCHING 
    PATTERN "*.jpg" 
    PATTERN "*.png" 
    PATTERN "*.bmp"
    PATTERN "*.tiff"
)

install(FILES README.md
    DESTINATION share/doc/${PROJECT_NAME}
    COMPONENT Documentation
)

if(WIN32 AND OpenCV_DIR)
    string(REPLACE "/lib" "/bin" OPENCV_BIN_DIR "${OpenCV_DIR}")
    
    file(GLOB OPENCV_RUNTIME_DLLS "${OPENCV_BIN_DIR}/opencv_world*.dll")
    
    if(OPENCV_RUNTIME_DLLS)
        install(FILES ${OPENCV_RUNTIME_DLLS}
            DESTINATION bin
            COMPONENT Runtime
        )
    endif()
endif()

set(CPACK_PACKAGE_NAME "ShadowSegmentation")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Shadow Segmentation Application")
set(CPACK_COMPONENTS_ALL Runtime Examples Documentation)
include(CPack)