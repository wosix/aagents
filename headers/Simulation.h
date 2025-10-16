#include <raylib.h>
#include <queue>
#include <cstring>
#include "Grid.h"
#include "Agent.h"

class Simulation
{
private:
    Grid grid;
    Agent agent;
    int currentTargetId;
    bool hasTarget = false;

    int iter = 1;

    queue<int> pathQueue; // Kolejka celów do odwiedzenia

public:
    Simulation();
    Simulation(Grid grid, Agent agent);
    void reset();
    void update();
    void goToRandomPoint();
    void goToUnvisitedPoint();
    void draw();
};

Simulation::Simulation() {}

Simulation::Simulation(Grid grid, Agent agent) : grid(grid), agent(agent) {}

void Simulation::reset()
{
    hasTarget = false;
    agent = Agent(10, 600, 500);
}

void Simulation::update()
{

    if (agent.hasVisitedAllPoints(grid.getPointIds()))
    {
        char text[32] = "";
        // string iStr = to_string(iter);
        // strcat(text, iStr.c_str());
        // strcat(text, ":");
        // string length = to_string(agent.getPathLength());
        // strcat(text, length.c_str());


        printf("\n");
        printf("i%d: %d", iter, static_cast<int>(agent.getPathLength()));
        printf("\n");
        // DrawText(text, 50, 50, 80, BLUE);

        iter++;
        reset();



        // return;
    }

    if (!hasTarget)
    {
        // Jeśli nie mamy celu, zaplanuj nową ścieżkę
        goToRandomPoint();
        return;
    }

    Vertex currentTarget = grid.getPoint(currentTargetId);

    if (IsKeyDown(KEY_SPACE))
    {
        DrawText("SPACJA", 500, 350, 120, RED);
        reset();
    }

    // Przesuń agenta w kierunku aktualnego celu
    if (agent.move(currentTarget.getX(), currentTarget.getY()))
    {
        // Agent dotarł do celu - aktualizuj jego pozycję
        agent.setLocationId(currentTargetId);

        // Sprawdź czy są nowe nieodiwedzone punkty
        if (agent.hasVisitedAllNeighbors(currentTarget.getNeighbors()))
        {
            goToRandomPoint();
        }
        else
        {
            goToUnvisitedPoint();
        }
    }
}

void Simulation::goToRandomPoint()
{
    int currentLocation = agent.getLocationId();
    Vertex currentVertex = grid.getPoint(currentLocation);

    // Wybierz losowego sąsiada
    vector<int> neighbors = currentVertex.getNeighbors();
    int randomIndex = GetRandomValue(0, neighbors.size() - 1);
    int nextTarget = neighbors[randomIndex];

    // Ustaw nowy cel
    currentTargetId = nextTarget;
    hasTarget = true;
}

void Simulation::goToUnvisitedPoint()
{
    int currentLocation = agent.getLocationId();
    Vertex currentVertex = grid.getPoint(currentLocation);

    vector<int> neighbors = currentVertex.getNeighbors();

    vector<int> unvisited = agent.findUnvisited(neighbors);

    int randomIndex = GetRandomValue(0, unvisited.size() - 1);
    int nextTarget = unvisited[randomIndex];

    // Ustaw nowy cel
    currentTargetId = nextTarget;
    hasTarget = true;
}

void Simulation::draw()
{
    grid.draw(agent.getVisited());
    agent.draw();
}