#pragma once

#include <nanobind/nanobind.h>
#include <nanobind/typing.h>
#include <functional>
#include <vector>

class Signal {
    public:
        void connect(nanobind::callable function);
        void emit(nanobind::args args);
        void emit_internal(nanobind::tuple args);

        std::vector<nanobind::callable> callbacks;
    
    private:
};