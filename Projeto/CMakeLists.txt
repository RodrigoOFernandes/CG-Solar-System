cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 11) # Ou 14, 17...
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Project Name
PROJECT(CG_SOLAR_SYSTEM)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Function to add source files from a directory
function(add_source_files target directory)
    file(GLOB_RECURSE SOURCE_FILES "${directory}/*.cpp")
    target_sources(${target} PRIVATE ${SOURCE_FILES})
endfunction()

# Function to add include directories
function(add_include_directories target directory)
    file(GLOB_RECURSE INCLUDE_DIRS "${directory}/*.hpp" "${directory}/*.h")
    foreach(INCLUDE_DIR ${INCLUDE_DIRS})
        get_filename_component(INCLUDE_PATH ${INCLUDE_DIR} DIRECTORY)
        target_include_directories(${target} PRIVATE ${INCLUDE_PATH})
    endforeach()
endfunction()

# === Engine Executable ===
add_executable(engine)  # Creates `engine` executable
add_source_files(engine "${CMAKE_CURRENT_SOURCE_DIR}/engine/src")
add_source_files(engine "${CMAKE_CURRENT_SOURCE_DIR}/Projeto/src/config")  # Add config source files
add_include_directories(engine "${CMAKE_CURRENT_SOURCE_DIR}/engine/include")
add_include_directories(engine "${CMAKE_CURRENT_SOURCE_DIR}/Projeto/include/config")  # Add config include directory

# === Generator Executable ===
add_executable(generator)  # Creates `generator` executable
add_source_files(generator "${CMAKE_CURRENT_SOURCE_DIR}/generator/src")
add_include_directories(generator "${CMAKE_CURRENT_SOURCE_DIR}/generator/include")

# === OpenGL, GLUT and GLEW Configuration ===
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})

if(NOT OPENGL_FOUND)
    message(FATAL_ERROR "OpenGL not found!")
endif()

# Manual GLEW configuration
set(GLEW_INCLUDE_DIR "/usr/include")  # Parent of GL/glew.h
set(GLEW_LIBRARY "/usr/lib/x86_64-linux-gnu/libGLEW.so")  # Full path to library
include_directories(${GLEW_INCLUDE_DIR})

message(STATUS "Using manual GLEW paths:")
message(STATUS "  GLEW_INCLUDE_DIR = ${GLEW_INCLUDE_DIR}")
message(STATUS "  GLEW_LIBRARY = ${GLEW_LIBRARY}")

if (WIN32)
    set(TOOLKITS_FOLDER "" CACHE PATH "Path to Toolkits folder")
    
    if (NOT EXISTS "${TOOLKITS_FOLDER}/glut/GL/glut.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/glut/glut32.lib")
        message(FATAL_ERROR "GLUT not found!")
    endif()
    
    include_directories(${TOOLKITS_FOLDER}/glut)
    
    target_link_libraries(engine 
        ${OPENGL_LIBRARIES} 
        ${GLEW_LIBRARIES}
        ${TOOLKITS_FOLDER}/glut/glut32.lib
    )
    target_link_libraries(generator 
        ${OPENGL_LIBRARIES} 
        ${GLEW_LIBRARIES}
        ${TOOLKITS_FOLDER}/glut/glut32.lib
    )
    
    if (EXISTS "${TOOLKITS_FOLDER}/glut/glut32.dll")
        file(COPY ${TOOLKITS_FOLDER}/glut/glut32.dll DESTINATION ${CMAKE_BINARY_DIR})
    endif()

    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT engine)

else()  # Linux and MacOS
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated")
    find_package(GLUT REQUIRED)
    include_directories(${GLUT_INCLUDE_DIR})
    link_directories(${GLUT_LIBRARY_DIRS})
    add_definitions(${GLUT_DEFINITIONS})
    
    if(NOT GLUT_FOUND)
        message(FATAL_ERROR "GLUT not found!")
    endif()

    target_link_libraries(engine 
        ${OPENGL_LIBRARIES} 
        ${GLEW_LIBRARY}  # Using manual path
        ${GLUT_LIBRARIES}
    )
    target_link_libraries(generator 
        ${OPENGL_LIBRARIES} 
        ${GLEW_LIBRARY}  # Using manual path
        ${GLUT_LIBRARIES}
    )
endif()
