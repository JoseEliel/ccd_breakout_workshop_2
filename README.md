# Plinko Physics Workshop: An Introduction to Collision Detection

 Welcome to the Plinko Physics Workshop\! This repository contains the source code for a Plinko game built with C++ and the [Raylib](https://www.raylib.com/) library. This project is a hands-on exercise designed to guide you through implementing a **Continuous Collision Detection (CCD)** system from scratch.

Your task is to write the code that makes the ball bounce realistically off the pegs on the board.

## Workshop Objective

Your goal is to implement the `FindEarliestCollision` function, which you can find in the `src/workshop.cpp` file. This function is the core of the physics simulation. It is responsible for predicting if and when the moving ball will collide with any of the pegs on the board within a single frame.

For this workshop, you will focus on implementing collision detection between a moving circle (the ball) and static, axis-aligned squares (the pegs). This will teach you the fundamentals of CCD, including:

  - How to mathematically represent a moving object's path.
  - How to calculate the precise time of impact with a static object.
  - How to determine the surface normal of the collision point.
  - Understanding how this information is used to create a realistic bounce.

## How to Build and Run

This project is configured as a Visual Studio solution.

1.  Clone the repository to your local machine.
2.  Open the `Plinko.sln` file in Visual Studio.
3.  Make sure your build configuration is set to **Debug** and **x64**.
4.  Build the solution (F7 or Build \> Build Solution).
5.  Run the project (F5 or Debug \> Start Debugging).

The Raylib dependency is included in the `vendor/` directory, and the project is pre-configured with the correct paths and libraries. It should compile and run without any additional setup.

## How to Play

  - The ball will drop from the top of the screen automatically.
  - Press the **SPACE** key to reset the ball to its starting position.

## Codebase Overview

  - `src/main.cpp`: The main entry point of the application. It contains the game loop, rendering, and physics integration (like gravity). It calls your `FindEarliestCollision` function every frame to check for collisions. **For this workshop, ensure the `USE_ROTATED_PEGS` flag at the top of this file is set to `false`**.
  - `src/collision.hpp`: Defines the data structures used in the workshop, such as `Peg` and `RayHit`.
  - `src/workshop.cpp`: **This is where you will write your code\!** Your entire focus will be on implementing the body of the `FindEarliestCollision` function.
