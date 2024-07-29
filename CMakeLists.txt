cmake_minimum_required(VERSION 3.14)

project(MaskDefence LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -------------------------------------------------------------------------------------------------
# add cmake module paths
# -------------------------------------------------------------------------------------------------
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/")

# -------------------------------------------------------------------------------------------------
# Generic CMake Settings
# -------------------------------------------------------------------------------------------------
include(ProjectEnvironment)

# -------------------------------------------------------------------------------------------------
# Find packages
# -------------------------------------------------------------------------------------------------
find_package(Qt5 COMPONENTS Core Quick Multimedia AndroidExtras REQUIRED)
find_package(Dlib CONFIG REQUIRED)
find_package(OpenCV CONFIG REQUIRED)

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
    "${CMAKE_SOURCE_DIR}/source/utils/opencv_helper.h"
    "${CMAKE_SOURCE_DIR}/source/utils/opencv_helper.cpp"
    "${CMAKE_SOURCE_DIR}/source/utils/rgb_frame_helper.h"
    "${CMAKE_SOURCE_DIR}/source/utils/rgb_frame_helper.cpp"

    "${CMAKE_SOURCE_DIR}/ui/qml.qrc"
    "${CMAKE_SOURCE_DIR}/resources/res.qrc"
    )

if(ANDROID)
    add_library(MaskDefence SHARED
        ${PROJECT_SOURCES}
        )
else()
    add_executable(MaskDefence
        ${PROJECT_SOURCES}
        )
endif()

target_compile_definitions(${PROJECT_NAME}
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

# -------------------------------------------------------------------------------------------------
# Link libraries
# -------------------------------------------------------------------------------------------------
target_link_libraries(${PROJECT_NAME}
    PRIVATE Qt5::Core Qt5::Quick Qt5::Multimedia Qt5::AndroidExtras)

target_link_libraries(${PROJECT_NAME}
    PUBLIC ${DLIB_LIBRARIES}
    PUBLIC ${OpenCV_LIBS}
    EGL
    GLESv2
    )

# -------------------------------------------------------------------------------------------------
# Special Compiler Flags
# -------------------------------------------------------------------------------------------------
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

# DLib special compiler parameters
set(DLIB_USE_CUDA ON CACHE BOOL "Use AVX instructions")
set(USE_AVX_INSTRUCTIONS ON CACHE BOOL "Use AVX instructions")
set(USE_SSE2_INSTRUCTIONS ON CACHE BOOL "Use SSE2 instructions")
set(USE_SSE4_INSTRUCTIONS ON CACHE BOOL "Use SSE4 instructions")

# -------------------------------------------------------------------------------------------------
# Include directories
# -------------------------------------------------------------------------------------------------
target_include_directories(${PROJECT_NAME} SYSTEM
    PUBLIC ${CMAKE_SOURCE_DIR}
    PUBLIC ${DLIB_DIRS}
    PUBLIC ${OpenCV_INCLUDE_DIRS}
    )
