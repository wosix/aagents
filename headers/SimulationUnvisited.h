#include <raylib.h>
#include <set>
#include <cstring>
#include "Simulation.h"
#include "Grid.h"
#include "Agent.h"

class SimulationUnvisited : public Simulation
{
public:
    using Simulation::Simulation;

    void update() override;

    void planMove(Agent &agent);
    void makeMove(Agent &agent);

    bool hasAgentsVisitedAllPoints();
    void setRandomTarget(Agent &agent);
    void setUnvisitedTarget(Agent &agent);
};

void SimulationUnvisited::update()
{

    printf("=== UPDATE Iteracja: %d ===\n", getIteration());

    if (hasAgentsVisitedAllPoints())
    {
        printf("Wszystkie punkty odwiedzone!\n");
        // ... reszta kodu ...
        return;
    }

    // DEBUG: Stan agentów
    printf("Stan agentów przed planowaniem:\n");
    for (int i = 0; i < getAgentSize(); i++)
    {
        Agent &agent = getAgent(i);
        printf("Agent %d: target=%d, reachedTarget=%d\n",
               i, agent.getTargetId(), agent.hasReachedTarget());
    }

    exchangeVisitedBetweenNeighbors();

    // FAZA 1: PLANOWANIE - tylko jeśli wszyscy dotarli do celów
    bool allReached = everyAgentHasReachedTarget();
    printf("everyAgentHasReachedTarget() = %d\n", allReached);

    if (allReached)
    {
        printf("WSZYSCY DOTARLI - PLANUJEMY!\n");
        for (int i = 0; i < getAgentSize(); i++)
        {
            Agent &agent = getAgent(i);
            planMove(agent);
        }
    }

    // FAZA 2: WYKONANIE
    printf("WYKONUJEMY RUCH:\n");
    for (int i = 0; i < getAgentSize(); i++)
    {
        Agent &agent = getAgent(i);
        makeMove(agent);
        printf("Agent %d po makeMove: target=%d, reached=%d\n",
               i, agent.getTargetId(), agent.hasReachedTarget());
    }

    addIteration();
    printf("=== KONEC UPDATE ===\n\n");
}

void SimulationUnvisited::planMove(Agent &agent)
{

    // Tylko jeśli agent nie ma celu LUB dotarł do celu
    if (!agent.hasTarget() || agent.hasReachedTarget())
    {
        Vertex &current = grid.getVertex(agent.getCurrentPointId());
        vector<int> neighbors = current.getNeighbors();

        // TYLKO WOLNE WIERZCHOŁKI
        vector<int> available;
        for (int neighborId : neighbors)
        {
            if (!grid.isVertexBusy(neighborId))
            {
                available.push_back(neighborId);
            }
        }

        if (!available.empty())
        {
            int randomIndex = GetRandomValue(0, available.size() - 1);
            int chosenTarget = available[randomIndex];

            // OD RAZU ZAREZERWUJ (żeby inni nie wybrali tego samego)
            if (grid.reserveVertex(chosenTarget, agent.getId()))
            {
                agent.setTargetId(chosenTarget);
            }
        }
        else
        {
            // Brak dostępnych - zostaje bez celu
            agent.setTargetId(-1);
            agent.setReachedTarget(true); // "Dotarł" do braku celu
        }
    }
}

void SimulationUnvisited::makeMove(Agent &agent)
{
    if (agent.hasTarget() && !agent.hasReachedTarget())
    {
        agent.moveToTarget();
    }
}

bool SimulationUnvisited::hasAgentsVisitedAllPoints()
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

void SimulationUnvisited::setRandomTarget(Agent &agent)
{
    // Vertex currentVertex = grid.getVertex(agent.getCurrentPointId());
    // vector<int> neighbors = currentVertex.getNeighbors();

    vector<int> available = getAvailablePointIds(agent.getCurrentPointId());

    if (!available.empty())
    {
        int randomIndex = GetRandomValue(0, available.size() - 1);
        agent.setTargetId(available[randomIndex]);
    }
    else
    {
        agent.setTargetId(-1);
    }

    // int randomIndex = GetRandomValue(0, neighbors.size() - 1);
    // int nextTarget = neighbors[randomIndex];
    // agent.setTargetId(nextTarget);
}

void SimulationUnvisited::setUnvisitedTarget(Agent &agent)
{
    // Vertex currentVertex = grid.getVertex(agent.getCurrentPointId());
    // vector<int> neighbors = currentVertex.getNeighbors();

    vector<int> available = getAvailablePointIds(agent.getCurrentPointId());

    vector<int> unvisited = agent.findUnvisited(available);

    if (!unvisited.empty())
    {
        int randomIndex = GetRandomValue(0, available.size() - 1);
        agent.setTargetId(unvisited[randomIndex]);
    }
    else
    {
        agent.setTargetId(-1);
    }

    // int randomIndex = GetRandomValue(0, unvisited.size() - 1);
    // int nextTarget = unvisited[randomIndex];
    // agent.setTargetId(nextTarget);
}
