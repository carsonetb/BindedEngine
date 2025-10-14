import binding
import math

def test():
    global should_quit
    should_quit = True

def test_callback():
    print("every frame")

engine = binding.Engine.get_singleton()
engine.should_close.connect(test)

obj = binding.Image("tex.png")
obj.position = binding.Vector2(100, 100)
engine.add_object(obj)

rect = binding.Image("tex.png")
rect.position = binding.Vector2(100, 0)
obj.add_child(rect)

should_quit = False
while not should_quit:
    obj.rotation += 0.01
    obj.scale.x = math.sin(obj.rotation)
    rect.rotation += 0.01
    out = engine.mainloop()

del rect
del obj
del engine