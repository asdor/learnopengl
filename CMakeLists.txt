cmake_minimum_required(VERSION 3.10)
project(learnopengl VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

set(OpenGL_GL_PREFERENCE LEGACY)
find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)

add_subdirectory(glad)

file(GLOB LEARNOPENGL_SRC src/*.cpp)

add_executable(${PROJECT_NAME} ${LEARNOPENGL_SRC})
target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL glfw Glad)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${OPENGL_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
    -Wall -Wextra -Wpedantic -Werror -fconcepts>
)
