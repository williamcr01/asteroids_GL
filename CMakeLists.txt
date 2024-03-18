cmake_minimum_required(VERSION 3.20.0)
project(space_rocks)
add_executable(${PROJECT_NAME} ./src/main.c)
include_directories(SYSTEM
  "./lib/glfw-3.4/include"
  "./lib/glad/include"
)
target_link_libraries(${PROJECT_NAME} ../lib/glfw-3.4/build/src/glfw3)
add_library(glad STATIC ../lib/glad/src/glad.c)
target_link_libraries(${PROJECT_NAME} glad)
