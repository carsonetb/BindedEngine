#include "object.hpp"
#include "shape.hpp"
#include "engine.hpp"

#include <memory>

void Object::update() {
    for (std::shared_ptr<Object> child : children) {
        child->update();
    }
}

void Object::add_child(std::shared_ptr<Object> child) {
    child->parent = std::shared_ptr<Object>(this);
    children.push_back(child); 
    Engine::get_singleton()->add_object(child);
}

std::vector<std::shared_ptr<Object>> Object::get_children() { 
    return children; 
}

std::shared_ptr<Object> Object::get_child_by_name(std::string p_name) {
    for (std::shared_ptr<Object> child : children) {
        if (child->name == p_name) {
            return child;
        }
    }
    return nullptr;
}