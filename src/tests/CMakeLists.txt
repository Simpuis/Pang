cmake_minimum_required(VERSION 3.27)
project(tests)

set(CMAKE_CXX_STANDARD 20)

find_path(TINYGLTF_INCLUDE_DIRS "tiny_gltf.h")

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

find_package(EnTT CONFIG REQUIRED)
find_package(Stb REQUIRED)

add_executable(tests
        serialization.cpp
)

target_include_directories(tests PRIVATE ${TINYGLTF_INCLUDE_DIRS})
target_include_directories(tests PRIVATE ${Stb_INCLUDE_DIR})

target_link_libraries(tests gtest_main)
target_link_libraries(tests EnTT::EnTT)
target_link_libraries(tests OpenGLPongLib)

include(GoogleTest)
gtest_discover_tests(tests)
