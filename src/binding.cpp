#include <nanobind/nanobind.h>
#include <nanobind/trampoline.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/shared_ptr.h>
#include <nanobind/stl/string.h>
#include <iostream>

#include "engine.hpp"
#include "signal.hpp"
#include "callback.hpp"
#include "shape.hpp"

NB_MODULE(binding, m) {
    nanobind::class_<EngineCommunications> engine_communications(m, "EngineCommunications");
    engine_communications.def_prop_ro("status", [](EngineCommunications &comms) { return comms.status; });
    
    nanobind::class_<Engine> engine(m, "Engine");
    engine.def("add_object", &Engine::add_object);
    engine.def("mainloop", &Engine::mainloop);
    engine.def_static("get_singleton", &Engine::get_singleton);
    engine.def_prop_ro("should_close", [](Engine &engine) -> Signal& { return engine.should_close; });

    nanobind::class_<Object, PythonObject> object(m, "Object");
    object.def(nanobind::init<>());
    object.def("update", &Object::update);
    object.def("add_child", &Object::add_child);
    object.def("get_children", &Object::get_children);
    object.def("get_child_by_name", &Object::get_child_by_name);
    object.def_rw("name", &Object::name);
    
    nanobind::class_<Vector2> vector2(m, "Vector2");
    vector2.def("__init__", [](Vector2 *vec2, float x, float y) { new (vec2) Vector2 {x, y}; });
    vector2.def_rw("x", &Vector2::x);
    vector2.def_rw("y", &Vector2::y);
    
    nanobind::class_<Color> color(m, "Color");
    color.def("__init__", [](Color *col, unsigned char r, unsigned char g, unsigned char b, unsigned char a) { new (col) Color {r, g, b, a}; });
    color.def_rw("r", &Color::r);
    color.def_rw("g", &Color::g);
    color.def_rw("b", &Color::b);
    color.def_rw("a", &Color::a);

    nanobind::class_<Texture> texture(m, "Texture");
    texture.def_rw("tex_id", &Texture::id);
    texture.def_rw("width", &Texture::width);
    texture.def_rw("height", &Texture::height);

    nanobind::class_<Shape, Object, PythonShape> shape(m, "Shape");
    shape.def("update", &Shape::update);
    shape.def("draw", &Shape::draw);
    shape.def_rw("position", &Shape::position);
    shape.def_rw("rotation", &Shape::rotation);
    shape.def_rw("scale", &Shape::scale);
    shape.def_rw("color", &Shape::color);
    
    nanobind::class_<Circle, Shape, PythonCircle> circle(m, "Circle");
    circle.def(nanobind::init<float, Color>(), nanobind::arg("radius"), nanobind::arg("color") = Color {0, 0, 0, 255});
    circle.def("draw", &Circle::draw);
    circle.def_rw("radius", &Circle::radius);
    
    nanobind::class_<Rect, Shape, PythonRect> rect(m, "Rect");
    rect.def(
        nanobind::init<float, float, Color, float, float, Rect::RoundedQuality>(),
        nanobind::arg("width"), nanobind::arg("height"), 
        nanobind::arg("color") = Color {0, 0, 0, 255},
        nanobind::arg("border_width") = -1.0,
        nanobind::arg("border_radius") = -1.0,
        nanobind::arg("quality") = 10 // Rect::RoundedQuality::MEDIUM
    );
    rect.def("draw", &Rect::draw);
    rect.def_rw("width", &Rect::width);
    rect.def_rw("height", &Rect::height);
    rect.def_rw("border_width", &Rect::border_width);
    rect.def_rw("border_radius", &Rect::border_radius);
    rect.def_rw("quality", &Rect::quality);

    nanobind::enum_<Rect::RoundedQuality> rounded_quality(rect, "RoundedQuality", nanobind::is_arithmetic());
    rounded_quality.value("LOW", Rect::RoundedQuality::LOW);
    rounded_quality.value("MEDIUM", Rect::RoundedQuality::MEDIUM);
    rounded_quality.value("HIGH", Rect::RoundedQuality::HIGH);
    
    nanobind::class_<Polygon, Shape, PythonPolygon> polygon(m, "Polygon");
    polygon.def(
        nanobind::init<std::vector<Vector2>, Color, float>(), 
        nanobind::arg("points"), 
        nanobind::arg("color") = Color {0, 0, 0, 255}, 
        nanobind::arg("border_width") = -1.0
    );
    polygon.def_prop_rw(
        "points", 
        [](Polygon &poly) -> std::vector<Vector2> { return poly.points; }, 
        [](Polygon &poly, std::vector<Vector2> points) { poly.update_positions(points); }
    );

    nanobind::class_<ImageObject, Shape, PythonImage> image(m, "Image");
    image.def(nanobind::init<>());
    image.def(nanobind::init<Texture, Color>(), nanobind::arg("texture"), nanobind::arg("color") = Color {255, 255, 255, 255});
    image.def(nanobind::init<std::string, Color>(), nanobind::arg("path"), nanobind::arg("color") = Color {255, 255, 255, 255});
    image.def_static("load_texture", &LoadTexture);
    image.def_rw("texture", &ImageObject::texture);

    m.attr("Args") = nanobind::type_var_tuple("Args");
    nanobind::class_<Signal> signal(m, "Signal", nanobind::is_generic(), nanobind::sig("class Signal(typing.Generic[Args])"));
    signal.def(nanobind::init<>());
    signal.def("connect", &Signal::connect, nanobind::sig("def connect(self, typing.Callable[Args, None]) -> None"));
    signal.def("emit", &Signal::emit, nanobind::sig("def emit(self, *args: Args.args, **kwargs: Args.kwargs) -> None"));
    
    nanobind::class_<Callback> callback(m, "Callback", nanobind::is_generic());
    callback.def(nanobind::init<nanobind::callable>());
    callback.def("call", &Callback::call);
    callback.def_rw("callback", &Callback::callback);
}
