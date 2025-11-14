#include <raylib.h>
#include <queue>
#include <unordered_set>
#include "Simulation.h"
#include "Grid.h"
#include "Agent.h"

class SimulationFrontier : public Simulation
{
private:
    vector<int> frontiers;

public:
    using Simulation::Simulation;
    string getName() override;
    void planMove(Agent &agent) override;

private:
    void updateFrontiers();
    vector<int> findPathToNearestFrontier(int startVertexId);
    int findNextStepToNearestFrontier(int currentVertexId);
    bool isVertexVisitedByAnyone(int vertexId);
};

string SimulationFrontier::getName()
{
    return "frontierBased";
}

void SimulationFrontier::planMove(Agent &agent)
{
    if (!agent.hasTarget() || agent.hasReachedTarget())
    {
        updateFrontiers();

        // Znajdź NASTĘPNY KROK na ścieżce do najbliższej granicy
        int nextStep = findNextStepToNearestFrontier(agent.getCurrentPointId());

        if (nextStep != -1 && grid.reserveVertex(nextStep, agent.getId()))
        {
            agent.setTargetId(nextStep);
            printf("Agent %d: następny krok do granicy: %d\n", agent.getId(), nextStep);
        }
        else
        {
            agent.setTargetId(-1);
            agent.setReachedTarget(true);
            printf("Agent %d: brak dostępnej ścieżki do granicy\n", agent.getId());
        }
    }
}

void SimulationFrontier::updateFrontiers()
{
    frontiers.clear();

    for (int i = 0; i < getAgentSize(); i++)
    {
        Agent &agent = getAgent(i);
        vector<int> visited = agent.getVisited();

        for (int visitedId : visited)
        {
            Vertex &vertex = grid.getVertex(visitedId);
            for (int neighborId : vertex.getNeighbors())
            {
                if (!isVertexVisitedByAnyone(neighborId))
                {
                    if (find(frontiers.begin(), frontiers.end(), neighborId) == frontiers.end())
                    {
                        frontiers.push_back(neighborId);
                    }
                }
            }
        }
    }

    printf("Frontiers: ");
    for (int f : frontiers)
        printf("%d ", f);
    printf("\n");
}

vector<int> SimulationFrontier::findPathToNearestFrontier(int startVertexId)
{
    // BFS do znalezienia najkrótszej ścieżki do najbliższej dostępnej granicy
    queue<vector<int>> q;
    unordered_set<int> visited;

    vector<int> startPath = {startVertexId};
    q.push(startPath);
    visited.insert(startVertexId);

    while (!q.empty())
    {
        vector<int> currentPath = q.front();
        q.pop();

        int currentVertex = currentPath.back();

        // sprawdzenie czy agent dotarł do dostępnej granicy
        if (find(frontiers.begin(), frontiers.end(), currentVertex) != frontiers.end() &&
            !grid.isVertexBusy(currentVertex))
        {
            return currentPath; // Znaleźliśmy ścieżkę
        }

        // dodanie sąsiadów do kolejki
        Vertex &vertex = grid.getVertex(currentVertex);
        for (int neighborId : vertex.getNeighbors())
        {
            if (visited.find(neighborId) == visited.end())
            {
                visited.insert(neighborId);

                vector<int> newPath = currentPath;
                newPath.push_back(neighborId);
                q.push(newPath);
            }
        }
    }

    return {};
}

int SimulationFrontier::findNextStepToNearestFrontier(int currentVertexId)
{
    vector<int> path = findPathToNearestFrontier(currentVertexId);

    if (path.size() > 1)
    {
        return path[1]; // Pierwszy krok na ścieżce (path[0] to currentVertex)
    }

    return -1;
}

bool SimulationFrontier::isVertexVisitedByAnyone(int vertexId)
{
    for (int i = 0; i < getAgentSize(); i++)
    {
        Agent &agent = getAgent(i);
        vector<int> visited = agent.getVisited();
        if (find(visited.begin(), visited.end(), vertexId) != visited.end())
        {
            return true;
        }
    }
    return false;
}