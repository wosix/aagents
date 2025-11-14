#pragma once
#include <iostream>
#include <vector>

using namespace std;

class Vertex
{
private:
    int id;
    int x;
    int y;
    vector<int> neighbors;
    int busyByAgentId = -1;

public:
    Vertex();
    Vertex(int id, int a, int b, vector<int> neighborIds);
    void setX(int val);
    void setY(int val);
    int getId() const;
    int getX() const;
    int getY() const;
    void addNeighbor(int vertexId);
    vector<int> getNeighbors() const;
    int getBusyByAgentId();
    void setBusyByAgentId(int agentId);
    bool isBusy();
    void free();
};

Vertex::Vertex() {}

Vertex::Vertex(int pointId, int a, int b, vector<int> neighborIds) : id(pointId), x(a), y(b), neighbors(neighborIds) {}

void Vertex::setX(int val)
{
    x = val;
}

void Vertex::setY(int val)
{
    y = val;
}

int Vertex::getId() const
{
    return id;
}

int Vertex::getX() const
{
    return x;
}

int Vertex::getY() const
{
    return y;
}

void Vertex::addNeighbor(int vertexId)
{
    neighbors.push_back(vertexId);
}

vector<int> Vertex::getNeighbors() const
{
    return neighbors;
}

int Vertex::getBusyByAgentId()
{
    return busyByAgentId;
}

void Vertex::setBusyByAgentId(int agentId)
{
    busyByAgentId = agentId;
}

bool Vertex::isBusy()
{
    return busyByAgentId > -1;
}

void Vertex::free()
{
    busyByAgentId = -1;
}