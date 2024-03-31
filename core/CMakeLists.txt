cmake_minimum_required(VERSION 3.26)
project(SparrowEngine.Core)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_library(SparrowEngine.Core INTERFACE
        include/sparrow_engine/game_window.hpp
        include/sparrow_engine/engine.hpp
        include/sparrow_engine/time.hpp
        include/sparrow_engine/game_object.hpp
        include/sparrow_engine/shader.hpp
        include/sparrow_engine/scene.hpp
        include/sparrow_engine/behavior.hpp
        include/sparrow_engine/components/mesh.hpp
        include/sparrow_engine/logger.hpp
        include/sparrow_engine/texture.hpp
        include/sparrow_engine/transform.hpp
        include/sparrow_engine/components/camera.hpp
        include/sparrow_engine/lighting.hpp
        include/sparrow_engine/components/light.hpp
        include/sparrow_engine/material.hpp
        include/sparrow_engine/utils.hpp
        include/sparrow_engine/model_loader.hpp
)

find_package(glad CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} INTERFACE glad::glad)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} INTERFACE glfw)

find_package(glm CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} INTERFACE glm::glm)

find_package(Stb REQUIRED)
target_include_directories(${PROJECT_NAME} INTERFACE ${Stb_INCLUDE_DIR})

find_package(imgui CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} INTERFACE imgui::imgui)

find_package(assimp CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} INTERFACE assimp::assimp)

find_package(fmt CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} INTERFACE fmt::fmt-header-only)

set(SPARROW_ENGINE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} INTERFACE ${SPARROW_ENGINE_INCLUDE_DIR})
#target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src)

