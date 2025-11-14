#include <raylib.h>
#include "Simulation.h"
#include "Grid.h"
#include "Agent.h"

class SimulationRandom : public Simulation
{
public:
    using Simulation::Simulation;
    void planMove(Agent &agent) override;
};

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

        vector<int> unvisited = agent.findUnvisited(available);

        if (!unvisited.empty())
        {
            int randomIndex = GetRandomValue(0, unvisited.size() - 1);
            int chosenTarget = unvisited[randomIndex];

            if (grid.reserveVertex(chosenTarget, agent.getId()))
            {
                agent.setTargetId(chosenTarget);
            }
        }
        else
        {
            // Brak dostępnych - zostaje bez celu
            agent.setTargetId(-1);
            agent.setReachedTarget(true);
        }
    }
}
