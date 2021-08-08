add_library(RMCamSDK STATIC IMPORTED)

set_target_properties(RMCamSDK PROPERTIES
    IMPORTED_LOCATION ${CMAKE_BINARY_DIR}/libRMCamSDK.a)

set(EXAMPLE_SOURCE_DIR ${CMAKE_SOURCE_DIR}/example)
if ("${EXAMPLE}" STREQUAL "all")
    add_executable(test_camera_manager ${EXAMPLE_SOURCE_DIR}/test_camera_manager.cpp)
    target_link_libraries(test_camera_manager RMCamSDK MVSDK ${OpenCV_LIBS} pthread)
elseif ("${EXAMPLE}" STREQUAL "usb")
    add_executable(test_usb_camera ${EXAMPLE_SOURCE_DIR}/test_usb_camera.cpp)
    target_link_libraries(test_usb_camera RMCamSDK MVSDK ${OpenCV_LIBS} pthread)
elseif ("${EXAMPLE}" STREQUAL "mindvision")
    add_executable(test_mindvision_camera ${EXAMPLE_SOURCE_DIR}/test_mindvision_camera.cpp)
    target_link_libraries(test_mindvision_camera RMCamSDK MVSDK ${OpenCV_LIBS} pthread)
endif ("${EXAMPLE}" STREQUAL "all")