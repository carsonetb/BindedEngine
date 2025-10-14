#pragma once

#include <vector>

#include "raylib.h"

#include "object.hpp"

class Shape : public Object {
    public:
        virtual ~Shape() = default;

        struct TransformationHolder {
            Vector2 position;
            float rotation;
            Vector2 scale;
            static TransformationHolder create() {
                return TransformationHolder {Vector2 {0.0, 0.0}, 0.0, Vector2 {1.0, 1.0}};
            }
        };

        Vector2 position = Vector2 {0.0, 0.0};
        float rotation = 0.0;
        Vector2 scale = Vector2 {1.0, 1.0};
        TransformationHolder global_transform;
        bool updated_transform = false;
        Color color = BLACK;

        void propogate_transformation(TransformationHolder transform = TransformationHolder::create());
        virtual void update();
        virtual void draw() const = 0;
};

class PythonShape : public Shape {
    NB_TRAMPOLINE(Shape, 1);

    void update() override {
        NB_OVERRIDE(update);
    }
};

class Circle : public Shape {
    public:
        Circle(float p_radius, Color p_color = BLACK);

        float radius;

        virtual void update();
        void draw() const override;
};

class PythonCircle : public Circle {
    NB_TRAMPOLINE(Circle, 1);

    void update() override {
        NB_OVERRIDE(update);
    }
};

class Rect : public Shape {
    public:
        enum class RoundedQuality {
            LOW = 4,
            MEDIUM = 10,
            HIGH = 20,
        };

        Rect(float p_width, float p_height, Color p_color = BLACK, float p_border_width = -1.0, float p_border_radius = -1.0, RoundedQuality p_quality = RoundedQuality::MEDIUM);

        float width;
        float height;
        float border_width;
        float border_radius;
        RoundedQuality quality;

        virtual void update();
        void draw() const override;
};

class PythonRect : public Rect {
    NB_TRAMPOLINE(Rect, 1);

    void update() override {
        NB_OVERRIDE(update);
    }
};

class Polygon : public Shape {
    public:
        Polygon(std::vector<Vector2> p_points, Color p_color = BLACK, float p_border_width = -1.0);

        std::vector<Vector2> points;
        float border_width;

        virtual void update();
        void draw() const override;
        void update_positions(std::vector<Vector2> new_positions);
    
    private:
        struct Triangle {
            uint32_t a, b, c;
        };

        std::vector<Triangle> triangle_indices;
};

class PythonPolygon : public Polygon {
    NB_TRAMPOLINE(Polygon, 1);

    void update() override {
        NB_OVERRIDE(update);
    }
};

class ImageObject : public Shape {
    public:
        ImageObject() {}
        ImageObject(Texture p_texture, Color tint = WHITE);
        ImageObject(std::string image_path, Color tint = WHITE);
        ~ImageObject() = default;

        virtual void update();
        void draw() const override;

        Texture2D texture;
};

class PythonImage : public ImageObject {
    NB_TRAMPOLINE(ImageObject, 1);

    void update() override {
        NB_OVERRIDE(update);
    }
};