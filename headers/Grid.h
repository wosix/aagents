#pragma once
#include <iostream>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <Vertex.h>
#include <raylib.h>
#include <rcamera.h>

using namespace std;

class Grid
{

private:
    unordered_map<int, Vertex> points;
    unordered_multimap<int, int> connections;

public:
    Grid() {}
    unordered_map<int, Vertex> getPoints();
    unordered_multimap<int, int> getConnections();
    vector<int> getPointIds();
    int getSize();
    Vertex getPoint(int id);
    void addPoint(int x, int y);
    void connectPoints(int id1, int id2);
    void draw(vector<int> visited);

    void print()
    {
        for (const auto &[id, vertex] : points)
        {
            std::cout << "Point " << id << " at (" << vertex.getX() << ", " << vertex.getY() << ") connected to: ";

            vector<int> neighbors = vertex.getNeighbors();
            for (int neighbor : neighbors)
            {
                std::cout << neighbor << " ";
            }
            std::cout << std::endl;
        }
    }
};

unordered_map<int, Vertex> Grid::getPoints()
{
    return points;
}

unordered_multimap<int, int> Grid::getConnections()
{
    return connections;
}

vector<int> Grid::getPointIds()
{
    vector<int> keys;
    for (const auto &pair : points)
    {
        keys.push_back(pair.first);
    }
    return keys;
}

int Grid::getSize()
{
    return static_cast<int>(points.size());
}

Vertex Grid::getPoint(int id)
{
    return points[id];
}

void Grid::addPoint(int x, int y)
{
    int pointId = getSize();
    points[pointId] = Vertex(x, y, {});
}

void Grid::connectPoints(int id1, int id2)
{
    points[id1].addNeighbor(id2);
    points[id2].addNeighbor(id1);
    connections.insert({id1, id2});
}

bool isVisited(const std::vector<int> &visited, int node)
{
    return find(visited.begin(), visited.end(), node) != visited.end();
}
void Grid::draw(vector<int> visited)
{
    for (const auto &[id, point] : points)
    {
        Color color = Color(RED);
        if (isVisited(visited, id))
        {
            color = Color(RAYWHITE);
        }

        DrawCircle(point.getX(), point.getY(), 20, color);
    }

    for (const auto connection : connections)
    {
        Vertex point1 = points[connection.first];
        Vertex point2 = points[connection.second];
        DrawLine(point1.getX(), point1.getY(), point2.getX(), point2.getY(), GREEN);
    }
}

Grid makeGrid(Grid grid)
{
    grid.addPoint(100, 100);
    grid.addPoint(300, 100);
    grid.addPoint(600, 100);
    grid.addPoint(900, 100);

    grid.addPoint(100, 300);
    grid.addPoint(300, 300);
    grid.addPoint(500, 300);
    grid.addPoint(900, 300);
    grid.addPoint(1100, 300);

    grid.addPoint(100, 500);
    grid.addPoint(600, 500);
    grid.addPoint(700, 500);
    grid.addPoint(1100, 500);

    grid.addPoint(200, 700);
    grid.addPoint(500, 700);
    grid.addPoint(700, 700);
    grid.addPoint(900, 700);
    grid.addPoint(1100, 700);

    grid.connectPoints(0, 1);
    grid.connectPoints(1, 2);
    grid.connectPoints(1, 5);
    grid.connectPoints(4, 5);
    grid.connectPoints(4, 9);
    grid.connectPoints(5, 6);
    grid.connectPoints(14, 6);
    grid.connectPoints(14, 13);
    grid.connectPoints(14, 15);
    grid.connectPoints(11, 15);
    grid.connectPoints(11, 10);
    grid.connectPoints(2, 3);
    grid.connectPoints(7, 3);
    grid.connectPoints(7, 6);
    grid.connectPoints(7, 8);
    grid.connectPoints(12, 8);
    grid.connectPoints(12, 17);
    grid.connectPoints(16, 17);

    return grid;
}