cmake_minimum_required(VERSION 3.16)
project(PointShadows)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(ADDITIONAL_LIBRARIES
        -lGL
        -lGLU
        -lglfw
        -lGLEW
        -lassimp)

link_directories(./../RendererLibrary)
include_directories(./../RendererLibrary/Headers)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} RendererLibrary)
target_link_libraries(${PROJECT_NAME} ${ADDITIONAL_LIBRARIES} ${GLFW_LIBRARIES} ${OPENGL_LIBRARIES})