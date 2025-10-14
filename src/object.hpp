#pragma once

#include <nanobind/trampoline.h>
#include <iostream>
#include <memory>
#include <vector>

#include "callback.hpp"

class Object {
    public:
        Object() {}
        virtual ~Object() = default;

        virtual void update();

        void add_child(std::shared_ptr<Object> child);

        std::vector<std::shared_ptr<Object>> get_children();
        std::shared_ptr<Object> get_child_by_name(std::string p_name);

        std::shared_ptr<Object> parent = nullptr;
        std::vector<std::shared_ptr<Object>> children;
        std::string name;
};

struct PythonObject : Object {
    NB_TRAMPOLINE(Object, 1);

    void update() override {
        NB_OVERRIDE(update);
    }
};