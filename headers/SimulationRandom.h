#include <raylib.h>
#include <set>
#include <cstring>
#include "Simulation.h"
#include "Grid.h"
#include "Agent.h"

class SimulationRandom : public Simulation
{
public:
    using Simulation::Simulation;

    void update() override;

    bool hasAgentsVisitedAllPoints();
    void goToRandomPoint(Agent &agent);
};

void SimulationRandom::update()
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
            continue;
        }

        Vertex currentTarget = getPoint(agent.getTargetId());

        if (agent.move(currentTarget.getX(), currentTarget.getY()))
        {
            agent.setCurrentPoint(currentTarget);

            goToRandomPoint(agent);
        }
    }
}

bool SimulationRandom::hasAgentsVisitedAllPoints()
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

void SimulationRandom::goToRandomPoint(Agent &agent)
{
    Vertex currentVertex = agent.getCurrentPoint();
    vector<int> neighbors = currentVertex.getNeighbors();
    int randomIndex = GetRandomValue(0, neighbors.size() - 1);
    int nextTarget = neighbors[randomIndex];
    agent.setTargetId(nextTarget);
}
