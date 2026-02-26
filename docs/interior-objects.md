# Interior Objects System

Duke Nukem 3D-style furniture and props to make corridors feel like enclosed spaceship interiors.

## Overview

Objects are simple box geometry defined in map JSON files. Each object has a type that determines its default color, position, and size. Objects participate in collision detection - the player cannot walk through them.

## JSON Format

```json
{
  "objects": [
    {
      "type": "desk",
      "position": [0, 0, -5],
      "size": [0.8, 0.7, 0.4],
      "color": [139, 90, 43, 255]  // optional override
    }
  ]
}
```

### Fields

| Field | Type | Required | Description |
|-------|------|----------|-------------|
| type | string | yes | Object type (determines default color) |
| position | [x, y, z] | yes | Base center position (Y is floor level) |
| size | [w, h, d] | yes | Width, height, depth in meters |
| color | [r, g, b, a] | no | RGBA color override (0-255) |

### Position Convention

Position specifies the **base center** of the object:

- X: center of object width
- Y: bottom of object (floor level for furniture)
- Z: center of object depth

This makes it easy to place furniture on the floor - just set Y=0.

## Object Types

| Type | Default Color | Use |
|------|---------------|-----|
| desk | brown (139, 90, 43) | Work surface |
| chair | dark brown (101, 67, 33) | Seating |
| computer | dark gray (50, 50, 60) | Terminal base |
| screen | cyan (0, 200, 200) | Active display |
| panel | light gray (180, 180, 190) | Control panel |
| window | dark blue (20, 20, 40, 200) | View to space (semi-transparent) |
| crate | tan (194, 178, 128) | Storage |
| pipe | silver (160, 170, 180) | Infrastructure |
| light_fixture | white (240, 240, 250) | Ceiling lights |
| bed | gray-blue (120, 130, 150) | Sleep pod |

Unknown types default to gray (200, 200, 200).

## Architecture

```
src/core/object.hpp     GameObject struct, Color4, default_object_color()
src/core/world.hpp      World.objects vector, collision includes objects
src/core/world_loader.cpp  Parses "objects" array from JSON
src/platform/renderer.cpp  draw_object() renders with type-based colors
```

### GameObject Struct

```cpp
struct GameObject {
    std::string type;
    Vec3 position;
    Vec3 size;
    std::optional<Color4> color;

    AABB bounds() const;  // Computes bounding box from position/size
};
```

### Collision

Objects are included in `World::collides()` checks. The player's movement system automatically prevents walking through objects - no additional code needed.

```cpp
bool World::collides(AABB box) const {
    for (const auto& s : solids) {
        if (s.bounds.intersects(box)) return true;
    }
    for (const auto& obj : objects) {
        if (obj.bounds().intersects(box)) return true;
    }
    return false;
}
```

## Example: Workstation

```json
{
  "objects": [
    {"type": "desk", "position": [-0.8, 0, -18], "size": [0.8, 0.7, 0.4]},
    {"type": "chair", "position": [-0.8, 0, -17.2], "size": [0.5, 0.8, 0.5]},
    {"type": "computer", "position": [-0.8, 0.7, -18], "size": [0.4, 0.3, 0.3]},
    {"type": "screen", "position": [-0.8, 0.85, -18.1], "size": [0.35, 0.2, 0.02]}
  ]
}
```

The computer sits on top of the desk (Y=0.7, desk height). The screen sits on top of the computer (Y=0.85).

## Future Extensions

- **Rotation**: Add yaw angle for non-axis-aligned objects
- **Interactivity**: Door objects that open, screens that display info
- **Emissive**: Light-emitting objects (screens, light fixtures)
- **Breakable**: Crates that can be destroyed
