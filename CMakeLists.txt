cmake_minimum_required(VERSION 3.29.0)
project(space_rocks)
add_executable(${PROJECT_NAME} ./src/main.c ./src/util/dynArray.c)
include_directories(SYSTEM
  "./lib/glfw/include"
  "./lib/glad/include"
  "./src/util"
)

target_link_libraries(${PROJECT_NAME} ../lib/glfw/build/src/glfw3)
add_library(glad STATIC ../lib/glad/src/glad.c)
target_link_libraries(${PROJECT_NAME} glad)