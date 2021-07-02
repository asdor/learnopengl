cmake_minimum_required(VERSION 3.0)
project(Glad LANGUAGES C)

add_library(${PROJECT_NAME} src/glad.c)
target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_DL_LIBS})
target_include_directories(${PROJECT_NAME} PUBLIC include)
