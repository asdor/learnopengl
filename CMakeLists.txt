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
                BASIC_SETUP
                BUILD missing)

find_package(OpenGL REQUIRED)

file(GLOB LEARNOPENGL_SRC src/*.cpp)

add_executable(${PROJECT_NAME} ${LEARNOPENGL_SRC})
target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL ${CONAN_LIBS})

target_include_directories(${PROJECT_NAME} PUBLIC
    ${OPENGL_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
    -Wall -Wextra -Wpedantic -Werror -fconcepts>
)
