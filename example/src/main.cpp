#include "sparrow_engine/engine.hpp"
#include "sparrow_engine/game_window.hpp"

#include "scenes/scene1.hpp"
#include "scenes/scene2.hpp"


int main() {
    SparrowEngine::Engine::Initialize({
                                          .opengl = {
                                              .major_version = 4,
                                              .minor_version = 5,
                                          }
                                      });
    SparrowEngine::GameWindow window("Engine Example", 1920, 1080);

    auto scene = std::make_shared<Scene1>();
    window.bind_scene(scene);

    SparrowEngine::Engine::RenderingLoop();

    SparrowEngine::GameWindow::Terminate();
    return 0;
}
