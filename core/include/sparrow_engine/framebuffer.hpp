#pragma once

#include <glad/glad.h>
#include <memory>
#include <vector>
#include "pipeline.hpp"

namespace SE {
    class Framebuffer {
    private:
        static std::shared_ptr<Framebuffer> current_framebuffer;

    public:
        GLuint id{0};
        Pipeline pipeline{};

        Framebuffer() = default;
        ~Framebuffer();

        void load();
        void free();
        void use();

        static void use_default();

        static std::shared_ptr<Framebuffer> create(bool is_default = false);
        static std::shared_ptr<Framebuffer> GetCurrent();

        friend class Scene;
    };
}
