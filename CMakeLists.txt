cmake_minimum_required(VERSION 3.23)

project(MaskDefence LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/")

include(ProjectEnvironment)

find_package(Qt6 COMPONENTS
    Core
    Gui
    Quick
    Qml
    Multimedia
    REQUIRED
)

find_package(OpenCV CONFIG REQUIRED)

# -------------------------------------------------------------------------------------------------
# Dlib headers
# Current structure:
# artifacts/Android-arm64-v8a/include/dlib/...
# -------------------------------------------------------------------------------------------------
set(DLIB_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/artifacts/Android-arm64-v8a/include")

set(PROJECT_SOURCES
    "${CMAKE_SOURCE_DIR}/source/main.cpp"
    "${CMAKE_SOURCE_DIR}/source/application_controller.h"
    "${CMAKE_SOURCE_DIR}/source/application_controller.cpp"

    "${CMAKE_SOURCE_DIR}/source/camera/mask_detection_filter.h"
    "${CMAKE_SOURCE_DIR}/source/camera/mask_detection_filter.cpp"

    "${CMAKE_SOURCE_DIR}/source/core/mask_detection.h"
    "${CMAKE_SOURCE_DIR}/source/core/mask_detection.cpp"
    "${CMAKE_SOURCE_DIR}/source/core/mask_result.h"
    "${CMAKE_SOURCE_DIR}/source/core/mask_result.cpp"
    "${CMAKE_SOURCE_DIR}/source/core/mask_result_collector.h"
    "${CMAKE_SOURCE_DIR}/source/core/mask_result_collector.cpp"
    "${CMAKE_SOURCE_DIR}/source/core/mask_detection_worker.h"
    "${CMAKE_SOURCE_DIR}/source/core/mask_detection_worker.cpp"


    "${CMAKE_SOURCE_DIR}/source/utils/opencv_helper.h"
    "${CMAKE_SOURCE_DIR}/source/utils/opencv_helper.cpp"
    "${CMAKE_SOURCE_DIR}/source/utils/rgb_frame_helper.h"
    "${CMAKE_SOURCE_DIR}/source/utils/rgb_frame_helper.cpp"

    "${CMAKE_SOURCE_DIR}/ui/qml.qrc"
    "${CMAKE_SOURCE_DIR}/resources/res.qrc"
)

qt_add_executable(${PROJECT_NAME}
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
    source/core/mask_detection_worker.h source/core/mask_detection_worker.cpp
)

if(ANDROID)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY
            "${CMAKE_BINARY_DIR}/android-build-${PROJECT_NAME}/libs/${ANDROID_ABI}"
    )
endif()

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Quick
        Qt6::Qml
        Qt6::Multimedia
        ${OpenCV_LIBS}
)

if(ANDROID)
    target_link_libraries(${PROJECT_NAME}
        PRIVATE
            EGL
            GLESv2
            log
            android
    )
endif()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

target_include_directories(${PROJECT_NAME} SYSTEM
    PUBLIC
        ${CMAKE_SOURCE_DIR}
        ${OpenCV_INCLUDE_DIRS}
        ${DLIB_INCLUDE_DIR}
)

qt_finalize_executable(${PROJECT_NAME})