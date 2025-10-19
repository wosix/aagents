#include <raylib.h>
#include <set>
#include <cstring>
#include "Grid.h"
#include "Agent.h"

class Simulation
{
private:
    Grid grid;
    vector<Agent> agents;
    vector<int> currentTargetIds;
    vector<bool> hasTargets;

    int iter = 1;

public:
    Simulation();
    Simulation(Grid grid, int agentCount);
    void reset();
    void update();
    bool hasAgentsVisitedAllPoints();
    void goToRandomPointA(int id);
    void goToUnvisitedPointA(int id);
    void goToRandomPoint();
    void goToUnvisitedPoint();
    set<int> getVisitedTogether();
    void draw();
};

Simulation::Simulation() {}

Simulation::Simulation(Grid grid, int agentCount) : grid(grid)
{
    for (int i = 0; i < agentCount; i++)
    {
        int randomIndex = GetRandomValue(0, grid.getSize() - 1);
        Vertex startPoint = grid.getPoint(randomIndex);

        Agent agent = Agent(randomIndex, startPoint.getX(), startPoint.getY());
        agents.push_back(agent);
        hasTargets.push_back(false);
        currentTargetIds.push_back(-1);
    }
}

void Simulation::reset()
{
    for (Agent agent : agents)
    {
        agent.reset();
    }
}

void Simulation::update()
{
    if (hasAgentsVisitedAllPoints())
    {
        int ag = 1;
        char text[32] = "";
        printf("\n");
        printf("%d:", iter);
        for (Agent agent : agents)
        {
            printf("i%d: %d", ag, static_cast<int>(agent.getPathLength()));
            ag ++;
        }
        printf("\n");
        iter++;
        reset();
    }

    for (int i = 0; i < agents.size(); i++)
    {
        if (!hasTargets[i])
        {
            // Jeśli nie mamy celu, zaplanuj nową ścieżkę
            goToRandomPointA(i);
            return;
        }

        Vertex currentTarget = grid.getPoint(currentTargetIds[i]);

        if (agents[i].move(currentTarget.getX(), currentTarget.getY()))
        {
            // Agent dotarł do celu - aktualizuj pozycję
            agents[i].setLocationId(currentTargetIds[i]);

            if (agents[i].hasVisitedAllNeighbors(currentTarget.getNeighbors()))
            {
                goToRandomPointA(i);
            }
            else
            {
                goToUnvisitedPointA(i);
            }
        }
    }
}

// void Simulation::update()
// {

//     if (agent.hasVisitedAllPoints(grid.getPointIds()))
//     {
//         char text[32] = "";
//         // string iStr = to_string(iter);
//         // strcat(text, iStr.c_str());
//         // strcat(text, ":");
//         // string length = to_string(agent.getPathLength());
//         // strcat(text, length.c_str());

//         printf("\n");
//         printf("i%d: %d", iter, static_cast<int>(agent.getPathLength()));
//         printf("\n");
//         // DrawText(text, 50, 50, 80, BLUE);

//         iter++;
//         reset();

//         // return;
//     }

//     if (!hasTarget)
//     {
//         // Jeśli nie mamy celu, zaplanuj nową ścieżkę
//         goToRandomPoint();
//         return;
//     }

//     Vertex currentTarget = grid.getPoint(currentTargetId);

//     if (IsKeyDown(KEY_SPACE))
//     {
//         DrawText("SPACJA", 500, 350, 120, RED);
//         reset();
//     }

//     // Przesuń agenta w kierunku aktualnego celu
//     if (agent.move(currentTarget.getX(), currentTarget.getY()))
//     {
//         // Agent dotarł do celu - aktualizuj jego pozycję
//         agent.setLocationId(currentTargetId);
//         WaitTime(1);

//         // Sprawdź czy są nowe nieodiwedzone punkty
//         if (agent.hasVisitedAllNeighbors(currentTarget.getNeighbors()))
//         {
//             goToRandomPoint();
//         }
//         else
//         {
//             goToUnvisitedPoint();
//         }
//     }
// }

bool Simulation::hasAgentsVisitedAllPoints()
{
    set<int> visitedTogether = {};
    for (Agent agent : agents)
    {
        for (int visitedId : agent.getVisited())
        {
            visitedTogether.insert(visitedId);
        }
    }

    for (int pointId : grid.getPointIds())
    {
        if (find(visitedTogether.begin(), visitedTogether.end(), pointId) == visitedTogether.end())
        {
            return false;
        }
    }
    return true;
}

void Simulation::goToRandomPointA(int id)
{
    int currentLocation = agents[id].getLocationId();
    Vertex currentVertex = grid.getPoint(currentLocation);

    // Wybierz losowego sąsiada
    vector<int> neighbors = currentVertex.getNeighbors();
    int randomIndex = GetRandomValue(0, neighbors.size() - 1);
    int nextTarget = neighbors[randomIndex];

    // Ustaw nowy cel
    currentTargetIds[id] = nextTarget;
    hasTargets[id] = true;
}

void Simulation::goToUnvisitedPointA(int id)
{
    int currentLocation = agents[id].getLocationId();
    Vertex currentVertex = grid.getPoint(currentLocation);

    vector<int> neighbors = currentVertex.getNeighbors();

    vector<int> unvisited = agents[id].findUnvisited(neighbors);

    int randomIndex = GetRandomValue(0, unvisited.size() - 1);
    int nextTarget = unvisited[randomIndex];

    // Ustaw nowy cel
    currentTargetIds[id] = nextTarget;
    hasTargets[id] = true;
}

// void Simulation::goToRandomPoint()
// {
//     int currentLocation = agent.getLocationId();
//     Vertex currentVertex = grid.getPoint(currentLocation);

//     // Wybierz losowego sąsiada
//     vector<int> neighbors = currentVertex.getNeighbors();
//     int randomIndex = GetRandomValue(0, neighbors.size() - 1);
//     int nextTarget = neighbors[randomIndex];

//     // Ustaw nowy cel
//     currentTargetId = nextTarget;
//     hasTarget = true;
// }

// void Simulation::goToUnvisitedPoint()
// {
//     int currentLocation = agent.getLocationId();
//     Vertex currentVertex = grid.getPoint(currentLocation);

//     vector<int> neighbors = currentVertex.getNeighbors();

//     vector<int> unvisited = agent.findUnvisited(neighbors);

//     int randomIndex = GetRandomValue(0, unvisited.size() - 1);
//     int nextTarget = unvisited[randomIndex];

//     // Ustaw nowy cel
//     currentTargetId = nextTarget;
//     hasTarget = true;
// }

set<int> Simulation::getVisitedTogether()
{
    set<int> visitedTogether = {};
    for (Agent agent : agents)
    {
        for (int visitedId : agent.getVisited())
        {
            visitedTogether.insert(visitedId);
        }
    }
    return visitedTogether;
}

void Simulation::draw()
{
    grid.draw(getVisitedTogether());
    for (Agent agent : agents)
    {
        agent.draw();
    }
}