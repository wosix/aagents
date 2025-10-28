#pragma once
#include <iostream>
#include <raylib.h>
#include <vector>
#include <algorithm>
#include <string>
#include <cstring>
#include <cmath>
#include "Vertex.h"

#define AGENT_MOVE_SPEED 3

class Agent
{
private:
    Vertex startingPoint;
    Vertex currentPoint;
    int x;
    int y;
    vector<int> visited;
    float pathLength;
    int targetId;

public:
    Agent();
    Agent(Vertex startingPoint);
    void setX(int val);
    void setY(int val);

    void setCurrentPoint(Vertex vertex);
    Vertex getCurrentPoint();

    void setLocation(int x, int y);

    vector<int> getVisited();
    void addVisited(int pointId);
    bool hasVisitedAllNeighbors(vector<int> neighbors);
    bool hasVisitedAllPoints(vector<int> ids);
    vector<int> findUnvisited(vector<int> neighbors);
    int getPathLength();

    int getTargetId();
    void setTargetId(int pointId);
    bool hasTarget();

    bool movee(int x, int y, int tx, int ty);
    bool move(int x, int y);
    void draw();
    void reset();
    ~Agent();
};

Agent::Agent()
{
}
Agent::Agent(Vertex startingPoint) : startingPoint(startingPoint)
{
    visited = {};
    setX(startingPoint.getX());
    setY(startingPoint.getY());
    setCurrentPoint(startingPoint);
    setTargetId(-1);
    pathLength = 0;
}

void Agent::setX(int val)
{
    x = val;
}

void Agent::setY(int val)
{
    y = val;
}

void Agent::setTargetId(int pointId)
{
    targetId = pointId;
}

void Agent::setCurrentPoint(Vertex point)
{
    currentPoint = point;
    addVisited(point.getId());
}

Vertex Agent::getCurrentPoint()
{
    return currentPoint;
}

void Agent::setLocation(int x, int y)
{
    setX(x);
    setY(y);
}

vector<int> Agent::getVisited()
{
    return visited;
}

void Agent::addVisited(int pointId)
{
    if (find(visited.begin(), visited.end(), pointId) == visited.end())
    {
        visited.push_back(pointId);
    }
}

bool Agent::hasVisitedAllNeighbors(vector<int> neighbors)
{
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

int Agent::getPathLength()
{
    return pathLength;
}

int Agent::getTargetId()
{
    return targetId;
}

bool Agent::hasTarget()
{
    return targetId >= 0;
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
    setCurrentPoint(startingPoint);
    setLocation(startingPoint.getX(), startingPoint.getY());
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
