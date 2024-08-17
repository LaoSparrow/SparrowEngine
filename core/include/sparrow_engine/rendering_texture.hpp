#pragma once

#include <glad/glad.h>
#include <memory>
#include "sparrow_engine/framebuffer.hpp"

namespace SE {
    class RenderingTexture {
    public:
        GLuint id{0};
        GLuint rbo{0};
        int width{0};
        int height{0};
        std::shared_ptr<Framebuffer> framebuffer{};

        RenderingTexture() = default;
        ~RenderingTexture();

        void load(std::shared_ptr<Framebuffer> fb, int w, int h);
        void free();
        void use();

        static std::shared_ptr<RenderingTexture> create(int w, int h);
    };
}