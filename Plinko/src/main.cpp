#include "raylib.h"
#include "raymath.h"
#include "collision.hpp"
#include <vector>
#include <algorithm>

// Toggle Act I vs Act II
// Act I: false (AABB), Act II: true (OBB)
static const bool USE_ROTATED_PEGS = false; // Start with this set to false

struct Ball {
    Vector2 pos;
    Vector2 vel;
    float r;
    Color c;
};

// Your collision pipeline will be implemented in workshop.cpp
RayHit FindEarliestCollision(Vector2, Vector2, float, const std::vector<Peg>&, float);

int main() {
    const int W = 800, H = 900;
    InitWindow(W, H, "Plinko CCD — Single Pipeline");
    SetTargetFPS(60);

    Ball ball{{310, 50}, {0, 0}, 15.0f, GREEN};
    const Ball start = ball;

    // Build pegs
    std::vector<Peg> pegs;
    pegs.reserve(10 * 12);
    const int rows = 10, perRow = 12;
    const float pegW = 20, pegH = 20;
    const float rot = USE_ROTATED_PEGS ? 45.0f : 0.0f;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < perRow; j++) {
            float x = W / (perRow + 1.0f) * (j + 1.0f);
            if (i % 2)
                x += W / (perRow + 1.0f) * 0.5f;
            float y = 150 + i * 60;
            pegs.push_back({{x, y}, {pegW / 2, pegH / 2}, rot});
        }
        // add floor
        pegs.push_back({{W / 2.0f, H - 40 + pegH / 2}, {W / 2.0f, pegH / 2}, 0.0f});
    }

    Vector2 gravity{0, 980.0f};

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        if (dt > 1.0f / 30.0f)
            dt = 1.0f / 30.0f; // Lock timestep for stable CCD
        if (IsKeyPressed(KEY_SPACE) || ball.pos.y > H + 50)
            ball = start;

        // Integrate
        ball.vel = Vector2Add(ball.vel, Vector2Scale(gravity, dt));

        // Collision Detection - This is where your code comes in!
        RayHit hit = FindEarliestCollision(ball.pos, ball.vel, ball.r, pegs, dt);

        // Collision Resolution
        if (hit.hit) {
            // 1. Move to the point of impact
            ball.pos = Vector2Add(ball.pos, Vector2Scale(ball.vel, hit.t * dt));
            
            // 2. Reflect velocity based on the surface normal
            ball.vel = Vector2Scale(Vector2Reflect(ball.vel, hit.normal), 0.85f);
            
            // 3. Nudge the ball slightly to prevent sticking
            ball.pos = Vector2Add(ball.pos, Vector2Scale(hit.normal, 0.01f));

        } else {
            // No collision, so move for the full frame
            ball.pos = Vector2Add(ball.pos, Vector2Scale(ball.vel, dt));
        }

        // --- Drawing ---
        BeginDrawing();
        ClearBackground(BLACK);

        // Draw Pegs
        for (const Peg& p : pegs) {
            Rectangle r{p.center.x, p.center.y, p.halfExtents.x * 2, p.halfExtents.y * 2};
            Vector2 origin{p.halfExtents.x, p.halfExtents.y};
            DrawRectanglePro(r, origin, p.rotationDeg, DARKGRAY);
        }
        
        // Draw Ball
        DrawCircleV(ball.pos, ball.r, ball.c);

        // --- Draw Score Bins at the bottom ---
        const int numBins = 5;
        const float binWidth = (float)W / numBins;
        const float binHeight = 80.0f;
        const float bottomY = H - binHeight;
        const char* scores[] = {"100", "50", "10", "50", "100"};

        // Draw the container and dividers
        DrawRectangle(0, bottomY, W, binHeight, Color{130, 130, 130, 150});
        for (int i = 1; i < numBins; i++) {
            DrawLine(i * binWidth, bottomY, i * binWidth, H, BLACK);
        }
        DrawLine(0, bottomY, W, bottomY, BLACK); // Top border

        // Draw the score text in each bin
        for (int i = 0; i < numBins; i++) {
            const char* text = scores[i];
            int textWidth = MeasureText(text, 40);
            DrawText(text, (i * binWidth) + (binWidth / 2) - (textWidth / 2), bottomY + 5, 40, RAYWHITE);
        }

        // Draw UI text
        DrawText("SPACE to reset", 10, 10, 20, GRAY);
        DrawText(USE_ROTATED_PEGS ? "MODE: Rotated Pegs (OBB)" : "MODE: Square Pegs (AABB)", 10, 40, 20, BLUE);
        
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

