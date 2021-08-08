project(YSU-RM-CamSDK)

cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# 设置输出的可执行文件路径
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

set(OS ${CMAKE_SYSTEM_NAME})

# 判断当前操作系统
if (OS STREQUAL "Linux")
    add_definitions(-DLINUX)
    message(STATUS "Compiler platform is ${OS}.")
else (OS STREQUAL "Linux")
    message(FATAL_ERROR "Unsupported platform!")
endif (OS STREQUAL "Linux")

# 查找opencv依赖
find_package(OpenCV REQUIRED)

# 默认使用release进行编译
if("${CMAKE_BUILD_TYPE}" STREQUAL "")
    set(CMAKE_BUILD_TYPE "release")
endif("${CMAKE_BUILD_TYPE}" STREQUAL "")

# 设置编译参数
if("${CMAKE_BUILD_TYPE}" STREQUAL "release")
    set(CMAKE_CXX_FLAGS "-std=c++14 -O3")
    message(STATUS "CMAKE_BUILD_TYPE : ${CMAKE_BUILD_TYPE}, compile ${CMAKE_PROJECT_NAME} with ${CMAKE_CXX_FLAGS} flag.")
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "debug")
    set(CMAKE_CXX_FLAGS "-std=c++14 -O0")
    set(CMAKE_CXX_FLAGS "${COMMON_FLAGS} -g")
    message(STATUS "CMAKE_BUILD_TYPE : ${CMAKE_BUILD_TYPE}, compile ${CMAKE_PROJECT_NAME} with ${CMAKE_CXX_FLAGS} flag.")
endif("${CMAKE_BUILD_TYPE}" STREQUAL "release")

# 添加头文件路径
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/include)

# 添加链接库路径
link_libraries(${OpenCV_LIBRARY_DIRS})

add_subdirectory(src/camera)
add_subdirectory(src/utils)

# 将静态库合并
add_custom_command(OUTPUT libRMCamSDK.a
    COMMAND ar crsT libRMCamSDK.a $<TARGET_FILE:camera> $<TARGET_FILE:utils>
    DEPENDS camera utils)

add_custom_target(_RMCamSDK ALL DEPENDS libRMCamSDK.a)

# 编译示例程序
if (EXAMPLE)
    add_subdirectory(example)
endif (EXAMPLE)

# 安装
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION include)
install(FILES ${CMAKE_BINARY_DIR}/libRMCamSDK.a ${CMAKE_SOURCE_DIR}/lib/libMVSDK.so DESTINATION lib)