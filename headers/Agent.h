#pragma once
#include <iostream>
#include <raylib.h>
#include <vector>
#include <algorithm>
#include <string>
#include <cstring>
#include <cmath>
#include "Vertex.h"
#include "ColorManager.h"

#define AGENT_MOVE_SPEED 10

class Agent
{
private:
    Grid &grid;
    int id;
    int startPointId;
    int currentPointId;
    int x;
    int y;
    unordered_set<int> visited;
    float pathLength;
    int targetId;
    bool reachedTarget = true;
    Color color;
    Color visitedColor;

public:
    Agent(int agentId, int startPointId, Grid &grid, Color color);

    int getId();

    void setX(int val);
    void setY(int val);

    void setCurrentPointId(int pointId);
    int getCurrentPointId();

    void setLocation(int x, int y);

    unordered_set<int> getVisited();
    void addVisited(int pointId);
    bool hasVisitedVertex(int vertexId);
    bool hasVisitedAllNeighbors();
    bool hasVisitedAllPoints(unordered_set<int> ids);
    unordered_set<int> findUnvisited(unordered_set<int> neighbors);
    int getPathLength();
    int addPath(int);

    int getTargetId();
    void setTargetId(int pointId);
    bool hasTarget();

    bool hasReachedTarget() const;
    void setReachedTarget(bool value);

    void exchangeVisited(Agent &otherAgent);

    bool moveToTarget();

    bool move(int x, int y);
    void draw();
    void reset();
    ~Agent();

private:
    unordered_set<int> mergeVisited(const unordered_set<int> &base, const unordered_set<int> &toAdd);
    void setVisited(unordered_set<int> visited);
    void setVisitedColor(Color color);
    Color getVisitedColor();
};

Agent::Agent(int agentId, int startPointId, Grid &grid, Color agentColor) : id(agentId), startPointId(startPointId), grid(grid), color(agentColor)
{
    visited = {};
    Vertex &startVertex = grid.getVertex(startPointId);
    setX(startVertex.getX());
    setY(startVertex.getY());
    setCurrentPointId(startPointId);
    setTargetId(-1);
    grid.reserveVertex(startVertex.getId(), agentId);
    pathLength = 0;
    visitedColor = Brighten(color, 1.8);
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

unordered_set<int> Agent::getVisited() { return visited; }

void Agent::addVisited(int pointId)
{
    visited.insert(pointId);
}

bool Agent::hasVisitedVertex(int vertexId)
{
    return visited.find(vertexId) != visited.end();
}

bool Agent::hasVisitedAllNeighbors()
{
    unordered_set<int> neighbors = grid.getVertex(currentPointId).getNeighbors();
    for (int neighborId : neighbors)
    {
        if (visited.find(neighborId) == visited.end())
        {
            return false;
        }
    }
    return true;
}

bool Agent::hasVisitedAllPoints(unordered_set<int> pointsIds)
{
    for (int id : pointsIds)
    {
        if (visited.find(id) != visited.end())
        {
            return false;
        }
    }
    return true;
}

unordered_set<int> Agent::findUnvisited(unordered_set<int> neighbors)
{
    unordered_set<int> unvisited = {};
    for (int neighborId : neighbors)
    {
        if (visited.find(neighborId) == visited.end())
        {
            unvisited.insert(neighborId);
        }
    }
    return unvisited;
}

int Agent::getPathLength() { return pathLength; }

int Agent::getTargetId() { return targetId; }

bool Agent::hasTarget() { return targetId > -1; }

bool Agent::hasReachedTarget() const { return reachedTarget; }

void Agent::setReachedTarget(bool value) { reachedTarget = value; }

void Agent::setVisitedColor(Color color) { visitedColor = color; }

Color Agent::getVisitedColor() { return visitedColor; }

unordered_set<int> Agent::mergeVisited(const unordered_set<int> &base, const unordered_set<int> &toAdd)
{
    unordered_set<int> result = base;
    result.insert(toAdd.begin(), toAdd.end());
    return result;
}

void Agent::setVisited(unordered_set<int> newVisited) { visited = newVisited; }

void Agent::exchangeVisited(Agent &otherAgent)
{
    unordered_set<int> myVisited = getVisited();
    unordered_set<int> otherVisited = otherAgent.getVisited();

    unordered_set<int> mergedVisited = mergeVisited(myVisited, otherVisited);
    unordered_set<int> otherMergedVisited = mergeVisited(otherVisited, myVisited);

    setVisited(mergedVisited);
    otherAgent.setVisited(otherMergedVisited);

    Color mixedColor = MixColors(visitedColor, otherAgent.getVisitedColor());
    setVisitedColor(mixedColor);
    otherAgent.setVisitedColor(mixedColor);

    printf("Agent %d i Agent %d wymienili się visited!\n", id, otherAgent.getId());
}

bool Agent::moveToTarget()
{

    if (!hasTarget() || reachedTarget)
        return false;

    Vertex &target = grid.getVertex(targetId);
    bool reached = move(target.getX(), target.getY());

    if (reached)
    {
        double distanceTraveled = grid.getDistance(currentPointId, targetId);
        pathLength += distanceTraveled;
        grid.freeVertex(currentPointId);
        grid.reserveVertex(targetId, id);
        setCurrentPointId(targetId);
        reachedTarget = true;
    }

    return reached;
}

bool Agent::move(int targetX, int targetY)
{
    int speed = AGENT_MOVE_SPEED;

    float dx = targetX - x;
    float dy = targetY - y;

    float distance = sqrt(dx * dx + dy * dy);

    if (distance <= speed)
    {
        x = targetX;
        y = targetY;
        // pathLength += distance / 100.0f;
        return true;
    }

    float moveX = (dx / distance) * speed;
    float moveY = (dy / distance) * speed;

    x += static_cast<int>(moveX);
    y += static_cast<int>(moveY);

    // pathLength += speed / 100.0f;

    return (x == targetX && y == targetY);
}

void Agent::reset()
{
    visited.clear();
    pathLength = 0;
    setCurrentPointId(startPointId);
    Vertex startVertex = grid.getVertex(startPointId);
    setLocation(startVertex.getX(), startVertex.getY());
    setTargetId(-1);
    grid.reserveVertex(startVertex.getId(), id);
    visitedColor = Brighten(color, 1.8);
}

void Agent::draw()
{
    // char text[32] = "NIEZLY AGENT: ";
    // string length = to_string(static_cast<int>(pathLength));
    // strcat(text, length.c_str());

    // DrawText(text, 0, 0, 48, BLACK);

    for (int visitedId : visited)
    {
        Vertex vertex = grid.getVertex(visitedId);
        DrawCircle(vertex.getX(), vertex.getY(), 19, visitedColor);
    }

    DrawCircle(x, y, 15, color);
}

Agent::~Agent()
{
}