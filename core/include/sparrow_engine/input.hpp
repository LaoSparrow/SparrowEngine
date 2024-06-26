#pragma once

#include "sparrow_engine/utils.hpp"

#include <cstdint>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "glm/vec2.hpp"

namespace SparrowEngine {
    class Input {
    public:
        enum class KeyStates : uint8_t {
            Release = 1,
            Press = 2,
            Repeat = 4,
            Trigger = 8,
            ReleaseTrigger = 16
        };

    private:
        bool has_initialized {};

        glm::dvec2 last_cursor_pos {};
        glm::dvec2 cursor_pos {};
        glm::dvec2 delta_cursor_pos {};
        KeyStates mouse_button_states[GLFW_MOUSE_BUTTON_LAST + 1] {};
        KeyStates key_states[GLFW_KEY_LAST + 1] {};

        void mouse_button_event(int button, int action, int mods);
        void key_event(int key, int scancode, int action, int mods);
        void pre_update();
        void post_update();

    public:
        Input();

        static void SetInputMode(int mode, int value);
        static int GetInputMode(int mode);

        static glm::dvec2 GetCursorPosition();
        static glm::dvec2 GetDeltaCursorPosition();

        static bool MouseButtonReleased(int button);
        static bool MouseButtonPressed(int button);
        static bool MouseButtonTriggered(int button);
        static bool MouseButtonReleaseTriggered(int button);

        static bool KeyReleased(int key);
        static bool KeyPressed(int key);
        static bool KeyRepeat(int key);
        static bool KeyTriggered(int key);
        static bool KeyReleaseTriggered(int key);

        friend class GameWindow;
        friend class Input;
    };
}

DEFINE_ENUM_FLAG_OPERATORS(SparrowEngine::Input::KeyStates)
