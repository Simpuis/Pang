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

add_executable(OpenGLPong
        cereal_glm_serialization.h
        custom_game_logic.cpp
        custom_game_logic.h
        editor.cpp
        editor.h
        game.cpp
        game.h
        input_handler.cpp
        input_handler.h
        Main.cpp
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
        texture_loader.cpp
        texture_loader.h
        transform.cpp
        transform.h
        vec2.h
        vec3.h
        name.cpp
        name.h)

target_include_directories(OpenGLPong PRIVATE ${TINYGLTF_INCLUDE_DIRS})
target_include_directories(OpenGLPong PRIVATE ${Stb_INCLUDE_DIR})

target_link_libraries(OpenGLPong PRIVATE glad::glad)
target_link_libraries(OpenGLPong PRIVATE glfw)
target_link_libraries(OpenGLPong PRIVATE imgui::imgui glfw OpenGL::GL)
target_link_libraries(OpenGLPong PRIVATE EnTT::EnTT)
target_link_libraries(OpenGLPong PRIVATE glm::glm)
target_link_libraries(OpenGLPong PRIVATE nlohmann_json::nlohmann_json)