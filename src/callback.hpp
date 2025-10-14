#pragma once

#include <nanobind/nanobind.h>

class Callback {
    public:
        Callback() {}
        Callback(nanobind::callable function) { callback = function; }

        nanobind::callable callback;

        void call(nanobind::args args) { if (callback) callback(*args); }
};