
# Workshop: The Cosmic Collector


You are the captain of the **Star Scavenger**, a square industrial hauler designed to pick up cargo in deep space. However, your ship's sensors are offline.

1.  You fly right through the **Cargo** (Green Boxes) without picking it up.
2.  You fly right through **Asteroids** (Red Circles) without taking damage.

**Your Goal:** Implement the collision detection logic to make the game playable.

-----

  * `main.cpp` - The game engine. **You only need to edit the specific functions marked with `TODO`.**

-----

## Task 1: The Cargo (AABB vs AABB)

**Goal:** Implement `Student_CheckCollisionAABB`.
When your ship (Blue Square) overlaps with a Cargo Crate (Green Square), the function should return `true`.

### The Logic: "The Separating Axis Test"

Instead of asking "Are we touching?", ask **"Are we definitely separated?"**

Two non-rotated rectangles are **NOT** colliding if there is a gap between them on the X-axis **OR** a gap on the Y-axis.

-----

## Task 2: The Asteroids (Circle vs AABB)

**Goal:** Implement `Student_CheckCollisionAsteroid`.
When your ship hits an Asteroid, return `true` to trigger Game Over.

### Option A: The "Lazy" Way (Fast & Loose)

  * **Concept:** Treat the circle as if it were a square.
  * **Method:** Use the helper `rock.GetBoundingBox()` to get a square that surrounds the asteroid. Then, reuse your function from Task 1.
  * **Pros:** Takes 10 seconds to write.
  * **Cons:** Unfair hitboxes. You will die if you hit the invisible corner of the asteroid's box.

### Option B: The "Pro" Way (The Clamp Method)

  * **Concept:** Find the single point on the rectangle that is **closest** to the circle's center.
  * **Method:**
    1.  **Clamp** the Circle's center `(cx, cy)` to the bounds of the Rectangle.
          * `closestX = clamp(cx, rect.left, rect.right)`
          * `closestY = clamp(cy, rect.top, rect.bottom)`
    2.  Calculate the distance between the **Circle Center** and this **Closest Point**.
    3.  If `distance < radius`, you are colliding.

-----

## 🎓 Cheat Sheet

**Rectangle Properties**

```cpp
float left   = rec.x;
float right  = rec.x + rec.width;
float top    = rec.y;
float bottom = rec.y + rec.height;
```

**Clamping Helper**
We provided a helper function for you:

```cpp
float closestX = FloatClamp(value, min, max);
```

-----

## 🎮 Controls

  * **W / A / S / D**: Move Ship
  * **R**: Reset Game (after Game Over)
