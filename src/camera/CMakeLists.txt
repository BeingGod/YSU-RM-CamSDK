include_directories(${PROJECT_SOURCE_DIR}/include)

aux_source_directory(. SRC_LIST)
# add_library(camera OBJECT ${SRC_LIST})
add_library(camera STATIC ${SRC_LIST})
