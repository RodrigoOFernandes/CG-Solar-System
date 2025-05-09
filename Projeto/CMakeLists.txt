cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

PROJECT(CG_SOLAR_SYSTEM)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Configuração multiplataforma para GLM
if(APPLE)
    set(GLM_INCLUDE_DIR "/opt/homebrew/include" "/usr/local/include")
else()
    set(GLM_INCLUDE_DIR "/usr/include")
endif()

find_path(GLM_INCLUDE_PATH glm/glm.hpp
    PATHS ${GLM_INCLUDE_DIR}
    DOC "Caminho para os headers do GLM"
)

if(NOT GLM_INCLUDE_PATH)
    message(FATAL_ERROR "GLM não encontrado! Instale via: brew install glm (macOS) ou sudo apt-get install libglm-dev (Linux)")
else()
    message(STATUS "GLM encontrado em: ${GLM_INCLUDE_PATH}")
    include_directories(${GLM_INCLUDE_PATH})
endif()

# Funções auxiliares
function(add_source_files target directory)
    file(GLOB_RECURSE SOURCE_FILES "${directory}/*.cpp")
    target_sources(${target} PRIVATE ${SOURCE_FILES})
endfunction()

function(add_include_directories target directory)
    file(GLOB_RECURSE INCLUDE_DIRS "${directory}/*.hpp" "${directory}/*.h")
    foreach(INCLUDE_DIR ${INCLUDE_DIRS})
        get_filename_component(INCLUDE_PATH ${INCLUDE_DIR} DIRECTORY)
        target_include_directories(${target} PRIVATE ${INCLUDE_PATH})
    endforeach()
endfunction()

# === Executável Engine ===
add_executable(engine)
add_source_files(engine "${CMAKE_CURRENT_SOURCE_DIR}/engine/src")
add_source_files(engine "${CMAKE_CURRENT_SOURCE_DIR}/Projeto/src/config")
add_include_directories(engine "${CMAKE_CURRENT_SOURCE_DIR}/engine/include")
add_include_directories(engine "${CMAKE_CURRENT_SOURCE_DIR}/Projeto/include/config")

# === Executável Generator ===
add_executable(generator)
add_source_files(generator "${CMAKE_CURRENT_SOURCE_DIR}/generator/src")
add_include_directories(generator "${CMAKE_CURRENT_SOURCE_DIR}/generator/include")

# === OpenGL e GLUT ===
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})

if(NOT OPENGL_FOUND)
    message(FATAL_ERROR "OpenGL not found!")
endif()

if (WIN32)
    set(TOOLKITS_FOLDER "" CACHE PATH "Path to Toolkits folder")
    
    if (NOT EXISTS "${TOOLKITS_FOLDER}/glut/GL/glut.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/glut/glut32.lib")
        message(FATAL_ERROR "GLUT not found!")
    endif()
    
    include_directories(${TOOLKITS_FOLDER}/glut)
    
    target_link_libraries(engine ${OPENGL_LIBRARIES} ${TOOLKITS_FOLDER}/glut/glut32.lib)
    target_link_libraries(generator ${OPENGL_LIBRARIES} ${TOOLKITS_FOLDER}/glut/glut32.lib)
    
    if (EXISTS "${TOOLKITS_FOLDER}/glut/glut32.dll")
        file(COPY ${TOOLKITS_FOLDER}/glut/glut32.dll DESTINATION ${CMAKE_BINARY_DIR})
    endif()

    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT engine)

else()  # Linux e macOS
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated")
    find_package(GLUT REQUIRED)

    if(NOT GLUT_FOUND)
        message(FATAL_ERROR "GLUT not found!")
    endif()

    include_directories(${GLUT_INCLUDE_DIR})

    # === GLEW multiplataforma ===
    if(APPLE)
        find_path(GLEW_INCLUDE_DIR GL/glew.h
            PATHS /opt/homebrew/include /usr/local/include
        )

        find_library(GLEW_LIBRARY GLEW
            PATHS /opt/homebrew/lib /usr/local/lib
        )

        if(NOT GLEW_INCLUDE_DIR OR NOT GLEW_LIBRARY)
            message(FATAL_ERROR "GLEW não encontrado! Instale com: brew install glew")
        else()
            message(STATUS "GLEW encontrado!")
        endif()

        include_directories(${GLEW_INCLUDE_DIR})
        target_link_libraries(generator ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${GLEW_LIBRARY})
        target_link_libraries(engine ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${GLEW_LIBRARY})
    else()
        find_package(GLEW REQUIRED)

        if(NOT GLEW_FOUND)
            message(FATAL_ERROR "GLEW not found!")
        endif()

        include_directories(${GLEW_INCLUDE_DIRS})
        target_link_libraries(generator ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${GLEW_LIBRARIES})
        target_link_libraries(engine ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${GLEW_LIBRARIES})
    endif()

    # === DevIL (IL + ILU) ===
    if(APPLE)
        find_path(IL_INCLUDE_DIR IL/il.h
            PATHS /opt/homebrew/include /usr/local/include
        )

        find_library(IL_LIBRARY IL
            PATHS /opt/homebrew/lib /usr/local/lib
        )

        find_library(ILU_LIBRARY ILU
            PATHS /opt/homebrew/lib /usr/local/lib
        )

        if(NOT IL_INCLUDE_DIR OR NOT IL_LIBRARY OR NOT ILU_LIBRARY)
            message(FATAL_ERROR "DevIL/ILU não encontrados! Instale com: brew install devil")
        else()
            message(STATUS "DevIL e ILU encontrados!")
        endif()

        include_directories(${IL_INCLUDE_DIR})
        target_link_libraries(engine ${IL_LIBRARY} ${ILU_LIBRARY})
        target_link_libraries(generator ${IL_LIBRARY} ${ILU_LIBRARY})

    else()
        find_path(IL_INCLUDE_DIR IL/il.h /usr/include /usr/local/include)
        find_library(IL_LIBRARY IL /usr/lib /usr/local/lib)
        find_library(ILU_LIBRARY ILU /usr/lib /usr/local/lib)

        if(NOT IL_INCLUDE_DIR OR NOT IL_LIBRARY OR NOT ILU_LIBRARY)
            message(FATAL_ERROR "DevIL/ILU não encontrados! Instale com: sudo apt install libdevil-dev")
        else()
            message(STATUS "DevIL e ILU encontrados!")
        endif()

        include_directories(${IL_INCLUDE_DIR})
        target_link_libraries(engine ${IL_LIBRARY} ${ILU_LIBRARY})
        target_link_libraries(generator ${IL_LIBRARY} ${ILU_LIBRARY})
    endif()
endif()