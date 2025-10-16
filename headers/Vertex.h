#pragma once
#include <iostream>
#include <vector>

using namespace std;

class Vertex
{

private:
    int x;
    int y;
    vector<int> neighbors;

public:
    Vertex();
    Vertex(int a, int b, vector<int> neighborIds);
    void setX(int val);
    void setY(int val);
    int getX() const;
    int getY() const;
    void addNeighbor(int vertexId);
    vector<int> getNeighbors() const;
};

Vertex::Vertex() {}

Vertex::Vertex(int a, int b, vector<int> neighborIds) : x(a), y(b), neighbors(neighborIds) {}

void Vertex::setX(int val)
{
    x = val;
}

void Vertex::setY(int val)
{
    y = val;
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