cmake_minimum_required(VERSION 3.26)
project(SparrowEngine.Core)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_library(SparrowEngine.Core STATIC
        src/dummy/main.cpp
        include/sparrow_engine/game_window.hpp
        src/game_window.cpp
        include/sparrow_engine/engine.hpp
        src/engine.cpp
        include/sparrow_engine/time.hpp
        src/time.cpp
        include/sparrow_engine/game_object.hpp
        src/game_object.cpp
        include/sparrow_engine/shader.hpp
        src/shader.cpp
        include/sparrow_engine/scene.hpp
        src/scene.cpp
        include/sparrow_engine/behavior.hpp
        src/behavior.cpp
        include/sparrow_engine/components/mesh.hpp
        src/components/mesh.cpp
        include/sparrow_engine/logger.hpp
        src/logger.cpp)

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

set(SPARROW_ENGINE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${SPARROW_ENGINE_INCLUDE_DIR})
#target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src)

