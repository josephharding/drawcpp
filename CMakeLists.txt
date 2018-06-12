cmake_minimum_required(VERSION 3.10.2)
project(drawcpp)

# includes custom cmakes
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(glfw3 3.2 REQUIRED)

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

set(SOURCE_FILES src/main.cpp)

add_executable(drawcpp ${SOURCE_FILES})
target_link_libraries(drawcpp ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} glfw)
