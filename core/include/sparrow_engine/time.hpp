#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace SE {
    class Time {
    private:
        static double last_time;
        static double now;
        static double delta_time;

        static void inline Initialize() {
            last_time = glfwGetTime();
        }

        static void inline Update() {
            now = glfwGetTime();
            delta_time = now - last_time;
            last_time = now;
        }

        friend class Engine;

    public:
        Time() = delete;

        static double GetDeltaTime();
        static double GetTimeNow();
    };
}