cmake_minimum_required(VERSION 3.27)
project(OpenGLPong)

set(CMAKE_CXX_STANDARD 17)

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

add_library(OpenGLPongLib
        cereal_glm_serialization.h
        custom_game_logic.cpp
        custom_game_logic.h
        editor.cpp
        editor.h
        game.cpp
        game.h
        input_handler.cpp
        input_handler.h
        material.cpp
        material.h
        material_component.cpp
        material_component.h
        renderer.cpp
        renderer.h
        scene_graph.cpp
        scene_graph.h
        shader.cpp
        shader.h
        shader_loader.cpp
        shader_loader.h
        scene_serializer.cpp
        scene_serializer.h
        sprite.cpp
        sprite.h
        texture.cpp
        texture.h
        transform.cpp
        transform.h
        name.cpp
        name.h
        scene_deserializer.cpp
        scene_deserializer.h
        mesh_component.cpp
        mesh_component.h
        mesh.cpp
        mesh.h
        tiny_gltf_defines.cpp
        gl_debug.h
        camera.cpp
        camera.h
        freefly_camera.cpp
        freefly_camera.h
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

add_executable(OpenGLPong Main.cpp)

target_link_libraries(OpenGLPong OpenGLPongLib)

add_subdirectory(tests)

