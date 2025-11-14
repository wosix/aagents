#pragma once
#include <iostream>
#include <raylib.h>
#include <vector>
#include <algorithm>
#include <string>
#include <cstring>
#include <cmath>
#include "Vertex.h"

#define AGENT_MOVE_SPEED 1

class Agent
{
private:
    Grid &grid;
    int id;
    int startPointId;
    int currentPointId;
    int x;
    int y;
    vector<int> visited;
    float pathLength;
    int targetId;
    bool reachedTarget = true;

public:
    Agent(int agentId, int startPointId, Grid &grid);

    int getId();

    void setX(int val);
    void setY(int val);

    void setCurrentPointId(int pointId);
    int getCurrentPointId();

    void setLocation(int x, int y);

    vector<int> getVisited();
    void addVisited(int pointId);
    bool hasVisitedAllNeighbors();
    bool hasVisitedAllPoints(vector<int> ids);
    vector<int> findUnvisited(vector<int> neighbors);
    int getPathLength();

    int getTargetId();
    void setTargetId(int pointId);
    bool hasTarget();

    bool hasReachedTarget() const;
    void setReachedTarget(bool value);

    bool moveToTarget();

    bool move(int x, int y);
    void draw();
    void reset();
    ~Agent();
};

Agent::Agent(int agentId, int startPointId, Grid &grid) : id(agentId), startPointId(startPointId), grid(grid)
{
    visited = {};
    Vertex &startVertex = grid.getVertex(startPointId);
    setX(startVertex.getX());
    setY(startVertex.getY());
    setCurrentPointId(startPointId);
    setTargetId(-1);
    grid.reserveVertex(startVertex.getId(), agentId);
    pathLength = 0;
}

int Agent::getId() { return id; }
void Agent::setX(int val) { x = val; }
void Agent::setY(int val) { y = val; }

void Agent::setTargetId(int pointId)
{
    targetId = pointId;
    reachedTarget = false;
}

void Agent::setCurrentPointId(int pointId)
{
    if (!grid.vertexExists(pointId))
    {
        printf("Agent %d: nie można ustawić currentPointId na nieistniejący vertex %d\n", id, pointId);
        return;
    }
    currentPointId = pointId;
    addVisited(pointId);
}

int Agent::getCurrentPointId() { return currentPointId; }

void Agent::setLocation(int x, int y)
{
    setX(x);
    setY(y);
}

vector<int> Agent::getVisited() { return visited; }

void Agent::addVisited(int pointId)
{
    if (find(visited.begin(), visited.end(), pointId) == visited.end())
    {
        visited.push_back(pointId);
    }
}

bool Agent::hasVisitedAllNeighbors()
{
    vector<int> neighbors = grid.getVertex(currentPointId).getNeighbors();
    for (int neighborId : neighbors)
    {
        if (find(visited.begin(), visited.end(), neighborId) == visited.end())
        {
            return false;
        }
    }
    return true;
}

bool Agent::hasVisitedAllPoints(vector<int> pointsIds)
{
    for (int id : pointsIds)
    {
        if (find(visited.begin(), visited.end(), id) == visited.end())
        {
            return false;
        }
    }
    return true;
}

vector<int> Agent::findUnvisited(vector<int> neighbors)
{
    vector<int> unvisited = {};
    for (int neighborId : neighbors)
    {
        if (find(visited.begin(), visited.end(), neighborId) == visited.end())
        {
            unvisited.push_back(neighborId);
        }
    }
    return unvisited;
}

int Agent::getPathLength() { return pathLength; }
int Agent::getTargetId() { return targetId; }
bool Agent::hasTarget() { return targetId > -1; }
bool Agent::hasReachedTarget() const { return reachedTarget; }
void Agent::setReachedTarget(bool value) { reachedTarget = value; }

bool Agent::moveToTarget()
{

    if (!hasTarget() || reachedTarget)
        return false;

    Vertex &target = grid.getVertex(targetId);
    bool reached = move(target.getX(), target.getY());

    if (reached)
    {
        // Dotarł do celu - zajmij wierzchołek
        grid.freeVertex(currentPointId);
        grid.reserveVertex(targetId, id);
        setCurrentPointId(targetId);
        reachedTarget = true; // Oznacz że dotarł
    }

    return reached;

    // Vertex &target = grid.getVertex(targetId);
    // if (move(target.getX(), target.getY()))
    // {
    //     target.free();
    //     setCurrentPointId(targetId);
    //     setTargetId(-1);
    //     return true;
    // }
    // else
    // {
    //     return false;
    // }
}

bool Agent::move(int targetX, int targetY)
{
    int speed = AGENT_MOVE_SPEED;

    if (x < targetX)
        x = min(x + speed, targetX);
    else if (x > targetX)
        x = max(x - speed, targetX);

    if (y < targetY)
        y = min(y + speed, targetY);
    else if (y > targetY)
        y = max(y - speed, targetY);

    pathLength += speed / 100.0f;

    return (x == targetX && y == targetY);
}

void Agent::reset()
{
    visited.clear();
    pathLength = 0;
    targetId = -1;
    setCurrentPointId(startPointId);
    Vertex startPoint = grid.getVertex(startPointId);
    setLocation(startPoint.getX(), startPoint.getY());
}

void Agent::draw()
{
    DrawCircle(x, y, 15, ORANGE);

    char text[32] = "NIEZLY AGENT: ";
    string length = to_string(static_cast<int>(pathLength));
    strcat(text, length.c_str());

    DrawText(text, 0, 0, 48, BLACK);
}

Agent::~Agent()
{
}
