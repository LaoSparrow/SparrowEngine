#include "sparrow_engine/framebuffer.hpp"

using namespace SE;

Framebuffer::~Framebuffer() {
    free();
}

void Framebuffer::load() {
    if (id)
        return;
    glCreateFramebuffers(1, &id);
}

void Framebuffer::free() {
    if (id)
        glDeleteFramebuffers(1, &id);
    id = 0;
}

void Framebuffer::use() {
    glBindFramebuffer(GL_FRAMEBUFFER, id);
}

void Framebuffer::use_default() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

std::shared_ptr<Framebuffer> Framebuffer::create(bool is_default) {
    auto sp = std::make_shared<Framebuffer>();
    if (!is_default)
        sp->load();
    return sp;
}

std::shared_ptr<Framebuffer> Framebuffer::current_framebuffer{};

std::shared_ptr<Framebuffer> Framebuffer::GetCurrent() {
    return current_framebuffer;
}

