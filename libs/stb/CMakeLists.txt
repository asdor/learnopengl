cmake_minimum_required(VERSION 3.0)
project(stb_image LANGUAGES C)

add_library(${PROJECT_NAME} src/stb_image.c)
# target_link_libraries(${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} PUBLIC include)
