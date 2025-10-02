#pragma once
#include "raylib.h"
#include <vector>



// A structure to hold all the information about a collision event
struct RayHit {
    bool hit{false};
    float t{2.0f};       // Time of impact, normalized to [0,1]. Sentinel is > 1.
    Vector2 point{0,0};  // The world-space point of contact.
    Vector2 normal{0,0}; // The world-space surface normal at the contact point.
};

// A structure to define a single peg
struct Peg {
    Vector2 center;
    Vector2 halfExtents;   // Half-width and half-height.
    float   rotationDeg;   // Rotation in degrees (0 for AABB).
};

// --- Your Puzzle Pieces ---

// Broad phase: A fast check that returns indices of pegs that *might* be colliding.
std::vector<size_t> GetCollisionCandidates(
    Vector2 ballPos, Vector2 ballVel, float ballRadius,
    const std::vector<Peg>& allPegs, float dt);

// Narrow phase: A precise check for a swept circle against a single peg.
RayHit IntersectMovingCirclePeg(
    Vector2 circlePos, Vector2 circleVel, float circleRadius,
    const Peg& peg, float dt);