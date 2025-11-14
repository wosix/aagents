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
    vector<int> getGridPointsIds();
    set<int> getVisitedTogether();
    vector<int> getAvailablePointIds(int pointId);

    int getAgentSize();
    vector<Agent> getAgents();
    Agent &getAgent(int agentId);

    bool areAgentsNeighbors(Agent &agent1, Agent &agent2);
    void exchangeVisitedBetweenNeighbors();
    void resetExchangeCounter();

    int getIteration();
    void addIteration();

    virtual string getName() = 0;
    void saveSimulationToFile();

    void reset();
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
        WaitTime(1);
    }

    if (hasAgentsVisitedAllPoints())
    {
        printf("Wszystkie punkty odwiedzone!\n");
        saveSimulationToFile();
        return;
    }

    // DEBUG: Stan agentów
    // for (int i = 0; i < getAgentSize(); i++)
    // {
    //     Agent &agent = getAgent(i);
    //     printf("Agent %d: target=%d, reachedTarget=%d\n",
    //            i, agent.getTargetId(), agent.hasReachedTarget());
    // }

    // FAZA 1 - WYMIANA
    exchangeVisitedBetweenNeighbors();

    // FAZA 2 - PLANOWANIE
    bool allReached = everyAgentHasReachedTarget();

    if (allReached)
    {
        for (int i = 0; i < getAgentSize(); i++)
        {
            Agent &agent = getAgent(i);
            planMove(agent);
        }
    }

    // FAZA 3 - RUCH
    for (int i = 0; i < getAgentSize(); i++)
    {
        Agent &agent = getAgent(i);
        makeMove(agent);
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
    set<int> visitedTogether = getVisitedTogether();
    for (int pointId : getGridPointsIds())
    {
        if (find(visitedTogether.begin(), visitedTogether.end(), pointId) == visitedTogether.end())
        {
            return false;
        }
    }
    return true;
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

    for (int i = 0; i < getAgentSize(); i++)
    {
        Agent &agent = getAgent(i);
        bool hasTarget = agent.hasTarget();
        bool reached = agent.hasReachedTarget();

        // Jeśli agent ma cel ale jeszcze nie dotarł - return false
        if (hasTarget && !reached)
        {
            // printf("  -> Agent %d jeszcze nie dotarł do celu!\n", i);
            return false;
        }
    }
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

    // printf("DEBUG: getAvailablePointIds dla pointId: %d\n", pointId);

    // ✅ DODAJ SPRAWDZENIE CZY VERTEX ISTNIEJE
    if (!grid.vertexExists(pointId))
    {
        // printf("KRYTYCZNY BŁĄD: Vertex %d nie istnieje w getAvailablePointIds!\n", pointId);
        return {};
    }

    Vertex currentVertex = grid.getVertex(pointId);
    vector<int> neighbors = currentVertex.getNeighbors();

    // printf("Sąsiedzi vertex %d: ", pointId);
    // for (int n : neighbors)
    //     printf("%d ", n);
    // printf("\n");

    vector<int> available;
    for (int neighborId : neighbors)
    {
        if (!grid.vertexExists(neighborId))
        {
            // printf("BŁĄD: Sąsiad %d nie istnieje!\n", neighborId);
            continue;
        }

        if (!grid.isVertexBusy(neighborId))
        {
            available.push_back(neighborId);
            // printf("Vertex %d jest dostępny\n", neighborId);
        }
        else
        {
            // printf("Vertex %d jest zajęty\n", neighborId);
        }
    }

    // printf("Dostępni sąsiedzi: ");
    // for (int a : available)
    //     printf("%d ", a);
    // printf("\n");

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

    // Sprawdź czy agent2 jest sąsiadem agent1
    Vertex &v1 = grid.getVertex(agent1Vertex);
    vector<int> neighbors1 = v1.getNeighbors();
    bool isNeighbor = find(neighbors1.begin(), neighbors1.end(), agent2Vertex) != neighbors1.end();

    if (isNeighbor)
    {
        printf("Agent %d (vertex %d) i Agent %d (vertex %d) są sąsiadami\n",
               agent1.getId(), agent1Vertex, agent2.getId(), agent2Vertex);
    }

    return isNeighbor;
}

void Simulation::exchangeVisitedBetweenNeighbors()
{
    if (!everyAgentHasReachedTarget())
    {
        return;
    }

    printf("=== WYMIANA VISITED ===\n");

    for (int i = 0; i < getAgentSize(); i++)
    {
        for (int j = i + 1; j < getAgentSize(); j++)
        {
            Agent &agent1 = getAgent(i);
            Agent &agent2 = getAgent(j);

            if (areAgentsNeighbors(agent1, agent2))
            {
                printf("Agent %d i Agent %d są sąsiadami - wymieniamy visited!\n",
                       agent1.getId(), agent2.getId());

                // Wymień visited
                agent1.exchangeVisited(agent2);
                exchangeCounter++;

                // DEBUG: pokaż wyniki wymiany
                printf("Po wymianie - Agent %d visited: ", agent1.getId());
                for (int v : agent1.getVisited())
                    printf("%d ", v);
                printf("\n");

                printf("Po wymianie - Agent %d visited: ", agent2.getId());
                for (int v : agent2.getVisited())
                    printf("%d ", v);
                printf("\n");
            }
        }
    }
    printf("===== =====\n");
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