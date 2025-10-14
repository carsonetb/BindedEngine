import typing
import enum

class EngineCommunications:
    status: int

class Engine:
    should_close: Signal[[]]

    def add_object(self, obj: Object) -> None: ...
    def mainloop(self) -> EngineCommunications: ...
    @staticmethod
    def get_singleton() -> Engine: ...

class Object:
    name: str

    @typing.overload
    def __init__(self) -> None: ...
    @typing.overload
    def __init__(self, name: str) -> None: ...
    def update(self) -> None: ...
    def add_child(self, child: Object) -> None: ...
    def get_children(self) -> list[Object]: ...
    def get_child_by_name(self, name: str) -> Object: ...

class Vector2:
    x: float
    y: float

    def __init__(self, x: float, y: float) -> None: ...

class Color:
    r: float
    g: float
    b: float
    a: float

    def __init__(self, r: int, g: int, b: int, a: int) -> None: ...

class Texture:
    tex_id: int
    width: float
    height: float

class Shape(Object):
    position: Vector2
    rotation: float
    scale: Vector2 = Vector2(1.0, 1.0)
    color: Color

    def draw(self) -> None: ...

class Circle(Shape):
    radius: float

    def __init__(self, radius: float, color: Color = Color(0, 0, 0, 255)) -> None: ...

class Rect(Shape):
    class RoundedQuality(enum.IntEnum):
        LOW = 4
        MEDIUM = 10
        HIGH = 20

    width: float
    height: float
    border_width: float
    border_radius: float
    quality: RoundedQuality

    def __init__(self, width: float, height: float, color: Color = Color(0, 0, 0, 255), border_width = -1.0, border_radius = -1.0, quality = RoundedQuality.MEDIUM) -> None: ...

class Polygon(Shape):
    points: list[Vector2]

    def __init__(self, points: list[Vector2], color: Color = Color(0, 0, 0, 255), border_width: float = -1.0) -> None: ...

class Image(Shape):
    texture: Texture

    @typing.overload
    def __init__(self) -> None: ...
    @typing.overload
    def __init__(self, texture: Texture, color: Color = Color(255, 255, 255, 255)) -> None: ...
    @typing.overload
    def __init__(self, path: str, color: Color = Color(255, 255, 255, 255)) -> None: ...
    @staticmethod
    def load_texture(path: str) -> Texture: ...

Args = typing.ParamSpec("Args")
class Signal(typing.Generic[Args]):
    def __init__(self) -> None: ...
    def connect(self, function: typing.Callable[Args, None]) -> None: ...
    def emit(self, *args: Args.args, **kwargs: Args.kwargs) -> None: ...

Ret = typing.TypeVar("Ret")
class Callback(typing.Generic[Args, Ret]):
    callback: typing.Callable[Args, Ret]

    def __init__(self, function: typing.Callable[Args, Ret]) -> None: ...
    def call(self, *args: Args.args, **kwargs: Args.kwargs) -> Ret: ...