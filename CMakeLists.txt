cmake_minimum_required(VERSION 3.12)
project(learnopengl VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
   message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
   file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/master/conan.cmake"
                  "${CMAKE_BINARY_DIR}/conan.cmake")
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_cmake_run(CONANFILE conanfile.txt
                BASIC_SETUP NO_OUTPUT_DIRS
                BUILD missing)

find_package(OpenGL REQUIRED)

file(GLOB LEARNOPENGL_SRC src/*.cpp)

add_executable(${PROJECT_NAME} ${LEARNOPENGL_SRC})
target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL ${CONAN_LIBS})

target_include_directories(${PROJECT_NAME} PUBLIC
    ${OPENGL_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

file(GLOB ASSETS_DATA ${CMAKE_CURRENT_SOURCE_DIR}/assets/*)
file(COPY ${ASSETS_DATA}
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/assets)

file(GLOB SHADERS_DATA RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/src/shaders ${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/*)
foreach(SHADER_FILE ${SHADERS_DATA})
    set(SRC_SHADER ${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/${SHADER_FILE})
    set(DST_SHADER ${CMAKE_CURRENT_BINARY_DIR}/shaders/${SHADER_FILE})
    message("Copying ${SRC_SHADER} to ${DST_SHADER}")
    configure_file(${SRC_SHADER} ${DST_SHADER} COPYONLY)
endforeach()

target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
    -Wall -Wextra -Wpedantic -Werror -fconcepts>
    $<$<CXX_COMPILER_ID:MSVC>:
    /W4>
)
