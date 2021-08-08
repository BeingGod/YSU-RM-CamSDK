include_directories(${PROJECT_SOURCE_DIR}/include)

aux_source_directory(. SRC_LIST)
# add_library(utils OBJECT ${SRC_LIST})
add_library(utils STATIC ${SRC_LIST})
