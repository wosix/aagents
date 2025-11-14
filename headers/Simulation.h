#pragma once
#include <raylib.h>
#include <set>
#include <array>
#include <cstring>
#include "Grid.h"
#include "Agent.h"

const array<Color, 8> DEFAULT_COLORS = {
    ORANGE,
    BLUE,
    PINK,
    PURPLE,
    DARKBLUE,
    MAGENTA,
    GREEN,
    DARKPURPLE};

class Simulation
{
protected:
    Grid &grid;
    vector<Agent> agents;
    int iteration = 1;

public:
    Simulation(Grid &grid, int agentCount);
    virtual void update() = 0;

    bool everyAgentHasReachedTarget();

    Vertex &getPoint(int pointId);
    vector<int> getGridPointsIds();
    set<int> getVisitedTogether();
    vector<int> getAvailablePointIds(int pointId);

    int getAgentSize();
    vector<Agent> getAgents();
    Agent &getAgent(int agentId);

    int getIteration();
    void addIteration();

    void reset();
    void draw();
};

set<int> getRandomIds(int gridSize, int agentCount)
{
    SetRandomSeed(1);
    set<int> ids = {};
    while (ids.size() < agentCount)
    {
        int randomNumber = GetRandomValue(0, gridSize - 1);
        ids.insert(randomNumber);
    }
    return ids;
}

Simulation::Simulation(Grid &grid, int agentCount) : grid(grid)
{
    int agentId = 0;
    // to jest do testow
    set<int> startingIds = getRandomIds(grid.getSize(), agentCount);
    // set<int> startingIds = {0, 6};
    for (int id : startingIds)
    {
        Vertex &startPoint = grid.getVertex(id);
        Color color = DEFAULT_COLORS[agentId];
        Agent agent = Agent(agentId, startPoint.getId(), grid, color);
        agents.push_back(agent);
        agentId++;
    }
}

bool Simulation::everyAgentHasReachedTarget()
{
    // for (int i = 0; i < agents.size(); i++)
    // {
    //     if (!getAgent(i).hasReachedTarget())
    //     {
    //         return false;
    //     }
    // }
    // return true;

    printf("Sprawdzam everyAgentHasReachedTarget():\n");
    for (int i = 0; i < getAgentSize(); i++)
    {
        Agent &agent = getAgent(i);
        bool hasTarget = agent.hasTarget();
        bool reached = agent.hasReachedTarget();
        printf("  Agent %d: hasTarget=%d, reachedTarget=%d\n", i, hasTarget, reached);

        // Jeśli agent ma cel ale jeszcze nie dotarł - return false
        if (hasTarget && !reached)
        {
            printf("  -> Agent %d jeszcze nie dotarł do celu!\n", i);
            return false;
        }
    }
    printf("  -> WSZYSCY DOTARLI DO CELÓW!\n");
    return true;
}

Vertex &Simulation::getPoint(int pointId)
{
    return grid.getVertex(pointId);
}

vector<int> Simulation::getGridPointsIds()
{
    return grid.getPointIds();
}

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

vector<int> Simulation::getAvailablePointIds(int pointId)
{
    // Vertex currentVertex = grid.getVertex(pointId);
    // vector<int> neighbors = currentVertex.getNeighbors();

    // vector<int> available;
    // for (int neighborId : neighbors)
    // {
    //     if (!grid.isVertexBusy(neighborId))
    //     {
    //         available.push_back(neighborId);
    //     }
    // }
    // return available;

    printf("DEBUG: getAvailablePointIds dla pointId: %d\n", pointId);

    // ✅ DODAJ SPRAWDZENIE CZY VERTEX ISTNIEJE
    if (!grid.vertexExists(pointId))
    {
        printf("KRYTYCZNY BŁĄD: Vertex %d nie istnieje w getAvailablePointIds!\n", pointId);
        return {}; // zwróć pustą listę
    }

    Vertex currentVertex = grid.getVertex(pointId);
    vector<int> neighbors = currentVertex.getNeighbors();

    printf("Sąsiedzi vertex %d: ", pointId);
    for (int n : neighbors)
        printf("%d ", n);
    printf("\n");

    vector<int> available;
    for (int neighborId : neighbors)
    {
        // ✅ SPRAWDŹ CZY SĄSIAD ISTNIEJE
        if (!grid.vertexExists(neighborId))
        {
            printf("BŁĄD: Sąsiad %d nie istnieje!\n", neighborId);
            continue;
        }

        if (!grid.isVertexBusy(neighborId))
        {
            available.push_back(neighborId);
            printf("Vertex %d jest dostępny\n", neighborId);
        }
        else
        {
            printf("Vertex %d jest zajęty\n", neighborId);
        }
    }

    printf("Dostępni sąsiedzi: ");
    for (int a : available)
        printf("%d ", a);
    printf("\n");

    return available;
}

int Simulation::getAgentSize()
{
    return agents.size();
}

vector<Agent> Simulation::getAgents()
{
    return agents;
}

Agent &Simulation::getAgent(int agentId)
{
    return agents[agentId];
}

int Simulation::getIteration()
{
    return iteration;
}

void Simulation::addIteration()
{
    iteration++;
}

void Simulation::reset()
{
    for (int i = 0; i < agents.size(); i++)
    {
        agents[i].reset();
    }
    addIteration();
}

void Simulation::draw()
{
    grid.draw(getVisitedTogether());
    for (Agent agent : agents)
    {
        agent.draw();
    }
}