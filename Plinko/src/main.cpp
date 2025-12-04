#include "raylib.h"
#include <vector>

// ----------------------------------------------------------------------------------
// PART 1: THE DATA STRUCTURES
// ----------------------------------------------------------------------------------

struct Asteroid {
    Vector2 center;
    float radius;

    // HELPER METHOD: Returns a square box that surrounds the circle.
    // Students can use this for the "Easy" solution.
    Rectangle GetBoundingBox() {
        return { center.x - radius, center.y - radius, radius * 2, radius * 2 };
    }
};

struct Cargo {
    Rectangle rect;
    bool active;
};

// ----------------------------------------------------------------------------------
// PART 2: STUDENT ASSIGNMENT AREA
// ----------------------------------------------------------------------------------

// TASK 1: Implement Box vs Box collision (Separating Axis)
// Returns true if the two rectangles overlap
bool Student_CheckCollisionAABB(Rectangle rec1, Rectangle rec2) {
    // TODO:
    // 1. Check if they are separated on the X axis (rec1.x > rec2.x + rec2.width...)
    // 2. Check if they are separated on the Y axis
    // 3. If they are NOT separated on either, they must be touching.
    
    return false; // <-- Change this to return the real result
}

// HELPER: Clamps a value between a min and max (Useful for Task 2B)
float FloatClamp(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

// TASK 2: Implement Asteroid (Circle) vs Ship (Rectangle) collision
// Returns true if the asteroid hits the ship
bool Student_CheckCollisionAsteroid(Asteroid rock, Rectangle ship) {
    
    // --- OPTION A: The "Square" Approach (Easy) ---
    // Treat the asteroid like a square box using the helper method.
    // 1. Rectangle rockBox = rock.GetBoundingBox();
    // 2. return Student_CheckCollisionAABB(rockBox, ship);
    
    
    // --- OPTION B: The "Circle" Approach (Accurate) ---
    // 1. Find the closest point on the ship rectangle to the rock's center.
    //    Hint: Use FloatClamp(rock.center.x, ship.x, ship.x + ship.width)
    // 2. Calculate distance squared from that point to rock.center
    // 3. Check if distance squared < radius * radius
    
    return false; // <-- Change this!
}


// ----------------------------------------------------------------------------------
// PART 3: THE GAME ENGINE (Students don't need to touch this)
// ----------------------------------------------------------------------------------

int main() {
    InitWindow(800, 600, "Lecture 1: Cosmic Collector");
    SetTargetFPS(60);

    // Player variables
    Rectangle player = { 400, 300, 40, 40 }; // x, y, width, height
    
    // Spawn some asteroids
    std::vector<Asteroid> asteroids;
    for(int i=0; i<6; i++) {
        asteroids.push_back({ 
            {(float)GetRandomValue(0,800), (float)GetRandomValue(0,600)}, 
            (float)GetRandomValue(25, 45) 
        });
    }

    // Spawn some cargo
    std::vector<Cargo> cargoList;
    for(int i=0; i<10; i++) {
        cargoList.push_back({ 
            {(float)GetRandomValue(50,700), (float)GetRandomValue(50,500), 30, 30}, 
            true 
        });
    }

    int score = 0;
    bool gameOver = false;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        // --- UPDATE ---
        if (!gameOver) {
            // Player Movement
            if (IsKeyDown(KEY_W)) player.y -= 300 * dt;
            if (IsKeyDown(KEY_S)) player.y += 300 * dt;
            if (IsKeyDown(KEY_A)) player.x -= 300 * dt;
            if (IsKeyDown(KEY_D)) player.x += 300 * dt;

            // Keep player inside screen
            if (player.x < 0) player.x = 0;
            if (player.y < 0) player.y = 0;
            if (player.x > 800 - player.width) player.x = 800 - player.width;
            if (player.y > 600 - player.height) player.y = 600 - player.height;

            // 1. Check Cargo Collision (Uses Student Task 1)
            for (auto& crate : cargoList) {
                if (crate.active) {
                    if (Student_CheckCollisionAABB(player, crate.rect)) {
                        crate.active = false;
                        score++;
                    }
                }
            }

            // 2. Check Asteroid Collision (Uses Student Task 2)
            for (auto& rock : asteroids) {
                if (Student_CheckCollisionAsteroid(rock, player)) {
                    gameOver = true;
                }
                
                // Simple asteroid animation
                // (Using Y position for direction ensures stability without pointer casting)
                int direction = ((int)rock.center.y % 2 == 0) ? 1 : -1;
                rock.center.x += 150 * dt * direction;
                
                // Screen wrapping for asteroids
                if (rock.center.x > 850) rock.center.x = -50;
                if (rock.center.x < -50) rock.center.x = 850;
            }
        } else {
            // Reset Game
            if (IsKeyPressed(KEY_R)) {
                gameOver = false;
                score = 0;
                player.x = 400; player.y = 300;
                for (auto& c : cargoList) c.active = true;
            }
        }

        // --- DRAW ---
        BeginDrawing();
        ClearBackground({20, 20, 40, 255}); // Deep space blue

        // Draw Cargo (Green Boxes)
        for (auto& crate : cargoList) {
            if (crate.active) DrawRectangleRec(crate.rect, GREEN);
        }

        // Draw Player (Blue Box)
        DrawRectangleRec(player, SKYBLUE);

        // Draw Asteroids (Red Circles)
        for (auto& rock : asteroids) {
            DrawCircleV(rock.center, rock.radius, RED);
            
            // Visual Hint: Draw the bounding box faintly so they understand Option A
            Rectangle box = rock.GetBoundingBox();
            DrawRectangleLinesEx(box, 1, {255, 255, 255, 40}); 
        }

        // UI
        DrawText(TextFormat("Score: %i", score), 10, 10, 20, WHITE);
        
        if (gameOver) {
            DrawText("HULL CRITICAL!", 280, 250, 40, RED);
            DrawText("Press 'R' to Retry", 310, 300, 20, RAYWHITE);
        } else {
            DrawText("WASD to Move", 10, 570, 10, GRAY);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
