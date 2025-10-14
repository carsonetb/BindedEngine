#include "engine.hpp"

#include "shape.hpp"

#include <iostream>
#include <raylib.h>
#include <rlgl.h>

void Engine::init() {
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screen_width, screen_height, "raylib [core] example - basic window");
    rlDisableBackfaceCulling();

    SetTargetFPS(60);  
}

void Engine::add_object(std::shared_ptr<Object> object) {
    objects.push_back(object);
}

EngineCommunications Engine::mainloop() {
    for (std::shared_ptr<Object> object : objects) {
        std::shared_ptr<Shape> as_shape = std::dynamic_pointer_cast<Shape>(object);
        if (as_shape && !as_shape->updated_transform) {
            as_shape->propogate_transformation();
        }
    }

    BeginDrawing();
        ClearBackground(RAYWHITE);

        for (std::shared_ptr<Object> object : objects) {
            if (object->parent) { // Only want objects without a parent, aka they are at the top.
                std::shared_ptr<Shape> as_shape = std::dynamic_pointer_cast<Shape>(object);
                if (as_shape) {
                    DrawCircleV(as_shape->global_transform.position, 10, BLACK);
                }
                continue;
            }
            object->update();
            std::shared_ptr<Shape> as_shape = std::dynamic_pointer_cast<Shape>(object);
            if (as_shape) {
                DrawCircleV(as_shape->global_transform.position, 10, BLACK);
            }
        }
    EndDrawing();

    EngineCommunications output;
    if (WindowShouldClose()) {
        should_close.emit({});
        output.status = 1;
    }
    else {
        output.status = 0;
    }
    return output;
}