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

    bool hasAgentsVisitedAllPoints();
    void goToRandomPoint(Agent &agent);
    void goToUnvisitedPoint(Agent &agent);
};

void SimulationUnvisited::update()
{
    if (hasAgentsVisitedAllPoints())
    {
        int ag = 1;
        char text[32] = "";
        printf("\n");
        printf("%d:", getIteration());
        for (Agent agent : getAgents())
        {
            printf("\ni%d: %d", ag, static_cast<int>(agent.getPathLength()));
            ag++;
        }
        printf("\n");

        reset();
        WaitTime(1);
        return;
    }

    for (int i = 0; i < getAgentSize(); i++)
    {

        Agent &agent = getAgent(i);

        if (!agent.hasTarget())
        {
            goToRandomPoint(agent);
            return;
        }

        Vertex currentTarget = getPoint(agent.getTargetId());

        if (agent.move(currentTarget.getX(), currentTarget.getY()))
        {
            agent.setCurrentPoint(currentTarget);

            if (agent.hasVisitedAllNeighbors(currentTarget.getNeighbors()))
            {
                goToRandomPoint(agent);
            }
            else
            {
                goToUnvisitedPoint(agent);
            }
        }
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

void SimulationUnvisited::goToRandomPoint(Agent &agent)
{
    Vertex currentVertex = agent.getCurrentPoint();
    vector<int> neighbors = currentVertex.getNeighbors();
    int randomIndex = GetRandomValue(0, neighbors.size() - 1);
    int nextTarget = neighbors[randomIndex];
    agent.setTargetId(nextTarget);
}

void SimulationUnvisited::goToUnvisitedPoint(Agent &agent)
{
    Vertex currentVertex = agent.getCurrentPoint();
    vector<int> neighbors = currentVertex.getNeighbors();
    vector<int> unvisited = agent.findUnvisited(neighbors);
    int randomIndex = GetRandomValue(0, unvisited.size() - 1);
    int nextTarget = unvisited[randomIndex];
    agent.setTargetId(nextTarget);
}
