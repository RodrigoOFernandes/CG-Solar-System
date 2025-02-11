cmake_minimum_required(VERSION 3.5)

# Project Name
PROJECT(CG_SOLAR_SYSTEM)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Função para adicionar ficheiros de um diretório
function(add_source_files target directory)
    file(GLOB_RECURSE SOURCE_FILES "${directory}/*.cpp")
    target_sources(${target} PRIVATE ${SOURCE_FILES})
endfunction()

# Função para adicionar diretórios de include
function(add_include_directories target directory)
    file(GLOB_RECURSE INCLUDE_DIRS "${directory}/*.hpp" "${directory}/*.h")
    foreach(INCLUDE_DIR ${INCLUDE_DIRS})
        get_filename_component(INCLUDE_PATH ${INCLUDE_DIR} DIRECTORY)
        target_include_directories(${target} PRIVATE ${INCLUDE_PATH})
    endforeach()
endfunction()

# Adicionar executável
add_executable(${PROJECT_NAME})

# Adicionar ficheiros de código fonte
add_source_files(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/engine/src")
add_source_files(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/generator/src")

# Adicionar diretórios de include
add_include_directories(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/engine/include")
add_include_directories(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/generator/include")

find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})

if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)

if  (WIN32)

    message(STATUS "Toolkits_DIR set to: " ${TOOLKITS_FOLDER})
    set(TOOLKITS_FOLDER "" CACHE PATH "Path to Toolkits folder")
    
    if (NOT EXISTS "${TOOLKITS_FOLDER}/glut/GL/glut.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/glut/glut32.lib")
        message(ERROR ": GLUT not found")
    endif (NOT EXISTS "${TOOLKITS_FOLDER}/glut/GL/glut.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/glut/glut32.lib")    
    
    
    include_directories(${TOOLKITS_FOLDER}/glut )
    target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} 
                                              ${TOOLKITS_FOLDER}/glut/glut32.lib)
    
    if (EXISTS "${TOOLKITS_FOLDER}/glut/glut32.dll" )
        file(COPY ${TOOLKITS_FOLDER}/glut/glut32.dll DESTINATION ${CMAKE_BINARY_DIR})
    endif(EXISTS "${TOOLKITS_FOLDER}/glut/glut32.dll" )    
    
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
    
else (WIN32) #Linux and Mac

    set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wno-deprecated")
    find_package(GLUT REQUIRED)
    include_directories(${GLUT_INCLUDE_DIR})
    link_directories(${GLUT_LIBRARY_DIRS})
    add_definitions(${GLUT_DEFINITIONS})
    
    target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} )
    if(NOT GLUT_FOUND)
       message(ERROR ": GLUT not found!")
    endif(NOT GLUT_FOUND)
    
endif(WIN32)