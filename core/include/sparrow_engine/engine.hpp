#pragma once

#include "sparrow_engine/engine.hpp"
#include "sparrow_engine/game_window.hpp"
#include "sparrow_engine/scene.hpp"
#include "sparrow_engine/time.hpp"
#include "sparrow_engine/logger.hpp"

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace SparrowEngine {

    struct SparrowEngineConfig {
        struct {
            int major_version;
            int minor_version;
        } opengl;
    };

    class Engine {
    public:

//        static void inline InitializeGLAD() {
//            if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
//                std::cout << "Failed to Initialize GLAD" << std::endl;
//            }
//        }

        static void Initialize(SparrowEngineConfig config);

        static void RenderingLoop();
    };

}