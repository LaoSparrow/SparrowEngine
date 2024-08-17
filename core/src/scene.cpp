#include "sparrow_engine/scene.hpp"
#include "sparrow_engine/game_window.hpp"

#include <ranges>

using namespace SE;

#define ITERATE_OVER_OBJECTS(x) \
do {\
obj_stack.push(scene_root);\
while (!obj_stack.empty()) {\
    auto obj = obj_stack.top();\
    obj_stack.pop();\
    obj->x;\
    for (const auto& b : obj->behaviors) {\
        b->x;\
    }\
    for (const auto& child_obj : obj->children | std::views::reverse) {\
        obj_stack.push(child_obj);\
    }\
}\
} while(0)

void Scene::new_frame() {
    start_up.get();
    lighting->use();
    update();
}

void Scene::start() {
    ITERATE_OVER_OBJECTS(pre_start());
    ITERATE_OVER_OBJECTS(start());
    ITERATE_OVER_OBJECTS(post_start());
}

void Scene::update() {
    ITERATE_OVER_OBJECTS(pre_update());
    ITERATE_OVER_OBJECTS(update());
    ITERATE_OVER_OBJECTS(post_update());
}

void Scene::render() {
    while (!rendering_texture_queue.empty()) {
        auto sp = rendering_texture_queue.front();
        rendering_texture_queue.pop();
        Framebuffer::current_framebuffer = sp->framebuffer;
        sp->framebuffer->use();
        Pipeline::current = sp->framebuffer->pipeline;

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, sp->width, sp->height);

        ITERATE_OVER_OBJECTS(pre_render());
        ITERATE_OVER_OBJECTS(render());
        ITERATE_OVER_OBJECTS(post_render());
    }
    auto dfb = GameWindow::GetCurrent()->default_framebuffer;
    Framebuffer::current_framebuffer = dfb;
    dfb->use();
    Pipeline::current = dfb->pipeline;

    glViewport(0, 0, GameWindow::GetCurrent()->width, GameWindow::GetCurrent()->height);

    ITERATE_OVER_OBJECTS(pre_render());
    ITERATE_OVER_OBJECTS(render());
    ITERATE_OVER_OBJECTS(post_render());
}

std::shared_ptr<Scene> Scene::current_scene;
std::shared_ptr<Scene> Scene::next_scene;

std::shared_ptr<Scene> Scene::GetCurrent() {
    return current_scene;
}

void Scene::SwitchTo(std::shared_ptr<Scene> scene) {
    next_scene = std::move(scene);
}

#undef ITERATE_OVER_OBJECTS
