#include <raylib.h>
#include "Simulation.h"
#include "Grid.h"
#include "Agent.h"

class SimulationRandom : public Simulation
{
public:
    using Simulation::Simulation;
    string getName() override;
    void planMove(Agent &agent) override;
};

string SimulationRandom::getName()
{
    return "randomWalk";
}

void SimulationRandom::planMove(Agent &agent)
{
    // tylko jeśli agent nie ma celu LUB dotarł do celu
    if (!agent.hasTarget() || agent.hasReachedTarget())
    {
        Vertex &current = grid.getVertex(agent.getCurrentPointId());
        vector<int> neighbors = current.getNeighbors();

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
            int chosenTargetId = available[randomIndex];

            if (grid.reserveVertex(chosenTargetId, agent.getId()))
            {
                agent.setTargetId(chosenTargetId);
            }
            printf("Agent %d - from %d to %d", agent.getId(), current.getId(), chosenTargetId);
        }
        else
        {
            // Brak dostępnych - zostaje bez celu
            agent.setTargetId(-1);
            agent.setReachedTarget(true);
            printf("Agent %d - waits on %d", agent.getId(), current.getId());
        }
    }
}
