#include <iostream>
#include <raylib.h>
#include <rcamera.h>
#include "Grid.h"
#include "Simulation.h"
#include "Agent.h"

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 800
#define WINDOW_TITLE "autonomus agents"

using namespace std;

int main()
{

    Grid grid = Grid();
    grid = makeGrid(grid);

    // Vertex startPoint = grid.getPoint(10);
    // Agent agent = Agent(10, startPoint.getX(), startPoint.getY());

    Simulation simulation = Simulation(grid, 5);


    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------

        simulation.update();

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(DARKGRAY);

        simulation.draw();

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------


}