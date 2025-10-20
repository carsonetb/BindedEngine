#include "input.hpp"

#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <memory>
#include <raylib.h>
#include <nanobind/stl/string.h>

void Input::update() {
    for (std::string name : action_names) {
        if (actions.count(name) == 0) {
            std::cerr << "Error: Name in action_names not in actions. Continuing." << std::endl;
            continue;
        }
        actions[name]->update();
    }
}

void Input::register_key_event(std::string name, std::shared_ptr<InputEventKey> key) {
    if (actions.count(name) == 0) {
        actions[name] = std::make_shared<InputAction>();
        actions[name]->name = name;
        action_names.push_back(name);
    }
    actions[name]->events.push_back(key);
}

void Input::register_key_event(std::string name, int key) {
    std::shared_ptr<InputEventKey> pointer = std::make_shared<InputEventKey>();
    pointer->key = key;
    register_key_event(name, pointer);
}

std::shared_ptr<InputAction> Input::get_action(std::string name) {
    if (actions.count(name) == 0) {
        std::cerr << "Error: Action with name " << name << " doesn't exist." << std::endl;
        return std::shared_ptr<InputAction>();
    }
    return actions[name];
}

bool Input::is_action_pressed(std::string name) {
    return get_action(name)->pressed;
}

bool Input::is_action_released(std::string name) {
    return !is_action_pressed(name);
}

bool Input::is_action_just_pressed(std::string name) {
    return get_action(name)->just_pressed;
}

bool Input::is_action_just_released(std::string name) {
    return get_action(name)->just_released;
}

void InputAction::update() {
    just_pressed = false;
    just_released = false;
    for (std::shared_ptr<InputEvent> event : events) {
        event->update();
        if (event->pressed) {
            if (!pressed) {
                Input::get_singleton()->action_pressed.emit_internal(nanobind::make_tuple(name));
                just_pressed = true;
            }
            pressed = true;
            released = false;
            return;
        }
    }
    if (!released) {
        Input::get_singleton()->action_released.emit_internal(nanobind::make_tuple(name));
        just_released = true;
    }
    released = true;
    pressed = false;
}

void InputEventKey::update() {
    pressed = IsKeyDown(key);
}