#pragma once
#include <raylib.h>
#include <fstream>
#include <set>
#include <cstring>
#include "Grid.h"
#include "Agent.h"
#include "ColorManager.h"

class Simulation
{
protected:
    string name;
    Grid &grid;
    vector<Agent> agents;
    int iteration = 1;
    int exchangeCounter = 0;

public:
    Simulation(Grid &grid, int agentCount);
    void update();
    virtual void planMove(Agent &agent) = 0;
    void makeMove(Agent &agent);

    bool hasAgentsVisitedAllPoints();
    bool everyAgentHasReachedTarget();

    Vertex &getPoint(int pointId);
    unordered_set<int> getGridPointsIds();
    unordered_set<int> getVisitedTogether();
    unordered_set<int> getAvailablePointIds(int pointId);

    int getAgentSize();
    vector<Agent> getAgents();
    Agent &getAgent(int agentId);

    bool areAgentsNeighbors(Agent &agent1, Agent &agent2);
    virtual void exchangeVisitedBetweenNeighbors();
    void resetExchangeCounter();

    int getIteration();
    void addIteration();

    virtual string getName() = 0;
    void saveSimulationToFile();

    virtual void reset();
    void draw();
};

set<int> getRandomIds(int gridSize, int agentCount)
{
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
    set<int> startingIds = getRandomIds(grid.getSize(), agentCount);
    // do testow
    // set<int> startingIds = {
    //     0,
    // };
    for (int id : startingIds)
    {
        Vertex &startPoint = grid.getVertex(id);
        Color color = DEFAULT_COLORS[agentId];
        Agent agent = Agent(agentId, startPoint.getId(), grid, color);
        agents.push_back(agent);
        agentId++;
    }
}

void Simulation::update()
{
    // pause
    if (IsKeyPressed(KEY_SPACE))
    {
        WaitTime(5);
    }

    if (hasAgentsVisitedAllPoints())
    {
        printf("Wszystkie punkty odwiedzone!\n");
        saveSimulationToFile();
        return;
    }

    exchangeVisitedBetweenNeighbors();

    if (everyAgentHasReachedTarget())
    {
        for (int i = 0; i < getAgentSize(); i++)
        {
            Agent &agent = getAgent(i);
            planMove(agent);
        }
    }
    else
    {
        for (int i = 0; i < getAgentSize(); i++)
        {
            Agent &agent = getAgent(i);
            makeMove(agent);
        }
    }
}

void Simulation::makeMove(Agent &agent)
{
    if (agent.hasTarget() && !agent.hasReachedTarget())
    {
        agent.moveToTarget();
    }
}

bool Simulation::hasAgentsVisitedAllPoints()
{
    unordered_set<int> visitedTogether = getVisitedTogether();
    for (int pointId : getGridPointsIds())
    {
        if (visitedTogether.find(pointId) == visitedTogether.end())
        {
            return false;
        }
    }
    return true;
}

bool Simulation::everyAgentHasReachedTarget()
{
    for (int i = 0; i < getAgentSize(); i++)
    {
        Agent &agent = getAgent(i);
        bool hasTarget = agent.hasTarget();
        bool reached = agent.hasReachedTarget();

        if (hasTarget && !reached)
        {
            return false;
        }
    }
    return true;
}

Vertex &Simulation::getPoint(int pointId)
{
    return grid.getVertex(pointId);
}

unordered_set<int> Simulation::getGridPointsIds()
{
    return grid.getPointIds();
}

unordered_set<int> Simulation::getVisitedTogether()
{
    unordered_set<int> visitedTogether = {};
    for (Agent agent : agents)
    {
        for (int visitedId : agent.getVisited())
        {
            visitedTogether.insert(visitedId);
        }
    }
    return visitedTogether;
}

unordered_set<int> Simulation::getAvailablePointIds(int pointId)
{
    if (!grid.vertexExists(pointId))
    {
        return {};
    }

    Vertex currentVertex = grid.getVertex(pointId);
    unordered_set<int> neighbors = currentVertex.getNeighbors();

    unordered_set<int> available;
    for (int neighborId : neighbors)
    {
        if (!grid.vertexExists(neighborId))
        {
            continue;
        }

        if (!grid.isVertexBusy(neighborId))
        {
            available.insert(neighborId);
        }
    }

    return available;
}

int Simulation::getAgentSize() { return agents.size(); }

vector<Agent> Simulation::getAgents() { return agents; }

Agent &Simulation::getAgent(int agentId)
{
    return agents[agentId];
}

bool Simulation::areAgentsNeighbors(Agent &agent1, Agent &agent2)
{
    int agent1Vertex = agent1.getCurrentPointId();
    int agent2Vertex = agent2.getCurrentPointId();

    Vertex &v1 = grid.getVertex(agent1Vertex);
    unordered_set<int> neighbors1 = v1.getNeighbors();
    bool isNeighbor = neighbors1.find(agent2Vertex) != neighbors1.end();

    return isNeighbor;
}

void Simulation::exchangeVisitedBetweenNeighbors()
{
    if (!everyAgentHasReachedTarget())
    {
        return;
    }

    for (int i = 0; i < getAgentSize(); i++)
    {
        for (int j = i + 1; j < getAgentSize(); j++)
        {
            Agent &agent1 = getAgent(i);
            Agent &agent2 = getAgent(j);

            if (areAgentsNeighbors(agent1, agent2))
            {
                agent1.exchangeVisited(agent2);
                exchangeCounter++;
            }
        }
    }
}

int Simulation::getIteration()
{
    return iteration;
}

void Simulation::addIteration()
{
    iteration++;
}

void Simulation::resetExchangeCounter() { exchangeCounter = 0; }

void Simulation::saveSimulationToFile()
{
    int lengthCombined = 0;

    ofstream file(getName(), ios::app);
    if (file.is_open())
    {
        file << "=====================\n"
             << endl;
        file << "Iteracja - " << getIteration() << endl;
        for (Agent agent : getAgents())
        {
            file << "Agent " << agent.getId() << " przebył drogę: " << static_cast<int>(agent.getPathLength()) << endl;
            lengthCombined += agent.getPathLength();
        }
        file << "\nŁączna długość: " << lengthCombined << endl;
        file << "\nŚrednia długość: " << lengthCombined / getAgentSize() << endl;
        file << "\nLiczba wymian między agentami: " << exchangeCounter << endl;
        file << "\n=====================\n"
             << endl;

        file.close();
    }
    else
    {
        printf("ERR: Nie można otworzyć pliku!");
    }

    reset();
    WaitTime(1);
}

void Simulation::reset()
{
    grid.freeAllVertex();
    for (int i = 0; i < agents.size(); i++)
    {
        agents[i].reset();
    }
    addIteration();
    resetExchangeCounter();
}

void Simulation::draw()
{
    grid.draw(getVisitedTogether());
    for (Agent agent : agents)
    {
        agent.draw();
    }
}