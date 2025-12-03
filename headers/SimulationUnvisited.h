#include <raylib.h>
#include "Simulation.h"
#include "Grid.h"
#include "Agent.h"

class SimulationUnvisited : public Simulation
{
public:
    using Simulation::Simulation;
    string getName() override;
    void planMove(Agent &agent) override;
};

string SimulationUnvisited::getName()
{
    return "unvisitedPriority";
}

void SimulationUnvisited::planMove(Agent &agent)
{
    if (!agent.hasTarget() || agent.hasReachedTarget())
    {
        Vertex &current = grid.getVertex(agent.getCurrentPointId());
        unordered_set<int> neighbors = current.getNeighbors();

        vector<int> available;
        for (int neighborId : neighbors)
        {
            if (!grid.isVertexBusy(neighborId))
            {
                available.push_back(neighborId);
            }
        }

        unordered_set<int> availableSet(available.begin(), available.end());
        unordered_set<int> unvisitedSet = agent.findUnvisited(availableSet);
        vector<int> unvisited(unvisitedSet.begin(), unvisitedSet.end());

        if (!unvisited.empty())
        {
            int randomIndex = GetRandomValue(0, unvisited.size() - 1);
            int chosenTarget = unvisited[randomIndex];

            if (grid.reserveVertex(chosenTarget, agent.getId()))
            {
                agent.setTargetId(chosenTarget);
            }
        }
        else if (!available.empty())
        {
            int randomIndex = GetRandomValue(0, available.size() - 1);
            int chosenTarget = available[randomIndex];

            if (grid.reserveVertex(chosenTarget, agent.getId()))
            {
                agent.setTargetId(chosenTarget);
            }
        }
        else
        {
            agent.setTargetId(-1);
            agent.setReachedTarget(true);
        }
    }
}
