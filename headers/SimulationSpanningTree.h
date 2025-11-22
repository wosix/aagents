#include <raylib.h>
#include <queue>
#include <map>
#include <deque>
#include <unordered_set>
#include "Simulation.h"
#include "Grid.h"
#include "Agent.h"

class SimulationSpanningTree : public Simulation
{
private:
    struct AgentMemory
    {
        map<int, set<pair<double, int>>> tree;
        unordered_set<int> edges;
    };
    unordered_map<int, AgentMemory> agentsKnolage;

public:
    using Simulation::Simulation;
    string getName() override;
    void planMove(Agent &agent) override;
    void exchangeVisitedBetweenNeighbors() override;
    void reset() override;

private:
    void exchangeTrees(Agent &agent1, Agent &agent2);
    void updateTrees(Agent &agent);
    unordered_set<int> findTreeEdges(int agentId);
    bool isVertexInTree(int agentId, int vertexId);
    bool isVertexInEdges(int agentId, int vertexId);
    deque<int> findPathToNearestUnvisited(int agentId, int vertexId);
    deque<int> reconstructPath(const unordered_map<int, int> &predecessors,
                               int startId, int targetId);
    unordered_set<int> getAllTreeVertices(int agentId);
    void resetAgentsMemory();
};

string SimulationSpanningTree::getName()
{
    return "spanningTree";
}

void SimulationSpanningTree::planMove(Agent &agent)
{

    if (!agent.hasTarget() || agent.hasReachedTarget())
    {
        updateTrees(agent);

        deque<int> path = findPathToNearestUnvisited(agent.getId(), agent.getCurrentPointId());

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

void SimulationSpanningTree::exchangeVisitedBetweenNeighbors()
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
                exchangeTrees(agent1, agent2);
                exchangeCounter++;
            }
        }
    }
}

void SimulationSpanningTree::updateTrees(Agent &agent)
{
    int agentId = agent.getId();
    int currentVertexId = agent.getCurrentPointId();

    if (!isVertexInTree(agentId, currentVertexId))
    {
        Vertex &vertex = grid.getVertex(currentVertexId);
        set<pair<double, int>> treeBranch = {};

        for (int neighborId : vertex.getNeighbors())
        {
            if (!isVertexInEdges(agentId, neighborId) && !agent.hasVisitedVertex(neighborId))
            {
                double distance = grid.getDistance(currentVertexId, neighborId);
                treeBranch.insert({distance, neighborId});
            }
        }

        agentsKnolage[agentId].tree.insert({currentVertexId, treeBranch});
    }

    agentsKnolage[agentId].edges = findTreeEdges(agentId);
}

unordered_set<int> SimulationSpanningTree::findTreeEdges(int agentId)
{
    map<int, set<pair<double, int>>> tree = agentsKnolage[agentId].tree;
    unordered_set<int> edges;

    for (auto &[vertex, neighbors] : tree)
    {
        for (auto &[cost, neighbor] : neighbors)
        {
            if (tree.find(neighbor) == tree.end())
            {
                edges.insert(neighbor);
            }
        }
    }

    return edges;
}

void SimulationSpanningTree::exchangeTrees(Agent &agent1, Agent &agent2)
{
    auto &mem1 = agentsKnolage[agent1.getId()];
    auto &mem2 = agentsKnolage[agent2.getId()];

    for (auto &[vertex, neighbors] : mem2.tree)
    {
        mem1.tree[vertex].insert(neighbors.begin(), neighbors.end());
    }
    for (auto &[vertex, neighbors] : mem1.tree)
    {
        mem2.tree[vertex].insert(neighbors.begin(), neighbors.end());
    }
}

bool SimulationSpanningTree::isVertexInTree(int agentId, int vertexId)
{
    AgentMemory agMem = agentsKnolage[agentId];
    return agMem.tree.find(vertexId) != agMem.tree.end();
}

bool SimulationSpanningTree::isVertexInEdges(int agentId, int vertexId)
{
    AgentMemory agMem = agentsKnolage[agentId];
    return agMem.edges.find(vertexId) != agMem.edges.end();
}

deque<int> SimulationSpanningTree::findPathToNearestUnvisited(int agentId, int vertexId)
{
    auto &memory = agentsKnolage[agentId];
    auto allTreeVertices = getAllTreeVertices(agentId);

    if (memory.edges.empty())
    {
        return {};
    }

    unordered_map<int, double> distances;
    unordered_map<int, int> predecessors;
    unordered_set<int> visited;

    for (int i = 0; i < grid.getSize(); i++)
    {
        distances[i] = numeric_limits<double>::max();
    }
    distances[vertexId] = 0.0;

    struct CompareCost
    {
        bool operator()(const pair<double, int> &a, const pair<double, int> &b)
        {
            return a.first > b.first;
        }
    };

    priority_queue<pair<double, int>, vector<pair<double, int>>, CompareCost> pq(CompareCost{});
    pq.push({0.0, vertexId});

    int nearestEdge = -1;

    while (!pq.empty())
    {
        auto [currentCost, currentVertex] = pq.top();
        pq.pop();

        if (visited.count(currentVertex))
            continue;
        visited.insert(currentVertex);

        if (memory.edges.count(currentVertex))
        {
            nearestEdge = currentVertex;
            break;
        }

        if (allTreeVertices.count(currentVertex))
        {
            Vertex &vertex = grid.getVertex(currentVertex);
            for (int neighborId : vertex.getNeighbors())
            {
                if (visited.count(neighborId))
                    continue;

                if (allTreeVertices.count(neighborId))
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

    if (nearestEdge != -1)
    {
        return reconstructPath(predecessors, vertexId, nearestEdge);
    }

    return {};
}

deque<int> SimulationSpanningTree::reconstructPath(const unordered_map<int, int> &predecessors,
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

unordered_set<int> SimulationSpanningTree::getAllTreeVertices(int agentId)
{
    auto &memory = agentsKnolage[agentId];
    unordered_set<int> allVertices;

    for (auto &[vertex, neighbors] : memory.tree)
    {
        allVertices.insert(vertex);
        for (auto &[distance, neighborId] : neighbors)
        {
            allVertices.insert(neighborId);
        }
    }
    return allVertices;
}

void SimulationSpanningTree::reset()
{
    grid.freeAllVertex();
    for (int i = 0; i < agents.size(); i++)
    {
        agents[i].reset();
    }
    addIteration();
    resetExchangeCounter();
    resetAgentsMemory();
}

void SimulationSpanningTree::resetAgentsMemory()
{
    for (int i = 0; i < getAgentSize(); i++)
    {
        agentsKnolage[i].tree = {};
        agentsKnolage[i].edges = {};
    }
}