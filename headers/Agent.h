#pragma once
#include <iostream>
#include <raylib.h>
#include <rcamera.h>
#include <vector>
#include <algorithm>
#include <string>
#include <cstring>
#include "Vertex.h"

#define AGENT_MOVE_SPEED 25

class Agent
{
private:
    int locationId;
    int x;
    int y;
    vector<int> visited;
    float pathLength;

public:
    Agent();
    Agent(int id, int a, int b);
    void setX(int val);
    void setY(int val);
    void setLocation(int x, int y);
    void setLocationId(int id);
    int getLocationId();
    vector<int> getVisited();
    void addVisited(int id);
    bool hasVisitedAllNeighbors(vector<int> neighbors);
    bool hasVisitedAllPoints(vector<int> ids);
    vector<int> findUnvisited(vector<int> neighbors);
    int getPathLength();
    bool move(int x, int y);
    void draw();
    ~Agent();
};

Agent::Agent()
{
}
Agent::Agent(int id, int a, int b) : locationId(id), x(a), y(b)
{
    pathLength = 0;
    visited = {};
    addVisited(id);
}

void Agent::setX(int val)
{
    x = val;
}

void Agent::setY(int val)
{
    y = val;
}

void Agent::setLocation(int x, int y)
{
    setX(x);
    setY(y);
}

void Agent::setLocationId(int id)
{
    locationId = id;
    addVisited(id);
}

int Agent::getLocationId()
{
    return locationId;
}

vector<int> Agent::getVisited()
{
    return visited;
}

void Agent::addVisited(int id)
{
    if (find(visited.begin(), visited.end(), id) == visited.end())
    {
        visited.push_back(id);
    }
}

bool Agent::hasVisitedAllNeighbors(vector<int> neighbors)
{
    for (int neighborId : neighbors)
    {
        if (find(visited.begin(), visited.end(), neighborId) == visited.end())
        {
            return false; // znaleziono przynajmniej jeden brakujący
        }
    }
    return true; // wszystkie elementy z vec1 są w vec2
}

bool Agent::hasVisitedAllPoints(vector<int> pointsIds)
{
    for (int id : pointsIds)
    {
        if (find(visited.begin(), visited.end(), id) == visited.end())
        {
            return false; // znaleziono przynajmniej jeden brakujący
        }
    }
    return true; // wszystkie elementy z vec1 są w vec2
}

vector<int> Agent::findUnvisited(vector<int> neighbors)
{
    vector<int> unvisited = {};
    for (int neighborId : neighbors)
    {
        if (find(visited.begin(), visited.end(), neighborId) == visited.end())
        {
            unvisited.push_back(neighborId); // zwraca pierwszy napotkany brakujący
        }
    }
    return unvisited; // zwraca -1 gdy nie ma brakujących
}

int Agent::getPathLength() {
    return pathLength;
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

void Agent::draw()
{
    DrawCircle(x, y, 15, ORANGE);

    char text[32] = "NIEZLY AGENT: ";
    string length = to_string(static_cast<int>(pathLength));
    strcat(text, length.c_str());

    DrawText(text, 0, 0 , 48, BLACK);
}

Agent::~Agent()
{
}
