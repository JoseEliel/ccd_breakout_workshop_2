#include "collision.hpp"
#include "raymath.h"
#include <algorithm>
#include <cmath>

namespace {
    // Helper function to calculate the AABB of a moving circle over a frame.
    Rectangle SweptCircleAABB(Vector2 pos, Vector2 vel, float r, float dt) {
        Rectangle a{pos.x - r, pos.y - r, 2 * r, 2 * r};
        Vector2 e = Vector2Add(pos, Vector2Scale(vel, dt));
        Rectangle b{e.x - r, e.y - r, 2 * r, 2 * r};
        float minX = std::min(a.x, b.x);
        float minY = std::min(a.y, b.y);
        float maxX = std::max(a.x + a.width, b.x + b.width);
        float maxY = std::max(a.y + a.height, b.y + a.height);
        return {minX, minY, maxX - minX, maxY - minY};
    }

    // Helper function to get the AABB of a potentially rotated peg.
    Rectangle PegAABB(const Peg& peg) {
        if (peg.rotationDeg == 0.0f) {
            return {peg.center.x - peg.halfExtents.x, peg.center.y - peg.halfExtents.y, peg.halfExtents.x * 2, peg.halfExtents.y * 2};
        }
        Vector2 x = {cosf(peg.rotationDeg * DEG2RAD), sinf(peg.rotationDeg * DEG2RAD)};
        Vector2 y = {-x.y, x.x};
        float ex = peg.halfExtents.x;
        float ey = peg.halfExtents.y;
        float px = ex * fabsf(x.x) + ey * fabsf(y.x);
        float py = ex * fabsf(x.y) + ey * fabsf(y.y);
        return {peg.center.x - px, peg.center.y - py, 2 * px, 2 * py};
    }

    // Forward declaration for the core narrow-phase logic.
    RayHit SegRoundedAABB(Vector2 s, Vector2 e, Rectangle R, float r);
}

// Puzzle Piece: Broad Phase Implementation
std::vector<size_t> GetCollisionCandidates(
    Vector2 ballPos, Vector2 ballVel, float ballRadius,
    const std::vector<Peg>& allPegs, float dt) {
    std::vector<size_t> out;
    out.reserve(allPegs.size());
    Rectangle sweptBallBox = SweptCircleAABB(ballPos, ballVel, ballRadius, dt);
    for (size_t i = 0; i < allPegs.size(); ++i) {
        if (CheckCollisionRecs(sweptBallBox, PegAABB(allPegs[i]))) {
            out.push_back(i);
        }
    }
    return out;
}

// Puzzle Piece: Narrow Phase Implementation
RayHit IntersectMovingCirclePeg(
    Vector2 circlePos, Vector2 circleVel, float circleRadius,
    const Peg& peg, float dt) {
    Vector2 s = circlePos;
    Vector2 e = Vector2Add(circlePos, Vector2Scale(circleVel, dt));

    // --- Act I: AABB Collision ---
    // If the peg isn't rotated, we can treat it as a simple AABB.
    // This is the only logic we've provided!
    if (peg.rotationDeg == 0.0f) {
        Rectangle localRect = {peg.center.x - peg.halfExtents.x, peg.center.y - peg.halfExtents.y,
                               peg.halfExtents.x * 2.0f, peg.halfExtents.y * 2.0f};
        return SegRoundedAABB(s, e, localRect, circleRadius);
    }

    // --- Act II: OBB Collision (YOUR TASK!) ---
    // For rotated pegs, the simple AABB check above is incorrect.
    // The key idea is to transform the ball's motion into the peg's
    // "local space", where the peg is not rotated. Then you can use the
    // existing SegRoundedAABB function and transform the results back to world space.
    //
    // PSEUDOCODE GUIDE WITH CODE HINTS:
    //
    // 1. **Go to Local Space:**
    //    - Get the angle to "un-rotate" the world into the peg's perspective.
    //      float angleRad = -peg.rotationDeg * DEG2RAD;
    //
    //    - Transform the ball's start position into the peg's local space.
    //      Hint: Use Vector2Subtract to find the vector from peg to ball, then Vector2Rotate.
    //      Vector2 localS = Vector2Rotate(Vector2Subtract(s, peg.center), angleRad);
    //
    //    - Do the same for the ball's end position.
    //      Vector2 localE = ... your code here ...
    //
    // 2. **Perform AABB Check in Local Space:**
    //    - Define the peg's rectangle, now centered at the origin {0,0}.
    //      Rectangle localPegRect = { -peg.halfExtents.x, -peg.halfExtents.y, peg.halfExtents.x * 2.0f, peg.halfExtents.y * 2.0f };
    //
    //    - Call `SegRoundedAABB` with your new local-space variables.
    //      RayHit localHit = SegRoundedAABB(localS, localE, localPegRect, circleRadius);
    //
    // 3. **Go back to World Space:**
    //    - If the check in local space found a hit:
    //      if (localHit.hit) {
    //          // Rotate the normal back to world space. Use the *negative* of the angle
    //          // you used before to reverse the rotation.
    //          localHit.normal = Vector2Rotate(localHit.normal, -angleRad);
    //
    //          // Rotate the contact point back and then translate it to the peg's world position.
    //          // Hint: Use Vector2Rotate first, then Vector2Add.
    //          localHit.point = Vector2Add(Vector2Rotate(localHit.point, -angleRad), peg.center);
    //
    //          // Don't forget to return the final, corrected hit information!
    //          return localHit;
    //      }

    // Return a 'no hit' for now for rotated pegs until you implement the logic above.
    return {false};
}


namespace { // Anonymous namespace for internal helper functions
    RayHit SegAABB(Vector2 s, Vector2 e, Rectangle R) {
        Vector2 d = Vector2Subtract(e, s);
        float tmin = 0.0f, tmax = 1.0f; Vector2 n{0, 0};
        if (fabsf(d.x) > EPSILON) {
            float t1=(R.x-s.x)/d.x, t2=(R.x+R.width-s.x)/d.x;
            float enter=std::min(t1,t2), exit=std::max(t1,t2);
            Vector2 nx=(d.x>0)?Vector2{-1,0}:Vector2{1,0};
            if (enter>tmin){tmin=enter;n=nx;} tmax=std::min(tmax,exit); if(tmin>tmax) return {false};
        } else if (s.x<R.x || s.x>R.x+R.width) return {false};
        if (fabsf(d.y) > EPSILON) {
            float t1=(R.y-s.y)/d.y, t2=(R.y+R.height-s.y)/d.y;
            float enter=std::min(t1,t2), exit=std::max(t1,t2);
            Vector2 ny=(d.y>0)?Vector2{0,-1}:Vector2{0,1};
            if (enter>tmin){tmin=enter;n=ny;} tmax=std::min(tmax,exit); if(tmin>tmax) return {false};
        } else if (s.y<R.y || s.y>R.y+R.height) return {false};
        if (tmin<0.0f || tmin>1.0f) return {false};
        return {true, tmin, Vector2Add(s, Vector2Scale(d, tmin)), n};
    }

    RayHit SegCircle(Vector2 s, Vector2 e, Vector2 q, float r) {
        Vector2 v = Vector2Subtract(e, s), f = Vector2Subtract(s, q);
        float a = Vector2DotProduct(v, v); if (a < EPSILON) return {false};
        float b = 2.0f * Vector2DotProduct(v, f);
        float c = Vector2DotProduct(f, f) - r*r;
        if (c <= 0.0f) return {true, 0.0f, s, Vector2Normalize(Vector2Subtract(s, q))};
        double D = (double)b*b - 4.0*a*c; if (D < 0.0) return {false};
        double t0 = (-b - std::sqrt(D)) / (2.0*a); if (t0<0.0 || t0>1.0) return {false};
        Vector2 p = Vector2Add(s, Vector2Scale(v, (float)t0));
        return {true, (float)t0, p, Vector2Normalize(Vector2Subtract(p, q))};
    }

    bool OnFlatFace(const RayHit& h, const Rectangle& R) {
        if (fabsf(h.normal.x) > 0.5f) return h.point.y >= R.y-EPSILON && h.point.y <= R.y+R.height+EPSILON;
        return h.point.x >= R.x - EPSILON && h.point.x <= R.x + R.width + EPSILON;
    }
    
    // A moving circle vs. an AABB is equivalent to casting the circle's center
    // against a "rounded rectangle" (the AABB expanded by the circle's radius).
    // We can decompose this rounded rectangle into its flat faces and rounded corners.
    RayHit SegRoundedAABB(Vector2 s, Vector2 e, Rectangle R, float r) {
        // Start with a sentinel hit record. Any valid hit (t <= 1.0) will be "better".
        RayHit bestHit{false, 2.0f};

        // --- 1. Test against the flat faces ---
        // We can test against the four faces by intersecting the motion segment
        // with an AABB that has been expanded by the circle's radius.
        Rectangle expandedRect = {R.x - r, R.y - r, R.width + 2 * r, R.height + 2 * r};
        RayHit faceHit = SegAABB(s, e, expandedRect);

        // A hit on the expanded box is only a valid "face hit" if the contact point
        // lies on the flat part of the original rectangle's edge.
        if (faceHit.hit && OnFlatFace(faceHit, R)) {
            // If this is the earliest hit we've found, store it.
            if (faceHit.t < bestHit.t) {
                bestHit = faceHit;
            }
        }

        // --- 2. Test against the four corner circles ---
        // The rounded corners of the shape are just circles centered at the
        // original AABB's corners, with the same radius as our moving circle.
        Vector2 corners[4] = {
            {R.x, R.y},                             // Top-left
            {R.x + R.width, R.y},                   // Top-right
            {R.x, R.y + R.height},                  // Bottom-left
            {R.x + R.width, R.y + R.height}         // Bottom-right
        };

        for (const auto& cornerCenter : corners) {
            RayHit cornerHit = SegCircle(s, e, cornerCenter, r);
            // If this corner collision is the earliest one so far, store it.
            if (cornerHit.hit && cornerHit.t < bestHit.t) {
                bestHit = cornerHit;
            }
        }

        // --- 3. Finalize the result ---
        // After checking all candidates (faces and corners), if our best `t`
        // is within the valid frame time [0, 1], then we have a valid collision.
        if (bestHit.t <= 1.0f) {
            bestHit.hit = true;
        } else {
            bestHit.hit = false; // Ensure hit is false if t > 1.0
        }

        return bestHit;
    }
}

