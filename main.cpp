#include <iostream>
#include <raylib.h>
#include "Grid.h"
#include "SimulationUnvisited.h"
#include "SimulationRandom.h"
#include "Agent.h"

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 800
#define WINDOW_TITLE "autonomus agents"

using namespace std;

int main()
{

    Grid grid = Grid();
    grid = makeGrid(grid);

    // SimulationRandom simulation = SimulationRandom(grid, 1);
    SimulationUnvisited simulation = SimulationUnvisited(grid, 1);

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(60);

    while (!WindowShouldClose())
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
    CloseWindow();
}