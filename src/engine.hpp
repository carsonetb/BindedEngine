#pragma once

#include <memory>
#include <vector>

#include "signal.hpp"
#include "object.hpp"

struct EngineCommunications {
    int status;
};

class Engine {
    private:
        const int screen_width = 800;
        const int screen_height = 450;

        void init();
    
    public:
        Engine() { init(); }

        static Engine *get_singleton() {
            static Engine singleton;
            return &singleton;
        }
        void add_object(std::shared_ptr<Object>);
        EngineCommunications mainloop();
        
        Signal should_close;
        std::vector<std::shared_ptr<Object>> objects;
};