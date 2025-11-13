#pragma once
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
    int iteration = 1;

public:
    Simulation();
    Simulation(Grid grid, int agentCount);
    virtual void update() = 0;

    Vertex getPoint(int pointId);
    vector<int> getGridPointsIds();
    set<int> getVisitedTogether();

    int getAgentSize();
    vector<Agent> getAgents();
    Agent &getAgent(int agentId);

    int getIteration();
    void addIteration();

    void reset();
    void draw();
};

Simulation::Simulation() {}

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

Simulation::Simulation(Grid grid, int agentCount) : grid(grid)
{
    set<int> startingIds = getRandomIds(grid.getSize(), agentCount);
    for (int id : startingIds)
    {
        Vertex startPoint = grid.getPoint(id);
        Agent agent = Agent(startPoint);
        agents.push_back(agent);
    }
}

Vertex Simulation::getPoint(int pointId)
{
    return grid.getPoint(pointId);
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