#include "sparrow_engine/rendering_texture.hpp"
#include "sparrow_engine/game_window.hpp"

#include <utility>
#include <fmt/core.h>

using namespace SE;


RenderingTexture::~RenderingTexture() {
    free();
}

void RenderingTexture::load(std::shared_ptr<Framebuffer> fb, int w, int h) {
    if (fb->id == 0) {
        fmt::println("[RenderingTexture] loading default fb is prohibited");
        return;
    }
    width = w;
    height = h;

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D,
                 0, // mipmap level
                 GL_RGB,
                 width, height,
                 0, // always zero
                 GL_RGB,
                 GL_UNSIGNED_BYTE,
                 nullptr);

    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

    fb->use();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, id, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
    framebuffer = fb;

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        fmt::println("[RenderingTexture] framebuffer incomplete {}", status);
        return;
    }

    glEnable(GL_DEPTH_TEST);
}

void RenderingTexture::free() {
    if (id)
        glDeleteTextures(1, &id);
    id = 0;
    if (rbo)
        glDeleteRenderbuffers(1, &rbo);
    rbo = 0;
}

void RenderingTexture::use() {
    if (id)
        glBindTexture(GL_TEXTURE_2D, id);
}

std::shared_ptr<RenderingTexture> RenderingTexture::create(int w, int h) {
    auto sp = std::make_shared<RenderingTexture>();
    sp->load(std::move(Framebuffer::create()), w, h);
    return sp;
}
