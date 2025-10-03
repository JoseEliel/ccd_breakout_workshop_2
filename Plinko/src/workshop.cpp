#include "collision.hpp"
#include <cstdio>

// Your task is to assemble the collision detection pipeline here.
// Think of it like a puzzle: you have the pieces in collision.cpp,
// and you need to connect them in the right order to make the magic happen.
// This single pipeline should work for both square (AABB) and rotated (OBB) pegs.

RayHit FindEarliestCollision(
    Vector2 ballPos, Vector2 ballVel, float ballRadius,
    const std::vector<Peg>& allPegs, float dt)
{
    // This is a sentinel value. Since valid hits have a `t` between 0.0 and 1.0,
    // starting with a `t` of 2.0 ensures any valid hit will be considered "earlier".
    RayHit best{false, 2.0f};

    // --- PUZZLE PIECE 1: The Broad Phase ---
    // First, we need to quickly figure out which pegs MIGHT be hit this frame.
    // Doing a precise check against every single peg is too slow.
    // In collision.hpp, find the function that gives you a list of potential candidates.
    // Call it here to get a vector of candidate indices.
    //
    // std::vector<size_t> candidates = ... your function call here ...


    // --- PUZZLE PIECE 2: The Narrow Phase ---
    // Now that you have a short list of candidates, you need to check each one carefully.
    // 1. Loop through the candidate indices you got from the broad phase.
    // 2. Inside the loop, get the actual Peg object from the `allPegs` vector using the index.
    // 3. Call the function that performs the precise intersection test between the moving ball and that single peg.
    // 4. This function gives you a RayHit. Check if it's a valid hit (`.hit == true`).
    // 5. If it is a valid hit, check if it's the earliest one we've found so far (its `.t` is smaller than `best.t`).
    // 6. If it's a valid AND earlier hit, update `best` to store this new result.
    //
    // for (size_t idx : candidates) {
    //     ... your logic here ...
    // }

    // After checking all candidates, `best` will either be the initial sentinel value (no hit)
    // or it will contain the information about the very first collision that will happen this frame.
    return best;
}