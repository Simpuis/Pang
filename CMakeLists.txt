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
        src/custom_game_logic.cpp
        src/custom_game_logic.h
        src/editor/editor.cpp
        src/editor/editor.h
        src/game.cpp
        src/game.h
        src/input_handler.cpp
        src/input_handler.h
        src/render/material.cpp
        src/render/material.h
        src/components/material_component.cpp
        src/components/material_component.h
        src/render/renderer.cpp
        src/render/renderer.h
        src/render/shader.cpp
        src/render/shader.h
        src/serialization/shader_loader.cpp
        src/serialization/shader_loader.h
        src/serialization/scene_serializer.cpp
        src/serialization/scene_serializer.h
        src/render/texture.cpp
        src/render/texture.h
        src/serialization/scene_deserializer.cpp
        src/serialization/scene_deserializer.h
        src/components/mesh_component.cpp
        src/components/mesh_component.h
        src/render/mesh.cpp
        src/render/mesh.h
        src/tiny_gltf_defines.cpp
        src/gl_debug.h
        src/render/camera.cpp
        src/render/camera.h
        src/render/freefly_camera.cpp
        src/render/freefly_camera.h
        src/utility/transformations.h
        src/modules/transformation/transformation.cpp
        src/modules/transformation/transformation.h
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

add_executable(OpenGLPong src/main.cpp)

target_link_libraries(OpenGLPong OpenGLPongLib)

add_subdirectory(src/tests)

