#pragma once
#include <iostream>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <set>
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
    void draw(set<int> visited);
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
    points[pointId] = Vertex(pointId, x, y, {});
}

void Grid::connectPoints(int id1, int id2)
{
    points[id1].addNeighbor(id2);
    points[id2].addNeighbor(id1);
    connections.insert({id1, id2});
}

bool isVisited(const set<int> &visited, int node)
{
    return find(visited.begin(), visited.end(), node) != visited.end();
}
void Grid::draw(set<int> visited)
{
    for (const auto &[id, point] : points)
    {
        Color color = Color(RED);
        if (isVisited(visited, id))
        {
            color = Color(RAYWHITE);
        }

        DrawCircle(point.getX(), point.getY(), 20, color);

        // draw vertex id
        string vertexId = to_string(id);
        DrawText(vertexId.c_str(), point.getX(), point.getY(), 48, BLACK);
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
    grid.addPoint(200, 100);
    grid.addPoint(300, 100);
    grid.addPoint(400, 100);
    grid.addPoint(500, 100);
    grid.addPoint(600, 100);
    grid.addPoint(700, 100);
    grid.addPoint(800, 100);
    grid.addPoint(900, 100);
    grid.addPoint(1000, 100);
    grid.addPoint(1100, 100);

    grid.addPoint(100, 200);
    grid.addPoint(200, 200);
    grid.addPoint(300, 200);
    grid.addPoint(400, 200);
    grid.addPoint(500, 200);
    grid.addPoint(600, 200);
    grid.addPoint(700, 200);
    grid.addPoint(800, 200);
    grid.addPoint(900, 200);
    grid.addPoint(1000, 200);
    grid.addPoint(1100, 200);

    grid.addPoint(100, 300);
    grid.addPoint(200, 300);
    grid.addPoint(300, 300);
    grid.addPoint(400, 300);
    grid.addPoint(500, 300);
    grid.addPoint(600, 300);
    grid.addPoint(700, 300);
    grid.addPoint(800, 300);
    grid.addPoint(900, 300);
    grid.addPoint(1000, 300);
    grid.addPoint(1100, 300);

    grid.addPoint(100, 400);
    grid.addPoint(200, 400);
    grid.addPoint(300, 400);
    grid.addPoint(400, 400);
    grid.addPoint(500, 400);
    grid.addPoint(600, 400);
    grid.addPoint(700, 400);
    grid.addPoint(800, 400);
    grid.addPoint(900, 400);
    grid.addPoint(1000, 400);
    grid.addPoint(1100, 400);

    grid.addPoint(100, 500);
    grid.addPoint(200, 500);
    grid.addPoint(300, 500);
    grid.addPoint(400, 500);
    grid.addPoint(500, 500);
    grid.addPoint(600, 500);
    grid.addPoint(700, 500);
    grid.addPoint(800, 500);
    grid.addPoint(900, 500);
    grid.addPoint(1000, 500);
    grid.addPoint(1100, 500);

    grid.addPoint(100, 600);
    grid.addPoint(200, 600);
    grid.addPoint(300, 600);
    grid.addPoint(400, 600);
    grid.addPoint(500, 600);
    grid.addPoint(600, 600);
    grid.addPoint(700, 600);
    grid.addPoint(800, 600);
    grid.addPoint(900, 600);
    grid.addPoint(1000, 600);
    grid.addPoint(1100, 600);

    grid.addPoint(100, 700);
    grid.addPoint(200, 700);
    grid.addPoint(300, 700);
    grid.addPoint(400, 700);
    grid.addPoint(500, 700);
    grid.addPoint(600, 700);
    grid.addPoint(700, 700);
    grid.addPoint(800, 700);
    grid.addPoint(900, 700);
    grid.addPoint(1000, 700);
    grid.addPoint(1100, 700);

    grid.connectPoints(0, 1);
    grid.connectPoints(1, 2);
    grid.connectPoints(3, 4);
    grid.connectPoints(4, 5);
    grid.connectPoints(6, 7);
    grid.connectPoints(7, 8);
    grid.connectPoints(9, 10);
    grid.connectPoints(11, 12);
    grid.connectPoints(12, 13);
    grid.connectPoints(14, 15);
    grid.connectPoints(15, 16);
    grid.connectPoints(17, 18);
    grid.connectPoints(18, 19);
    grid.connectPoints(20, 21);
    grid.connectPoints(22, 23);
    grid.connectPoints(23, 24);
    grid.connectPoints(25, 26);
    grid.connectPoints(26, 27);
    grid.connectPoints(28, 29);
    grid.connectPoints(30, 31);
    grid.connectPoints(31, 32);
    grid.connectPoints(33, 34);
    grid.connectPoints(34, 35);
    grid.connectPoints(36, 37);
    grid.connectPoints(37, 38);
    grid.connectPoints(39, 40);
    grid.connectPoints(41, 42);
    grid.connectPoints(42, 43);
    grid.connectPoints(44, 45);
    grid.connectPoints(45, 46);
    grid.connectPoints(47, 48);
    grid.connectPoints(48, 49);
    grid.connectPoints(50, 51);
    grid.connectPoints(52, 53);
    grid.connectPoints(53, 54);
    grid.connectPoints(55, 56);
    grid.connectPoints(56, 57);
    grid.connectPoints(58, 59);
    grid.connectPoints(60, 61);
    grid.connectPoints(61, 62);
    grid.connectPoints(63, 64);
    grid.connectPoints(64, 65);
    grid.connectPoints(66, 67);
    grid.connectPoints(67, 68);
    grid.connectPoints(69, 70);
    grid.connectPoints(71, 72);
    grid.connectPoints(74, 75);
    grid.connectPoints(75, 76);

    grid.connectPoints(0, 11);
    grid.connectPoints(2, 13);
    grid.connectPoints(4, 15);
    grid.connectPoints(6, 17);
    grid.connectPoints(8, 19);
    grid.connectPoints(10, 21);
    grid.connectPoints(12, 23);
    grid.connectPoints(14, 25);
    grid.connectPoints(16, 27);
    grid.connectPoints(18, 29);
    grid.connectPoints(20, 31);
    grid.connectPoints(24, 35);
    grid.connectPoints(26, 37);
    grid.connectPoints(28, 39);
    grid.connectPoints(30, 41);
    grid.connectPoints(32, 43);
    grid.connectPoints(34, 45);
    grid.connectPoints(36, 47);
    grid.connectPoints(38, 49);
    grid.connectPoints(40, 51);
    grid.connectPoints(42, 53);
    grid.connectPoints(44, 55);
    grid.connectPoints(46, 57);
    grid.connectPoints(48, 59);
    grid.connectPoints(50, 61);
    grid.connectPoints(52, 63);
    grid.connectPoints(54, 65);
    grid.connectPoints(56, 67);
    grid.connectPoints(58, 69);
    grid.connectPoints(60, 71);
    grid.connectPoints(62, 73);
    grid.connectPoints(64, 75);

    grid.connectPoints(9, 20);
    grid.connectPoints(19, 30);
    grid.connectPoints(29, 40);
    grid.connectPoints(39, 50);
    grid.connectPoints(49, 60);
    grid.connectPoints(59, 70);
    grid.connectPoints(69, 76);

    grid.connectPoints(35, 36);
    grid.connectPoints(2, 3);
    grid.connectPoints(16, 17);

    return grid;
}

void createFullGrid(Grid grid)
{
}
