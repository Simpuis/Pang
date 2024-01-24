cmake_minimum_required(VERSION 3.27)
project(OpenGLPong)

set(CMAKE_CXX_STANDARD 20)

include_directories(.)

find_path(TINYGLTF_INCLUDE_DIRS "tiny_gltf.h")

find_package(glad CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Stb REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(Microsoft.GSL CONFIG REQUIRED)
find_package(flecs CONFIG REQUIRED)

add_library(OpenGLPongLib
        src/editor/editor.cpp
        src/editor/editor.h
        src/game.ixx
        src/input_handler.cpp
        src/input_handler.h
        src/render/material.ixx
        src/render/renderer.ixx
        src/render/shader.ixx
        src/serialization/shader_loader.cpp
        src/serialization/shader_loader.h
        src/render/texture.ixx
        src/serialization/serializer.ixx
        src/render/mesh.ixx
        src/tiny_gltf_defines.cpp
        src/gl_debug.h
        src/render/camera.ixx
        src/flecs_modules/transformation/transformation.ixx
)

find_package(EnTT CONFIG REQUIRED)
find_package(Stb REQUIRED)

target_link_libraries(OpenGLPongLib glad::glad)
target_link_libraries(OpenGLPongLib glfw)
target_link_libraries(OpenGLPongLib imgui::imgui glfw OpenGL::GL)
target_link_libraries(OpenGLPongLib EnTT::EnTT)
target_link_libraries(OpenGLPongLib glm::glm)
target_link_libraries(OpenGLPongLib nlohmann_json::nlohmann_json)
target_link_libraries(OpenGLPongLib Microsoft.GSL::GSL)
target_link_libraries(OpenGLPongLib $<IF:$<TARGET_EXISTS:flecs::flecs>,flecs::flecs,flecs::flecs_static>)

add_executable(OpenGLPong src/main.cpp
        src/render/freefly_camera.ixx)

target_link_libraries(OpenGLPong OpenGLPongLib)

add_subdirectory(src/tests)

