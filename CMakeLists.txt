cmake_minimum_required(VERSION 2.8)
project ("OpenGLGame")

# VERSION
################################################################################
set(OPENGLGAME_MAJOR_VERSION 0)
set(OPENGLGAME_MINOR_VERSION 0)
set(OPENGLGAME_REVISION_VERSION 1)
################################################################################

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif(NOT CMAKE_BUILD_TYPE)

# REQUIRED LIBRARIES
################################################################################
# Detect and add SFML
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

# SFML
find_package(SFML 2.3 REQUIRED system window graphics)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(assimp REQUIRED)
#####################################################1###########################

set(INCLUDE_PATH "${PROJECT_SOURCE_DIR}/include")
set(SOURCE_PATH "${PROJECT_SOURCE_DIR}/src")

file(
        GLOB_RECURSE
        SOURCES
        ${SOURCE_PATH}/*
)

include_directories(${INCLUDE_PATH})
include_directories(${SFML_INCLUDE_DIR})
include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${ASSIMP_INCLUDE_DIR})
include_directories(SYSTEM "E:/Work/Programare/Libs/_sources/glm")

# Compiler settings
################################################################################
# Detect compiler
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(USING_GCC TRUE)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(USING_CLANG TRUE)
endif()

# For GCC and Clang, enable C++11 support and add some other flags
if(USING_GCC OR USING_CLANG)
  add_compile_options(-std=c++17 -pedantic -Wall)
endif()
################################################################################

message(${OPENGL_LIBRARIES})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

add_executable(
        "OpenGLGame"
        ${SOURCES}
)

target_link_libraries(OpenGLGame
	${SFML_LIBRARIES}
	${GLEW_LIBRARIES}
 	${OPENGL_LIBRARIES}
	"E:/Work/Programare/Libs/_builds/mingw610-v5r1/assimp/lib/libassimp.dll.a" )
