#include "shape.hpp"

#include <iostream>
#include <array>
#include <typeinfo>
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include <earcut/earcut.hpp>

static float triangle_area(Vector2 a, Vector2 b, Vector2 c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

void Shape::update() {
    rlPushMatrix();
        rlTranslatef(position.x, position.y, 0.0);
        rlRotatef(rotation * RAD2DEG, 0.0, 0.0, 1.0);
        rlScalef(scale.x, scale.y, 1.0);
        Object::update();
        draw();
    rlPopMatrix();
    updated_transform = false;
}

void Shape::propogate_transformation(Shape::TransformationHolder parent_transform) {
    updated_transform = true;
    global_transform = parent_transform;
    global_transform.position += Vector2Rotate(Vector2Multiply(position, parent_transform.scale), parent_transform.rotation); // Parent rotate and scale affects position.
    global_transform.rotation += rotation;
    global_transform.scale *= scale;
    for (std::shared_ptr<Object> child : children) {
        std::shared_ptr<Shape> as_shape = std::dynamic_pointer_cast<Shape>(child);
        if (as_shape) {
            as_shape->propogate_transformation(global_transform);
        }
    }
}

Circle::Circle(float p_radius, Color p_color) {
    color = p_color;
    radius = p_radius;
}

void Circle::draw() const {
    DrawCircle(0.0, 0.0, radius, color);
}

void Circle::update() { Shape::update(); }

Rect::Rect(float p_width, float p_height, Color p_color, float p_border_width, float p_border_radius, RoundedQuality p_quality) {
    color = p_color;
    width = p_width;
    height = p_height;
    border_width = p_border_width;
    border_radius = p_border_radius;
}

void Rect::draw() const {
    Rectangle rectangle { 0.0, 0.0, width, height };
    if (border_width > 0.0 && border_radius > 0.0) {
            DrawRectangleRoundedLinesEx(rectangle, border_radius, (int)quality, border_width, color);
        return;
    }
    if (border_width > 0.0) {
        DrawRectangleLinesEx(rectangle, border_width, color);
        return;
    }
    if (border_radius > 0.0) {
        DrawRectangleRounded(rectangle, border_radius, (int)quality, color);
        return;
    }
    DrawRectangle(-width / 2.0, -height / 2.0, width, height, color);
}

void Rect::update() { Shape::update(); }

Polygon::Polygon(std::vector<Vector2> p_points, Color p_color, float p_border_width) {
    color = p_color;
    update_positions(p_points);
    border_width = p_border_width;
}

void Polygon::update_positions(std::vector<Vector2> new_positions) {
    points = new_positions;
    triangle_indices.clear();

    std::vector<std::array<float, 2>> polygon;
    for (Vector2 point : points) {
        std::array<float, 2> converted = {point.x, point.y};
        polygon.push_back(converted);
    }

    std::vector<std::vector<std::array<float, 2>>> polygon_array = { polygon };
    std::vector<uint32_t> indices = mapbox::earcut<uint32_t>(polygon_array);

    for (int i = 0; i < indices.size() / 3; i++) {
        Triangle normal = Triangle { indices[i * 3], indices[i * 3 + 1], indices[i * 3 + 2] };
        Triangle reverse = Triangle { indices[i * 3], indices[i * 3 + 2], indices[i * 3 + 1] };
        float area = triangle_area(points[normal.a], points[normal.b], points[normal.c]);

        triangle_indices.push_back(area < 0.0 ? normal : reverse);
    }
}

void Polygon::draw() const {
    if (border_width > 0.0) {
        for (int i = 0; i < points.size(); i++) {
            Vector2 this_point = points[i];
            Vector2 next_point = i == points.size() - 1 ? points[0] : points[i + 1];
            DrawLineEx(Vector2Add(position, this_point), Vector2Add(position, next_point), border_width, color);
        }
    }
    else {
        for (Triangle triangle : triangle_indices) {
            DrawTriangle(points[triangle.a], points[triangle.b], points[triangle.c], color);
        }
    }
}

void Polygon::update() { Shape::update(); }

ImageObject::ImageObject(Texture p_texture, Color tint) {
    color = tint;
    texture = p_texture;
}

ImageObject::ImageObject(std::string image_path, Color tint) {
    color = tint;
    texture = LoadTexture(image_path.c_str());
}

void ImageObject::draw() const {
    DrawTexture(texture, -texture.width/2.0, -texture.height/2.0, color);
}

void ImageObject::update() { Shape::update(); }