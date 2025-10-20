#include "signal.hpp"

#include <nanobind/nanobind.h>
#include <functional>
#include <vector>
#include <iostream>

void Signal::connect(nanobind::callable function) {
    callbacks.push_back(function);
}

void Signal::emit(nanobind::args args) {
    for (nanobind::callable &function : callbacks) {
        function(*args);
    }
}

void Signal::emit_internal(nanobind::tuple args) {
    for (nanobind::callable &function : callbacks) {
        function(*args);
    }
}