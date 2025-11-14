#include <iostream>
#include <raylib.h>
#include "Grid.h"
#include "Agent.h"
#include "SimulationRandom.h"
#include "SimulationUnvisited.h"
#include "SimulationFrontier.h"

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 800
#define WINDOW_TITLE "autonomus agents"

using namespace std;

int main()
{
    SetRandomSeed((unsigned int)time(NULL));
    Grid grid = Grid();
    grid = makeGrid(grid);
    // grid = makeTestGrid(grid);

    // SimulationRandom simulation = SimulationRandom(grid, 1);
    // SimulationRandom simulation = SimulationRandom(grid, 2);
    // SimulationRandom simulation = SimulationRandom(grid, 4);
    // SimulationRandom simulation = SimulationRandom(grid, 8);

    // SimulationUnvisited simulation = SimulationUnvisited(grid, 1);
    // SimulationUnvisited simulation = SimulationUnvisited(grid, 2);
    // SimulationUnvisited simulation = SimulationUnvisited(grid, 4);
    SimulationUnvisited simulation = SimulationUnvisited(grid, 8);

    // SimulationFrontier simulation = SimulationFrontier(grid, 1);
    // SimulationFrontier simulation = SimulationFrontier(grid, 2);
    // SimulationFrontier simulation = SimulationFrontier(grid, 4);
    // SimulationFrontier simulation = SimulationFrontier(grid, 8);

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