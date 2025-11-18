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

private:
    void exchangeFrontiers(Agent &agent1, Agent &agent2);
    void updateFrontiers(Agent &agent);
    void addFrontier(int agentId, int vertexId);
    void removeFrontier(int agentId, int vertexId);
    deque<int> findPathToNearestFrontier(int agentId, int startVertexId);
    double getEdgeCost(int fromVertexId, int toVertexId);
    deque<int> reconstructPath(const unordered_map<int, int> &predecessors,
                               int startId, int targetId);
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

    printf("=== WYMIANA VISITED ? ===\n");

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

                // Wymień visited
                agent1.exchangeVisited(agent2);
                exchangeFrontiers(agent1, agent2);
                exchangeCounter++;

                // DEBUG: pokaż wyniki wymiany
                printf("Po wymianie - Agent %d visited: ", agent1.getId());
                for (int v : agent1.getVisited())
                    printf("%d ", v);
                printf("\n");

                printf("Po wymianie - Agent %d visited: ", agent2.getId());
                for (int v : agent2.getVisited())
                    printf("%d ", v);
                printf("\n");
            }
        }
    }
    printf("===== =====\n");
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
    if (frontiers.empty())
    {
        return {};
    }

    // Struktury danych dla Dijkstry
    unordered_map<int, double> distances;
    unordered_map<int, int> predecessors;
    unordered_set<int> treated;

    // Inicjalizacja odległości
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

    // Kolejka priorytetowa: (koszt, vertexId)
    using QueueElement = pair<double, int>;
    priority_queue<QueueElement, vector<QueueElement>, CompareCost> pq(CompareCost{});
    pq.push({0.0, startVertexId});

    int bestFrontier = -1;
    double bestCost = numeric_limits<double>::max();

    while (!pq.empty())
    {
        auto [currentCost, currentVertex] = pq.top();
        pq.pop();

        // Pomijamy jeśli już przetworzony
        if (treated.count(currentVertex))
            continue;
        else
            treated.insert(currentVertex);

        // Sprawdzamy czy to frontier (i nie jest zajęty)
        if (frontiers.count(currentVertex) && !grid.isVertexBusy(currentVertex))
        {
            if (currentCost < bestCost)
            {
                bestCost = currentCost;
                bestFrontier = currentVertex;
            }
        }

        // Przetwarzamy sąsiadów
        Vertex &vertex = grid.getVertex(currentVertex);
        for (int neighborId : vertex.getNeighbors())
        {
            if (treated.count(neighborId))
                continue;

            double edgeCost = getEdgeCost(currentVertex, neighborId);
            double newCost = currentCost + edgeCost;

            if (newCost < distances[neighborId])
            {
                distances[neighborId] = newCost;
                predecessors[neighborId] = currentVertex;
                pq.push({newCost, neighborId});
            }
        }
    }

    // Rekonstrukcja ścieżki jeśli znaleziono frontier
    if (bestFrontier != -1)
    {
        return reconstructPath(predecessors, startVertexId, bestFrontier);
    }

    return {};
}

double SimulationFrontier::getEdgeCost(int fromVertexId, int toVertexId)
{
    // Możesz dostosować koszty w zależności od typu terenu
    // Na razie zakładamy koszt = 1 dla wszystkich krawędzi
    return 1.0;
}

deque<int> SimulationFrontier::reconstructPath(const unordered_map<int, int> &predecessors,
                                               int startId, int targetId)
{
    deque<int> path;
    // Startujemy od celu i idziemy wstecz do startu
    int current = targetId;

    while (current != startId)
    {
        path.push_front(current); // Dodaj na początek - O(1)
        // Znajdź poprzednika
        auto it = predecessors.find(current);
        if (it == predecessors.end())
        {
            // Nie znaleziono poprzednika - błąd w ścieżce
            return {};
        }
        current = it->second; // Przejdź do poprzednika
    }

    // Na końcu dodaj wierzchołek startowy
    // path.push_front(startId);

    return path;
}

void SimulationFrontier::exchangeFrontiers(Agent &agent1, Agent &agent2)
{
    auto &frontiers = agentFrontiers[agent1.getId()];
    auto &neigborFrontiers = agentFrontiers[agent2.getId()];
    frontiers.merge(neigborFrontiers);

    vector<int> visited1 = agent1.getVisited();
    vector<int> visited2 = agent2.getVisited();

    // vector<int> mergedVisited = copy(visited2.begin(), visited2.end(), back_inserter(visited1));

    unordered_set<int> mergedSet(visited1.begin(), visited1.end());
    mergedSet.insert(visited2.begin(), visited2.end());

    for (int visited : mergedSet)
    {
        frontiers.erase(visited);
    }

    agentFrontiers[agent1.getId()] = frontiers;
    agentFrontiers[agent2.getId()] = frontiers;
}