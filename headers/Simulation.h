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
    int iter = 1;

public:
    Simulation();
    Simulation(Grid grid, int agentCount);
    void update();
    bool hasAgentsVisitedAllPoints();
    void goToRandomPointA(int id);
    void goToUnvisitedPointA(int id);
    set<int> getVisitedTogether();


    void reset();
    void draw();
};

Simulation::Simulation() {}

Simulation::Simulation(Grid grid, int agentCount) : grid(grid)
{
    for (int i = 0; i < agentCount; i++)
    {
        int randomIndex = GetRandomValue(0, grid.getSize() - 1);
        Vertex startPoint = grid.getPoint(randomIndex);

        Agent agent = Agent(randomIndex, startPoint.getX(), startPoint.getY());
        agents.push_back(agent);
    }
}

void Simulation::update()
{

    if (hasAgentsVisitedAllPoints())
    {
        int ag = 1;
        char text[32] = "";
        printf("\n");
        printf("%d:", iter);
        for (Agent agent : agents)
        {
            printf("\ni%d: %d", ag, static_cast<int>(agent.getPathLength()));
            ag++;

            agent.reset();
        }
        printf("\n");
        iter++;
        // reset();
        WaitTime(1);
    }

    for (int i = 0; i < agents.size(); i++)
    {

        Agent agent = agents[i];

        if (agent.hasTarget())
        {
            goToRandomPointA(i);
            return;
        }

        Vertex currentTarget = grid.getPoint(agent.getTargetId());

        if (agents[i].move(currentTarget.getX(), currentTarget.getY()))
        {
            agents[i].setLocationId(agent.getTargetId());

            if (agents[i].hasVisitedAllNeighbors(currentTarget.getNeighbors()))
            {
                goToRandomPointA(i);
            }
            else
            {
                goToUnvisitedPointA(i);
            }
        }
    }
}

bool Simulation::hasAgentsVisitedAllPoints()
{
    set<int> visitedTogether = getVisitedTogether();
    for (int pointId : grid.getPointIds())
    {
        if (find(visitedTogether.begin(), visitedTogether.end(), pointId) == visitedTogether.end())
        {
            return false;
        }
    }
    return true;
}

void Simulation::goToRandomPointA(int id)
{
    int currentLocation = agents[id].getLocationId();
    Vertex currentVertex = grid.getPoint(currentLocation);

    vector<int> neighbors = currentVertex.getNeighbors();
    int randomIndex = GetRandomValue(0, neighbors.size() - 1);
    int nextTarget = neighbors[randomIndex];

    agents[id].setTargetId(nextTarget);
}

void Simulation::goToUnvisitedPointA(int id)
{
    int currentLocation = agents[id].getLocationId();
    Vertex currentVertex = grid.getPoint(currentLocation);

    vector<int> neighbors = currentVertex.getNeighbors();
    vector<int> unvisited = agents[id].findUnvisited(neighbors);
    int randomIndex = GetRandomValue(0, unvisited.size() - 1);
    int nextTarget = unvisited[randomIndex];

    agents[id].setTargetId(nextTarget);
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

void Simulation::reset()
{
    for (int i = 0; i < agents.size(); i++)
    {
        agents[i].reset();
    }
}

void Simulation::draw()
{
    grid.draw(getVisitedTogether());
    for (Agent agent : agents)
    {
        agent.draw();
    }
}