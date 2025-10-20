#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

#include "signal.hpp"

class InputEvent {
    public:
        bool pressed;

        virtual void update() = 0;
};

class InputEventKey : public InputEvent {
    public:
        int key;

        void update() override;
};

class InputAction {
    public:
        std::string name;

        bool pressed;
        bool released;
        bool just_pressed;
        bool just_released;

        std::vector<std::shared_ptr<InputEvent>> events;

        void update();
};

class Input {
    private:
        std::vector<std::string> action_names;
        std::unordered_map<std::string, std::shared_ptr<InputAction>> actions;

    public:
        static Input *get_singleton() {
            static Input singleton;
            return &singleton;
        }

        Signal action_pressed;
        Signal action_released;

        void update();
        void register_key_event(std::string action_name, std::shared_ptr<InputEventKey> key);
        void register_key_event(std::string action_name, int key);

        std::shared_ptr<InputAction> get_action(std::string action_name);
        bool is_action_pressed(std::string action_name);
        bool is_action_released(std::string action_name);
        bool is_action_just_pressed(std::string action_name);
        bool is_action_just_released(std::string action_name);
};