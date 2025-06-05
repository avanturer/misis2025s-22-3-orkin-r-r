# prj.lab CMakeLists.txt

# Add all lab subdirectories
add_subdirectory(lab1_raster_formats_gamma)
add_subdirectory(lab2_image_noise)
add_subdirectory(lab3_auto_contrast)
add_subdirectory(lab4_binarization_detection)
add_subdirectory(lab5_linear_filtering)
add_subdirectory(lab6_object_detection_filtering)
add_subdirectory(lab7_edge_detection)

# Create a target to build all labs
add_custom_target(all_labs)
add_dependencies(all_labs 
    lab1_raster_formats_gamma
    lab2_image_noise
    lab3_auto_contrast
    lab4_binarization_detection
    lab5_linear_filtering
    lab6_object_detection_filtering
    lab7_edge_detection
)
