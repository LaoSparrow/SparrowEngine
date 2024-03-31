cmake_minimum_required(VERSION 3.26)
project(SparrowEngine.Example)

add_executable(SparrowEngine.Example src/main.cpp
        src/scenes/scene1.hpp
        src/scripts/camera_movement.hpp
        src/scenes/scene2.hpp
        src/constants.hpp
        src/scripts/transform_modification.hpp
        src/scenes/scene3.hpp
        src/scenes/scene4.hpp)

find_package(glad CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glad::glad)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

find_package(glm CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)

find_package(Stb REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${Stb_INCLUDE_DIR})

find_package(imgui CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE imgui::imgui)

find_package(assimp CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE assimp::assimp)

find_package(fmt CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt-header-only)

target_include_directories(${PROJECT_NAME} PRIVATE ${SparrowEngine.Core_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} PRIVATE SparrowEngine.Core)