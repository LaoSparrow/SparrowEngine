#include "sparrow_engine/input.hpp"
#include "sparrow_engine/game_window.hpp"

#include "fmt/core.h"

#include <algorithm>

using namespace SparrowEngine;

void Input::mouse_button_event(int button, int action, int mods) {
    switch (action) {
        case GLFW_RELEASE:
            mouse_button_states[button] = KeyStates::Release | KeyStates::ReleaseTrigger;
            break;
        case GLFW_PRESS:
            mouse_button_states[button] = KeyStates::Press | KeyStates::Trigger;
            break;
        default:
            fmt::println("Unsupported Mouse Button Action: {}", action);
            break;
    }
}

void Input::key_event(int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_UNKNOWN) {
        fmt::println("Unsupported Key {}: scancode: {}",
                     action == GLFW_PRESS ? "Pressed" : (action == GLFW_RELEASE ? "Released" : "Repeated"),
                     scancode);
        return;
    }

    switch (action) {
        case GLFW_RELEASE:
            key_states[key] = KeyStates::Release | KeyStates::ReleaseTrigger;
            break;
        case GLFW_PRESS:
            key_states[key] = KeyStates::Press | KeyStates::Trigger;
            break;
        case GLFW_REPEAT:
            key_states[key] |= KeyStates::Repeat;
            break;
        default:
            fmt::println("Unsupported Key Action: {}", action);
            break;
    }
}

void Input::pre_update() {
    if (!has_initialized) {
        has_initialized = true;
        glfwGetCursorPos(SparrowEngine::GameWindow::GetCurrent()->glfw_window, &cursor_pos.x, &cursor_pos.y);
        cursor_pos.y = SparrowEngine::GameWindow::GetCurrent()->height - cursor_pos.y;
        last_cursor_pos = cursor_pos;
        return;
    }
    last_cursor_pos = cursor_pos;
    glfwGetCursorPos(SparrowEngine::GameWindow::GetCurrent()->glfw_window, &cursor_pos.x, &cursor_pos.y);
    cursor_pos.y = SparrowEngine::GameWindow::GetCurrent()->height - cursor_pos.y;
    delta_cursor_pos = cursor_pos - last_cursor_pos;
}

void Input::post_update() {
    for (int i = 0; i <= GLFW_MOUSE_BUTTON_LAST; i++) {
        mouse_button_states[i] &= ~(KeyStates::Trigger | KeyStates::ReleaseTrigger);
    }
    for (int i = 0; i <= GLFW_KEY_LAST; i++) {
        key_states[i] &= ~(KeyStates::Repeat | KeyStates::Trigger | KeyStates::ReleaseTrigger);
    }
}

Input::Input() {
    std::fill_n(mouse_button_states, GLFW_MOUSE_BUTTON_LAST + 1, KeyStates::Release);
    std::fill_n(key_states, GLFW_KEY_LAST + 1, KeyStates::Release);
}

void Input::SetInputMode(int mode, int value) {
    glfwSetInputMode(SparrowEngine::GameWindow::GetCurrent()->glfw_window, mode, value);
}

int Input::GetInputMode(int mode) {
    return glfwGetInputMode(SparrowEngine::GameWindow::GetCurrent()->glfw_window, mode);
}

glm::dvec2 Input::GetCursorPosition() {
    return SparrowEngine::GameWindow::GetCurrent()->input_system.cursor_pos;
}

glm::dvec2 Input::GetDeltaCursorPosition() {
    return SparrowEngine::GameWindow::GetCurrent()->input_system.delta_cursor_pos;
}

bool Input::MouseButtonReleased(int button) {
    return (bool)(SparrowEngine::GameWindow::GetCurrent()->input_system.mouse_button_states[button] & Input::KeyStates::Release);
}

bool Input::MouseButtonPressed(int button) {
    return (bool)(SparrowEngine::GameWindow::GetCurrent()->input_system.mouse_button_states[button] & Input::KeyStates::Press);
}

bool Input::MouseButtonTriggered(int button) {
    return (bool)(SparrowEngine::GameWindow::GetCurrent()->input_system.mouse_button_states[button] & Input::KeyStates::Trigger);
}

bool Input::MouseButtonReleaseTriggered(int button) {
    return (bool)(SparrowEngine::GameWindow::GetCurrent()->input_system.mouse_button_states[button] & Input::KeyStates::ReleaseTrigger);
}

bool Input::KeyReleased(int key) {
    return (bool)(SparrowEngine::GameWindow::GetCurrent()->input_system.key_states[key] & Input::KeyStates::Release);
}

bool Input::KeyPressed(int key) {
    return (bool)(SparrowEngine::GameWindow::GetCurrent()->input_system.key_states[key] & Input::KeyStates::Press);
}

bool Input::KeyRepeat(int key) {
    return (bool)(SparrowEngine::GameWindow::GetCurrent()->input_system.key_states[key] & Input::KeyStates::Repeat);
}

bool Input::KeyTriggered(int key) {
    return (bool)(SparrowEngine::GameWindow::GetCurrent()->input_system.key_states[key] & Input::KeyStates::Trigger);
}

bool Input::KeyReleaseTriggered(int key) {
    return (bool)(SparrowEngine::GameWindow::GetCurrent()->input_system.key_states[key] & Input::KeyStates::ReleaseTrigger);
}
