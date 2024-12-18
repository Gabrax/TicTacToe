cmake_minimum_required(VERSION 3.5)

include(FetchContent)

# Set this to false if you don’t use clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Compiler settings for Clang, MSVC, and GCC
if(MSVC)
    # Set the MSVC runtime library to static multithreading for Release configuration
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:RelWithDebInfo>:RelWithDebInfo>")
    add_compile_options(/MP)
    target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC _CRT_SECURE_NO_WARNINGS)

elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options(
        -Wall -Wextra -Wpedantic
        -Wno-unused-function
        -Wno-unused-parameter
        -ferror-limit=0
        -std=c++17
    )
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(
        -Wall -Wextra -Wpedantic
        -Wno-unused-function
        -Wno-unused-parameter
        -std=c++17
        -fexceptions 
    )
endif()

project(tictactoe)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

add_subdirectory(vendor/glfw-3.3.2)
add_subdirectory(vendor/glad)
add_subdirectory(vendor/stb_image)
add_subdirectory(vendor/glm)
add_subdirectory(vendor/glText)

file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable("${CMAKE_PROJECT_NAME}")

set_property(TARGET "${CMAKE_PROJECT_NAME}" PROPERTY CXX_STANDARD 17)

target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC GLFW_INCLUDE_NONE=1)

target_sources("${CMAKE_PROJECT_NAME}" PRIVATE ${MY_SOURCES})
set(ICON_FILE "${CMAKE_CURRENT_SOURCE_DIR}/res/app.rc")
target_sources("${CMAKE_PROJECT_NAME}" PRIVATE ${ICON_FILE})

option(RELEASE "Enable Windows subsystem for the application" OFF)

if(RELEASE)
    set_target_properties("${CMAKE_PROJECT_NAME}" PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
endif()

# Target link libraries
target_link_libraries("${CMAKE_PROJECT_NAME}" PRIVATE glm glfw glad stb_image glText)
