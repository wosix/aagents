#include <raylib.h>
#include <queue>
#include <deque>
#include <unordered_set>
#include "Simulation.h"
#include "Grid.h"
#include "Agent.h"

class SimulationFrontier : public Simulation
{
private:
    unordered_map<int, unordered_set<int>> agentFrontiers;

public:
    using Simulation::Simulation;
    string getName() override;
    void planMove(Agent &agent) override;
    void exchangeVisitedBetweenNeighbors() override;
    void reset() override;

private:
    void exchangeFrontiers(Agent &agent1, Agent &agent2);
    void updateFrontiers(Agent &agent);
    void addFrontier(int agentId, int vertexId);
    void removeFrontier(int agentId, int vertexId);
    deque<int> findPathToNearestFrontier(int agentId, int startVertexId);
    deque<int> reconstructPath(const unordered_map<int, int> &predecessors,
                               int startId, int targetId);
    void resetAgentFrontires();
};

string SimulationFrontier::getName()
{
    return "frontierBased";
}

void SimulationFrontier::planMove(Agent &agent)
{
    if (!agent.hasTarget() || agent.hasReachedTarget())
    {
        updateFrontiers(agent);

        deque<int> path = findPathToNearestFrontier(agent.getId(), agent.getCurrentPointId());

        if (!path.empty())
        {
            int nextStep = path.front();
            if (grid.reserveVertex(nextStep, agent.getId()))
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
        else
        {
            agent.setTargetId(-1);
            agent.setReachedTarget(true);
            printf("Agent %d: brak dostępnej ścieżki do granicy\n", agent.getId());
        }
    }
}

void SimulationFrontier::exchangeVisitedBetweenNeighbors()
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
                printf("Agent %d i Agent %d są sąsiadami - wymieniamy visited!\n",
                       agent1.getId(), agent2.getId());

                agent1.exchangeVisited(agent2);
                exchangeFrontiers(agent1, agent2);
                exchangeCounter++;
            }
        }
    }
}

void SimulationFrontier::updateFrontiers(Agent &agent)
{
    int currentVertexId = agent.getCurrentPointId();
    Vertex &vertex = grid.getVertex(currentVertexId);
    for (int neighborId : vertex.getNeighbors())
    {
        if (!agent.hasVisitedVertex(neighborId))
        {
            addFrontier(agent.getId(), neighborId);
        }
        else
        {
            removeFrontier(agent.getId(), neighborId);
        }
    }
}

void SimulationFrontier::addFrontier(int agentId, int vertexId)
{
    agentFrontiers[agentId].insert(vertexId);
}

void SimulationFrontier::removeFrontier(int agentId, int vertexId)
{
    agentFrontiers[agentId].erase(vertexId);
}

deque<int> SimulationFrontier::findPathToNearestFrontier(int agentId, int startVertexId)
{
    auto &frontiers = agentFrontiers[agentId];
    auto allVisitedByAgent = getAgent(agentId).getVisited();

    if (frontiers.empty())
    {
        return {};
    }

    unordered_map<int, double> distances;
    unordered_map<int, int> predecessors;
    unordered_set<int> treated;

    for (int i = 0; i < grid.getSize(); i++)
    {
        distances[i] = numeric_limits<double>::max();
    }
    distances[startVertexId] = 0.0;

    struct CompareCost
    {
        bool operator()(const pair<double, int> &a, const pair<double, int> &b)
        {
            return a.first > b.first;
        }
    };

    using QueueElement = pair<double, int>;
    priority_queue<QueueElement, vector<QueueElement>, CompareCost> pq(CompareCost{});
    pq.push({0.0, startVertexId});

    int bestFrontier = -1;
    double bestCost = numeric_limits<double>::max();

    while (!pq.empty())
    {
        auto [currentCost, currentVertex] = pq.top();
        pq.pop();

        if (treated.count(currentVertex))
            continue;
        treated.insert(currentVertex);

        if (frontiers.count(currentVertex) && !grid.isVertexBusy(currentVertex))
        {
            if (currentCost < bestCost)
            {
                bestCost = currentCost;
                bestFrontier = currentVertex;
            }
        }

        if (allVisitedByAgent.count(currentVertex))
        {
            Vertex &vertex = grid.getVertex(currentVertex);
            for (int neighborId : vertex.getNeighbors())
            {
                if (treated.count(neighborId))
                    continue;

                if (allVisitedByAgent.count(currentVertex))
                {
                    double edgeCost = grid.getDistance(currentVertex, neighborId);
                    double newCost = currentCost + edgeCost;
                    if (newCost < distances[neighborId])
                    {
                        distances[neighborId] = newCost;
                        predecessors[neighborId] = currentVertex;
                        pq.push({newCost, neighborId});
                    }
                }
            }
        }
    }

    if (bestFrontier != -1)
    {
        return reconstructPath(predecessors, startVertexId, bestFrontier);
    }

    return {};
}

deque<int> SimulationFrontier::reconstructPath(const unordered_map<int, int> &predecessors,
                                               int startId, int targetId)
{
    deque<int> path;
    int current = targetId;

    while (current != startId)
    {
        path.push_front(current);
        auto it = predecessors.find(current);
        if (it == predecessors.end())
        {
            return {};
        }
        current = it->second;
    }
    return path;
}

void SimulationFrontier::exchangeFrontiers(Agent &agent1, Agent &agent2)
{
    auto &frontiers = agentFrontiers[agent1.getId()];
    auto &neigborFrontiers = agentFrontiers[agent2.getId()];
    frontiers.merge(neigborFrontiers);

    unordered_set<int> visited1 = agent1.getVisited();
    unordered_set<int> visited2 = agent2.getVisited();

    visited1.merge(visited2);

    // unordered_set<int> mergedSet = visited1;
    // mergedSet.insert(visited2.begin(), visited2.end());

    for (int visitedId : visited1)
    {
        frontiers.erase(visitedId);
    }

    agentFrontiers[agent1.getId()] = frontiers;
    agentFrontiers[agent2.getId()] = frontiers;
}

void SimulationFrontier::reset()
{
    grid.freeAllVertex();
    for (int i = 0; i < agents.size(); i++)
    {
        agents[i].reset();
    }
    addIteration();
    resetExchangeCounter();
    resetAgentFrontires();
}

void SimulationFrontier::resetAgentFrontires()
{
    for (int i = 0; i < getAgentSize(); i++)
    {
        agentFrontiers[i] = {};
    }
}