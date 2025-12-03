#pragma once
#include <iostream>
#include <algorithm>
#include <cmath>
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
    unordered_set<int> getPointIds();
    int getSize();
    void addPoint(int x, int y);
    void connectPoints(int id1, int id2);

    bool reserveVertex(int vertexId, int agentId);
    void freeAllVertex();
    void freeVertex(int vertexId);
    bool isVertexBusy(int vertexId);
    bool isVertexBusyByOtherAgent(int vertexId, int agentId);
    Vertex &getVertex(int vertexId);
    bool vertexExists(int vertexId) const;
    double getDistance(int id1, int id2);

    void draw(unordered_set<int> visited);
};

unordered_map<int, Vertex> Grid::getPoints()
{
    return points;
}

unordered_multimap<int, int> Grid::getConnections()
{
    return connections;
}

unordered_set<int> Grid::getPointIds()
{
    unordered_set<int> keys;
    for (const auto &pair : points)
    {
        keys.insert(pair.first);
    }
    return keys;
}

int Grid::getSize()
{
    return static_cast<int>(points.size());
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

bool Grid::reserveVertex(int vertexId, int agentId)
{
    Vertex &vertex = getVertex(vertexId);
    if (!vertex.isBusy())
    {
        vertex.setBusyByAgentId(agentId);
        return true;
    }
    return false;
}

void Grid::freeAllVertex()
{
    for (const auto &element : points)
    {
        freeVertex(element.first);
    }
}

void Grid::freeVertex(int vertexId)
{
    Vertex &vertex = getVertex(vertexId);
    vertex.free();
}

bool Grid::isVertexBusy(int vertexId)
{
    return getVertex(vertexId).isBusy();
}

bool Grid::isVertexBusyByOtherAgent(int vertexId, int agentId)
{
    return getVertex(vertexId).getBusyByAgentId() != agentId && getVertex(vertexId).getBusyByAgentId() != -1;
}

Vertex &Grid::getVertex(int vertexId)
{
    return points.at(vertexId);
}

bool Grid::vertexExists(int vertexId) const
{
    return points.find(vertexId) != points.end();
}

double Grid::getDistance(int id1, int id2)
{
    if (id2 < 0)
        return 0;

    Vertex &v1 = getVertex(id1);
    Vertex &v2 = getVertex(id2);

    double dx = v1.getX() - v2.getX();
    double dy = v1.getY() - v2.getY();

    return sqrt(dx * dx + dy * dy);
}

void Grid::draw(unordered_set<int> visited)
{
    for (const auto &[id, point] : points)
    {
        Color color = Color(BLACK);
        DrawCircle(point.getX(), point.getY(), 20, color);

        // draw vertex id
        string vertexId = to_string(id);
        DrawText(vertexId.c_str(), point.getX(), point.getY(), 48, LIGHTGRAY);
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

    grid.connectPoints(35, 36);
    grid.connectPoints(2, 3);
    grid.connectPoints(16, 17);

    return grid;
}

Grid makeFullGrid(Grid grid)
{
    grid.addPoint(100, 100);
    grid.addPoint(200, 100);
    grid.addPoint(300, 100);
    grid.addPoint(400, 100);
    grid.addPoint(500, 100);
    grid.addPoint(600, 100);
    grid.addPoint(700, 100);
    grid.addPoint(800, 100);

    grid.addPoint(100, 200);
    grid.addPoint(200, 200);
    grid.addPoint(300, 200);
    grid.addPoint(400, 200);
    grid.addPoint(500, 200);
    grid.addPoint(600, 200);
    grid.addPoint(700, 200);
    grid.addPoint(800, 200);

    grid.addPoint(100, 300);
    grid.addPoint(200, 300);
    grid.addPoint(300, 300);
    grid.addPoint(400, 300);
    grid.addPoint(500, 300);
    grid.addPoint(600, 300);
    grid.addPoint(700, 300);
    grid.addPoint(800, 300);

    grid.addPoint(100, 400);
    grid.addPoint(200, 400);
    grid.addPoint(300, 400);
    grid.addPoint(400, 400);
    grid.addPoint(500, 400);
    grid.addPoint(600, 400);
    grid.addPoint(700, 400);
    grid.addPoint(800, 400);

    grid.addPoint(100, 500);
    grid.addPoint(200, 500);
    grid.addPoint(300, 500);
    grid.addPoint(400, 500);
    grid.addPoint(500, 500);
    grid.addPoint(600, 500);
    grid.addPoint(700, 500);
    grid.addPoint(800, 500);

    grid.addPoint(100, 600);
    grid.addPoint(200, 600);
    grid.addPoint(300, 600);
    grid.addPoint(400, 600);
    grid.addPoint(500, 600);
    grid.addPoint(600, 600);
    grid.addPoint(700, 600);
    grid.addPoint(800, 600);

    grid.addPoint(100, 700);
    grid.addPoint(200, 700);
    grid.addPoint(300, 700);
    grid.addPoint(400, 700);
    grid.addPoint(500, 700);
    grid.addPoint(600, 700);
    grid.addPoint(700, 700);
    grid.addPoint(800, 700);

    grid.addPoint(100, 800);
    grid.addPoint(200, 800);
    grid.addPoint(300, 800);
    grid.addPoint(400, 800);
    grid.addPoint(500, 800);
    grid.addPoint(600, 800);
    grid.addPoint(700, 800);
    grid.addPoint(800, 800);

    grid.connectPoints(0, 1);
    grid.connectPoints(1, 2);
    grid.connectPoints(2, 3);
    grid.connectPoints(3, 4);
    grid.connectPoints(4, 5);
    grid.connectPoints(5, 6);
    grid.connectPoints(6, 7);
    grid.connectPoints(8, 9);
    grid.connectPoints(9, 10);
    grid.connectPoints(10, 11);
    grid.connectPoints(11, 12);
    grid.connectPoints(12, 13);
    grid.connectPoints(13, 14);
    grid.connectPoints(14, 15);
    grid.connectPoints(16, 17);
    grid.connectPoints(17, 18);
    grid.connectPoints(18, 19);
    grid.connectPoints(19, 20);
    grid.connectPoints(20, 21);
    grid.connectPoints(21, 22);
    grid.connectPoints(22, 23);
    grid.connectPoints(24, 25);
    grid.connectPoints(25, 26);
    grid.connectPoints(26, 27);
    grid.connectPoints(27, 28);
    grid.connectPoints(28, 29);
    grid.connectPoints(29, 30);
    grid.connectPoints(30, 31);
    grid.connectPoints(32, 33);
    grid.connectPoints(33, 34);
    grid.connectPoints(34, 35);
    grid.connectPoints(35, 36);
    grid.connectPoints(36, 37);
    grid.connectPoints(37, 38);
    grid.connectPoints(38, 39);
    grid.connectPoints(40, 41);
    grid.connectPoints(41, 42);
    grid.connectPoints(42, 43);
    grid.connectPoints(43, 44);
    grid.connectPoints(44, 45);
    grid.connectPoints(45, 46);
    grid.connectPoints(46, 47);
    grid.connectPoints(48, 49);
    grid.connectPoints(49, 50);
    grid.connectPoints(50, 51);
    grid.connectPoints(51, 52);
    grid.connectPoints(52, 53);
    grid.connectPoints(53, 54);
    grid.connectPoints(54, 55);
    grid.connectPoints(56, 57);
    grid.connectPoints(57, 58);
    grid.connectPoints(58, 59);
    grid.connectPoints(59, 60);
    grid.connectPoints(60, 61);
    grid.connectPoints(61, 62);
    grid.connectPoints(62, 63);
    grid.connectPoints(0, 8);
    grid.connectPoints(8, 16);
    grid.connectPoints(16, 24);
    grid.connectPoints(24, 32);
    grid.connectPoints(32, 40);
    grid.connectPoints(40, 48);
    grid.connectPoints(48, 56);
    grid.connectPoints(1, 9);
    grid.connectPoints(9, 17);
    grid.connectPoints(17, 25);
    grid.connectPoints(25, 33);
    grid.connectPoints(33, 41);
    grid.connectPoints(41, 49);
    grid.connectPoints(49, 57);
    grid.connectPoints(2, 10);
    grid.connectPoints(10, 18);
    grid.connectPoints(18, 26);
    grid.connectPoints(26, 34);
    grid.connectPoints(34, 42);
    grid.connectPoints(42, 50);
    grid.connectPoints(50, 58);
    grid.connectPoints(3, 11);
    grid.connectPoints(11, 19);
    grid.connectPoints(19, 27);
    grid.connectPoints(27, 35);
    grid.connectPoints(35, 43);
    grid.connectPoints(43, 51);
    grid.connectPoints(51, 59);
    grid.connectPoints(4, 12);
    grid.connectPoints(12, 20);
    grid.connectPoints(20, 28);
    grid.connectPoints(28, 36);
    grid.connectPoints(36, 44);
    grid.connectPoints(44, 52);
    grid.connectPoints(52, 60);
    grid.connectPoints(5, 13);
    grid.connectPoints(13, 21);
    grid.connectPoints(21, 29);
    grid.connectPoints(29, 37);
    grid.connectPoints(37, 45);
    grid.connectPoints(45, 53);
    grid.connectPoints(53, 61);
    grid.connectPoints(6, 14);
    grid.connectPoints(14, 22);
    grid.connectPoints(22, 30);
    grid.connectPoints(30, 38);
    grid.connectPoints(38, 46);
    grid.connectPoints(46, 54);
    grid.connectPoints(54, 62);
    grid.connectPoints(7, 15);
    grid.connectPoints(15, 23);
    grid.connectPoints(23, 31);
    grid.connectPoints(31, 39);
    grid.connectPoints(39, 47);
    grid.connectPoints(47, 55);
    grid.connectPoints(55, 63);

    return grid;
}

Grid makeTestGrid(Grid grid)
{
    grid.addPoint(100, 100);
    // grid.addPoint(200, 100);
    // grid.addPoint(300, 100);
    grid.addPoint(400, 100);
    grid.addPoint(500, 100);
    grid.addPoint(600, 100);
    grid.addPoint(700, 100);
    grid.addPoint(800, 100);

    grid.addPoint(500, 500);

    grid.connectPoints(0, 1);
    grid.connectPoints(1, 2);
    grid.connectPoints(3, 2);
    grid.connectPoints(3, 4);
    grid.connectPoints(5, 4);

    return grid;
}

Grid makeGraph(Grid grid)
{
    grid.addPoint(210, 153);
    grid.addPoint(526, 78);
    grid.addPoint(124, 122);
    grid.addPoint(255, 243);
    grid.addPoint(166, 420);
    grid.addPoint(603, 736);
    grid.addPoint(529, 650);
    grid.addPoint(664, 649);
    grid.addPoint(528, 552);
    grid.addPoint(589, 676);
    grid.addPoint(746, 702);
    grid.addPoint(549, 517);
    grid.addPoint(673, 587);
    grid.addPoint(661, 352);
    grid.addPoint(741, 431);
    grid.addPoint(426, 142);
    grid.addPoint(415, 543);
    grid.addPoint(355, 697);
    grid.addPoint(498, 719);
    grid.addPoint(444, 750);
    grid.addPoint(411, 613);
    grid.addPoint(328, 611);
    grid.addPoint(411, 716);
    grid.addPoint(282, 749);
    grid.addPoint(649, 137);
    grid.addPoint(403, 82);
    grid.addPoint(722, 538);
    grid.addPoint(556, 398);
    grid.addPoint(790, 541);
    grid.addPoint(659, 475);
    grid.addPoint(187, 653);
    grid.addPoint(638, 62);
    grid.addPoint(119, 501);
    grid.addPoint(122, 570);
    grid.addPoint(238, 547);
    grid.addPoint(568, 118);
    grid.addPoint(705, 102);
    grid.addPoint(107, 721);
    grid.addPoint(261, 679);
    grid.addPoint(572, 353);
    grid.addPoint(789, 480);
    grid.addPoint(792, 314);
    grid.addPoint(529, 248);
    grid.addPoint(302, 501);
    grid.addPoint(50, 570);
    grid.addPoint(357, 180);
    grid.addPoint(728, 333);
    grid.addPoint(307, 370);
    grid.addPoint(126, 366);
    grid.addPoint(288, 139);
    grid.addPoint(365, 306);
    grid.addPoint(544, 211);
    grid.addPoint(749, 288);
    grid.addPoint(51, 380);
    grid.addPoint(283, 422);
    grid.addPoint(197, 322);
    grid.addPoint(367, 53);
    grid.addPoint(673, 216);
    grid.addPoint(776, 146);
    grid.addPoint(472, 50);
    grid.addPoint(255, 302);
    grid.addPoint(67, 191);
    grid.addPoint(404, 291);
    grid.addPoint(248, 77);

    grid.connectPoints(0, 1);
    grid.connectPoints(0, 2);
    grid.connectPoints(0, 3);
    grid.connectPoints(1, 58);
    grid.connectPoints(1, 59);
    grid.connectPoints(2, 61);
    grid.connectPoints(2, 63);
    grid.connectPoints(3, 60);
    grid.connectPoints(3, 62);
    grid.connectPoints(4, 32);
    grid.connectPoints(4, 47);
    grid.connectPoints(4, 48);
    grid.connectPoints(5, 9);
    grid.connectPoints(5, 10);
    grid.connectPoints(5, 11);
    grid.connectPoints(6, 9);
    grid.connectPoints(6, 11);
    grid.connectPoints(6, 17);
    grid.connectPoints(7, 10);
    grid.connectPoints(7, 12);
    grid.connectPoints(7, 18);
    grid.connectPoints(8, 11);
    grid.connectPoints(8, 12);
    grid.connectPoints(8, 16);
    grid.connectPoints(9, 29);
    grid.connectPoints(10, 28);
    grid.connectPoints(11, 27);
    grid.connectPoints(12, 26);
    grid.connectPoints(13, 26);
    grid.connectPoints(13, 27);
    grid.connectPoints(13, 31);
    grid.connectPoints(14, 28);
    grid.connectPoints(14, 29);
    grid.connectPoints(14, 46);
    grid.connectPoints(15, 24);
    grid.connectPoints(15, 25);
    grid.connectPoints(15, 45);
    grid.connectPoints(16, 20);
    grid.connectPoints(16, 21);
    grid.connectPoints(17, 21);
    grid.connectPoints(17, 23);
    grid.connectPoints(18, 20);
    grid.connectPoints(18, 22);
    grid.connectPoints(19, 22);
    grid.connectPoints(19, 23);
    grid.connectPoints(20, 34);
    grid.connectPoints(21, 33);
    grid.connectPoints(22, 38);
    grid.connectPoints(23, 37);
    grid.connectPoints(24, 36);
    grid.connectPoints(24, 57);
    grid.connectPoints(25, 35);
    grid.connectPoints(25, 56);
    grid.connectPoints(26, 40);
    grid.connectPoints(27, 39);
    grid.connectPoints(28, 40);
    grid.connectPoints(29, 39);
    grid.connectPoints(30, 37);
    grid.connectPoints(30, 38);
    grid.connectPoints(30, 55);
    grid.connectPoints(31, 35);
    grid.connectPoints(31, 36);
    grid.connectPoints(32, 33);
    grid.connectPoints(32, 34);
    grid.connectPoints(33, 44);
    grid.connectPoints(34, 43);
    grid.connectPoints(35, 42);
    grid.connectPoints(36, 41);
    grid.connectPoints(37, 44);
    grid.connectPoints(38, 43);
    grid.connectPoints(39, 41);
    grid.connectPoints(40, 41);
    grid.connectPoints(41, 52);
    grid.connectPoints(42, 51);
    grid.connectPoints(43, 54);
    grid.connectPoints(44, 53);
    grid.connectPoints(45, 49);
    grid.connectPoints(45, 50);
    grid.connectPoints(46, 51);
    grid.connectPoints(46, 52);
    grid.connectPoints(47, 50);
    grid.connectPoints(47, 54);
    grid.connectPoints(48, 49);
    grid.connectPoints(48, 53);
    grid.connectPoints(49, 63);
    grid.connectPoints(50, 62);
    grid.connectPoints(51, 59);
    grid.connectPoints(52, 58);
    grid.connectPoints(53, 61);
    grid.connectPoints(54, 60);
    grid.connectPoints(55, 60);
    grid.connectPoints(55, 61);
    grid.connectPoints(56, 59);
    grid.connectPoints(56, 63);
    grid.connectPoints(57, 58);
    grid.connectPoints(57, 62);

    return grid;
}

Grid makeGraphTree(Grid grid)
{
    grid.addPoint(256, 290); // Punkt 0
    grid.addPoint(596, 527); // Punkt 1
    grid.addPoint(596, 598); // Punkt 2
    grid.addPoint(294, 220); // Punkt 3
    grid.addPoint(611, 412); // Punkt 4
    grid.addPoint(52, 213);  // Punkt 5
    grid.addPoint(746, 70);  // Punkt 6
    grid.addPoint(84, 750);  // Punkt 7
    grid.addPoint(465, 369); // Punkt 8
    grid.addPoint(184, 475); // Punkt 9
    grid.addPoint(275, 156); // Punkt 10
    grid.addPoint(620, 733); // Punkt 11
    grid.addPoint(79, 64);   // Punkt 12
    grid.addPoint(757, 514); // Punkt 13
    grid.addPoint(216, 607); // Punkt 14
    grid.addPoint(770, 189); // Punkt 15
    grid.addPoint(386, 568); // Punkt 16
    grid.addPoint(293, 542); // Punkt 17
    grid.addPoint(555, 241); // Punkt 18
    grid.addPoint(691, 655); // Punkt 19
    grid.addPoint(50, 395);  // Punkt 20
    grid.addPoint(761, 363); // Punkt 21
    grid.addPoint(317, 739); // Punkt 22
    grid.addPoint(537, 89);  // Punkt 23
    grid.addPoint(64, 526);  // Punkt 24
    grid.addPoint(627, 317); // Punkt 25
    grid.addPoint(415, 737); // Punkt 26
    grid.addPoint(464, 130); // Punkt 27
    grid.addPoint(761, 706); // Punkt 28
    grid.addPoint(258, 69);  // Punkt 29
    grid.addPoint(769, 296); // Punkt 30
    grid.addPoint(160, 555); // Punkt 31
    grid.addPoint(292, 462); // Punkt 32
    grid.addPoint(428, 537); // Punkt 33
    grid.addPoint(608, 672); // Punkt 34
    grid.addPoint(477, 276); // Punkt 35
    grid.addPoint(773, 448); // Punkt 36
    grid.addPoint(98, 296);  // Punkt 37
    grid.addPoint(668, 50);  // Punkt 38
    grid.addPoint(215, 745); // Punkt 39
    grid.addPoint(589, 342); // Punkt 40
    grid.addPoint(94, 464);  // Punkt 41
    grid.addPoint(371, 75);  // Punkt 42
    grid.addPoint(464, 746); // Punkt 43
    grid.addPoint(183, 79);  // Punkt 44
    grid.addPoint(756, 595); // Punkt 45
    grid.addPoint(75, 600);  // Punkt 46
    grid.addPoint(689, 349); // Punkt 47
    grid.addPoint(154, 407); // Punkt 48
    grid.addPoint(519, 394); // Punkt 49
    grid.addPoint(497, 713); // Punkt 50
    grid.addPoint(347, 118); // Punkt 51
    grid.addPoint(654, 537); // Punkt 52
    grid.addPoint(116, 158); // Punkt 53
    grid.addPoint(656, 242); // Punkt 54
    grid.addPoint(109, 673); // Punkt 55
    grid.addPoint(144, 239); // Punkt 56
    grid.addPoint(696, 461); // Punkt 57
    grid.addPoint(262, 660); // Punkt 58
    grid.addPoint(644, 153); // Punkt 59
    grid.addPoint(511, 641); // Punkt 60
    grid.addPoint(401, 197); // Punkt 61
    grid.addPoint(513, 494); // Punkt 62
    grid.addPoint(284, 393); // Punkt 63

    grid.connectPoints(0, 63);  // Połączenie 0
    grid.connectPoints(1, 62);  // Połączenie 1
    grid.connectPoints(2, 60);  // Połączenie 2
    grid.connectPoints(3, 61);  // Połączenie 3
    grid.connectPoints(4, 57);  // Połączenie 4
    grid.connectPoints(5, 56);  // Połączenie 5
    grid.connectPoints(6, 59);  // Połączenie 6
    grid.connectPoints(7, 58);  // Połączenie 7
    grid.connectPoints(8, 49);  // Połączenie 8
    grid.connectPoints(9, 48);  // Połączenie 9
    grid.connectPoints(10, 51); // Połączenie 10
    grid.connectPoints(11, 50); // Połączenie 11
    grid.connectPoints(12, 53); // Połączenie 12
    grid.connectPoints(13, 52); // Połączenie 13
    grid.connectPoints(14, 55); // Połączenie 14
    grid.connectPoints(15, 54); // Połączenie 15
    grid.connectPoints(16, 33); // Połączenie 16
    grid.connectPoints(17, 32); // Połączenie 17
    grid.connectPoints(18, 35); // Połączenie 18
    grid.connectPoints(19, 34); // Połączenie 19
    grid.connectPoints(20, 37); // Połączenie 20
    grid.connectPoints(21, 36); // Połączenie 21
    grid.connectPoints(22, 39); // Połączenie 22
    grid.connectPoints(23, 38); // Połączenie 23
    grid.connectPoints(24, 41); // Połączenie 24
    grid.connectPoints(25, 40); // Połączenie 25
    grid.connectPoints(26, 43); // Połączenie 26
    grid.connectPoints(27, 42); // Połączenie 27
    grid.connectPoints(28, 45); // Połączenie 28
    grid.connectPoints(29, 44); // Połączenie 29
    grid.connectPoints(30, 47); // Połączenie 30
    grid.connectPoints(31, 46); // Połączenie 31
    grid.connectPoints(32, 63); // Połączenie 32
    grid.connectPoints(33, 62); // Połączenie 33
    grid.connectPoints(34, 60); // Połączenie 34
    grid.connectPoints(35, 61); // Połączenie 35
    grid.connectPoints(36, 57); // Połączenie 36
    grid.connectPoints(37, 56); // Połączenie 37
    grid.connectPoints(38, 59); // Połączenie 38
    grid.connectPoints(39, 58); // Połączenie 39
    grid.connectPoints(40, 49); // Połączenie 40
    grid.connectPoints(41, 48); // Połączenie 41
    grid.connectPoints(42, 51); // Połączenie 42
    grid.connectPoints(43, 50); // Połączenie 43
    grid.connectPoints(44, 53); // Połączenie 44
    grid.connectPoints(45, 52); // Połączenie 45
    grid.connectPoints(46, 55); // Połączenie 46
    grid.connectPoints(47, 54); // Połączenie 47
    grid.connectPoints(48, 63); // Połączenie 48
    grid.connectPoints(49, 62); // Połączenie 49
    grid.connectPoints(50, 60); // Połączenie 50
    grid.connectPoints(51, 61); // Połączenie 51
    grid.connectPoints(52, 57); // Połączenie 52
    grid.connectPoints(53, 56); // Połączenie 53
    grid.connectPoints(54, 59); // Połączenie 54
    grid.connectPoints(55, 58); // Połączenie 55
    grid.connectPoints(56, 63); // Połączenie 56
    grid.connectPoints(57, 62); // Połączenie 57
    grid.connectPoints(58, 60); // Połączenie 58
    grid.connectPoints(59, 61); // Połączenie 59
    grid.connectPoints(60, 62); // Połączenie 60
    grid.connectPoints(61, 63); // Połączenie 61
    grid.connectPoints(62, 63); // Połączenie 62

    return grid;
}

Grid makeGraphGroups(Grid grid)
{
    grid.addPoint(618, 384); // Punkt 0
    grid.addPoint(609, 433); // Punkt 1
    grid.addPoint(603, 333); // Punkt 2
    grid.addPoint(698, 349); // Punkt 3
    grid.addPoint(220, 402); // Punkt 4
    grid.addPoint(56, 300);  // Punkt 5
    grid.addPoint(64, 725);  // Punkt 6
    grid.addPoint(165, 750); // Punkt 7
    grid.addPoint(125, 632); // Punkt 8
    grid.addPoint(438, 498); // Punkt 9
    grid.addPoint(653, 510); // Punkt 10
    grid.addPoint(411, 599); // Punkt 11
    grid.addPoint(723, 711); // Punkt 12
    grid.addPoint(595, 570); // Punkt 13
    grid.addPoint(571, 642); // Punkt 14
    grid.addPoint(499, 716); // Punkt 15
    grid.addPoint(708, 593); // Punkt 16
    grid.addPoint(452, 657); // Punkt 17
    grid.addPoint(479, 564); // Punkt 18
    grid.addPoint(582, 723); // Punkt 19
    grid.addPoint(364, 525); // Punkt 20
    grid.addPoint(308, 578); // Punkt 21
    grid.addPoint(315, 733); // Punkt 22
    grid.addPoint(230, 627); // Punkt 23
    grid.addPoint(160, 691); // Punkt 24
    grid.addPoint(265, 563); // Punkt 25
    grid.addPoint(275, 649); // Punkt 26
    grid.addPoint(50, 562);  // Punkt 27
    grid.addPoint(121, 480); // Punkt 28
    grid.addPoint(50, 457);  // Punkt 29
    grid.addPoint(160, 445); // Punkt 30
    grid.addPoint(53, 399);  // Punkt 31
    grid.addPoint(90, 373);  // Punkt 32
    grid.addPoint(132, 397); // Punkt 33
    grid.addPoint(270, 367); // Punkt 34
    grid.addPoint(436, 199); // Punkt 35
    grid.addPoint(173, 168); // Punkt 36
    grid.addPoint(163, 65);  // Punkt 37
    grid.addPoint(444, 75);  // Punkt 38
    grid.addPoint(488, 174); // Punkt 39
    grid.addPoint(89, 78);   // Punkt 40
    grid.addPoint(107, 238); // Punkt 41
    grid.addPoint(268, 50);  // Punkt 42
    grid.addPoint(236, 130); // Punkt 43
    grid.addPoint(66, 169);  // Punkt 44
    grid.addPoint(297, 195); // Punkt 45
    grid.addPoint(213, 245); // Punkt 46
    grid.addPoint(326, 120); // Punkt 47
    grid.addPoint(552, 59);  // Punkt 48
    grid.addPoint(574, 114); // Punkt 49
    grid.addPoint(622, 87);  // Punkt 50
    grid.addPoint(582, 156); // Punkt 51
    grid.addPoint(354, 225); // Punkt 52
    grid.addPoint(298, 280); // Punkt 53
    grid.addPoint(702, 71);  // Punkt 54
    grid.addPoint(651, 157); // Punkt 55
    grid.addPoint(394, 331); // Punkt 56
    grid.addPoint(710, 193); // Punkt 57
    grid.addPoint(713, 284); // Punkt 58
    grid.addPoint(478, 371); // Punkt 59
    grid.addPoint(538, 361); // Punkt 60
    grid.addPoint(709, 419); // Punkt 61
    grid.addPoint(684, 449); // Punkt 62
    grid.addPoint(651, 295); // Punkt 63

    grid.connectPoints(0, 1);   // Połączenie 0
    grid.connectPoints(0, 2);   // Połączenie 1
    grid.connectPoints(0, 3);   // Połączenie 2
    grid.connectPoints(1, 59);  // Połączenie 3
    grid.connectPoints(1, 62);  // Połączenie 4
    grid.connectPoints(2, 60);  // Połączenie 5
    grid.connectPoints(2, 63);  // Połączenie 6
    grid.connectPoints(3, 58);  // Połączenie 7
    grid.connectPoints(3, 61);  // Połączenie 8
    grid.connectPoints(4, 30);  // Połączenie 9
    grid.connectPoints(4, 33);  // Połączenie 10
    grid.connectPoints(4, 34);  // Połączenie 11
    grid.connectPoints(5, 31);  // Połączenie 12
    grid.connectPoints(5, 32);  // Połączenie 13
    grid.connectPoints(5, 34);  // Połączenie 14
    grid.connectPoints(6, 7);   // Połączenie 15
    grid.connectPoints(6, 8);   // Połączenie 16
    grid.connectPoints(6, 27);  // Połączenie 17
    grid.connectPoints(7, 23);  // Połączenie 18
    grid.connectPoints(7, 26);  // Połączenie 19
    grid.connectPoints(8, 24);  // Połączenie 20
    grid.connectPoints(8, 25);  // Połączenie 21
    grid.connectPoints(9, 10);  // Połączenie 22
    grid.connectPoints(9, 11);  // Połączenie 23
    grid.connectPoints(9, 20);  // Połączenie 24
    grid.connectPoints(10, 16); // Połączenie 25
    grid.connectPoints(10, 19); // Połączenie 26
    grid.connectPoints(11, 17); // Połączenie 27
    grid.connectPoints(11, 18); // Połączenie 28
    grid.connectPoints(12, 14); // Połączenie 29
    grid.connectPoints(12, 16); // Połączenie 30
    grid.connectPoints(12, 17); // Połączenie 31
    grid.connectPoints(13, 15); // Połączenie 32
    grid.connectPoints(13, 16); // Połączenie 33
    grid.connectPoints(13, 18); // Połączenie 34
    grid.connectPoints(14, 18); // Połączenie 35
    grid.connectPoints(14, 19); // Połączenie 36
    grid.connectPoints(15, 17); // Połączenie 37
    grid.connectPoints(15, 19); // Połączenie 38
    grid.connectPoints(20, 21); // Połączenie 39
    grid.connectPoints(20, 22); // Połączenie 40
    grid.connectPoints(21, 23); // Połączenie 41
    grid.connectPoints(21, 25); // Połączenie 42
    grid.connectPoints(22, 24); // Połączenie 43
    grid.connectPoints(22, 26); // Połączenie 44
    grid.connectPoints(23, 24); // Połączenie 45
    grid.connectPoints(25, 26); // Połączenie 46
    grid.connectPoints(27, 28); // Połączenie 47
    grid.connectPoints(27, 29); // Połączenie 48
    grid.connectPoints(28, 30); // Połączenie 49
    grid.connectPoints(28, 32); // Połączenie 50
    grid.connectPoints(29, 31); // Połączenie 51
    grid.connectPoints(29, 33); // Połączenie 52
    grid.connectPoints(30, 31); // Połączenie 53
    grid.connectPoints(32, 33); // Połączenie 54
    grid.connectPoints(34, 35); // Połączenie 55
    grid.connectPoints(35, 38); // Połączenie 56
    grid.connectPoints(35, 39); // Połączenie 57
    grid.connectPoints(36, 37); // Połączenie 58
    grid.connectPoints(36, 40); // Połączenie 59
    grid.connectPoints(36, 43); // Połączenie 60
    grid.connectPoints(37, 41); // Połączenie 61
    grid.connectPoints(37, 42); // Połączenie 62
    grid.connectPoints(38, 48); // Połączenie 63
    grid.connectPoints(38, 50); // Połączenie 64
    grid.connectPoints(39, 49); // Połączenie 65
    grid.connectPoints(39, 51); // Połączenie 66
    grid.connectPoints(40, 44); // Połączenie 67
    grid.connectPoints(40, 45); // Połączenie 68
    grid.connectPoints(41, 44); // Połączenie 69
    grid.connectPoints(41, 46); // Połączenie 70
    grid.connectPoints(42, 45); // Połączenie 71
    grid.connectPoints(42, 47); // Połączenie 72
    grid.connectPoints(43, 46); // Połączenie 73
    grid.connectPoints(43, 47); // Połączenie 74
    grid.connectPoints(44, 52); // Połączenie 75
    grid.connectPoints(45, 53); // Połączenie 76
    grid.connectPoints(46, 53); // Połączenie 77
    grid.connectPoints(47, 52); // Połączenie 78
    grid.connectPoints(48, 49); // Połączenie 79
    grid.connectPoints(48, 54); // Połączenie 80
    grid.connectPoints(49, 55); // Połączenie 81
    grid.connectPoints(50, 51); // Połączenie 82
    grid.connectPoints(50, 55); // Połączenie 83
    grid.connectPoints(51, 54); // Połączenie 84
    grid.connectPoints(52, 56); // Połączenie 85
    grid.connectPoints(53, 56); // Połączenie 86
    grid.connectPoints(54, 57); // Połączenie 87
    grid.connectPoints(55, 57); // Połączenie 88
    grid.connectPoints(56, 59); // Połączenie 89
    grid.connectPoints(57, 58); // Połączenie 90
    grid.connectPoints(58, 63); // Połączenie 91
    grid.connectPoints(59, 60); // Połączenie 92
    grid.connectPoints(60, 61); // Połączenie 93
    grid.connectPoints(61, 62); // Połączenie 94
    grid.connectPoints(62, 63); // Połączenie 95

    return grid;
}

Grid makeGraphRandom(Grid grid)
{
    grid.addPoint(349, 219); // Punkt 0
    grid.addPoint(357, 72);  // Punkt 1
    grid.addPoint(50, 82);   // Punkt 2
    grid.addPoint(492, 745); // Punkt 3
    grid.addPoint(511, 234); // Punkt 4
    grid.addPoint(358, 539); // Punkt 5
    grid.addPoint(392, 669); // Punkt 6
    grid.addPoint(331, 747); // Punkt 7
    grid.addPoint(197, 225); // Punkt 8
    grid.addPoint(206, 402); // Punkt 9
    grid.addPoint(243, 509); // Punkt 10
    grid.addPoint(101, 694); // Punkt 11
    grid.addPoint(726, 50);  // Punkt 12
    grid.addPoint(339, 390); // Punkt 13
    grid.addPoint(201, 84);  // Punkt 14
    grid.addPoint(530, 385); // Punkt 15
    grid.addPoint(60, 399);  // Punkt 16
    grid.addPoint(556, 466); // Punkt 17
    grid.addPoint(109, 750); // Punkt 18
    grid.addPoint(500, 67);  // Punkt 19
    grid.addPoint(713, 228); // Punkt 20
    grid.addPoint(657, 639); // Punkt 21
    grid.addPoint(484, 540); // Punkt 22
    grid.addPoint(627, 148); // Punkt 23
    grid.addPoint(654, 420); // Punkt 24
    grid.addPoint(712, 705); // Punkt 25
    grid.addPoint(638, 543); // Punkt 26
    grid.addPoint(474, 687); // Punkt 27
    grid.addPoint(276, 645); // Punkt 28
    grid.addPoint(260, 713); // Punkt 29
    grid.addPoint(54, 231);  // Punkt 30
    grid.addPoint(86, 556);  // Punkt 31

    grid.connectPoints(0, 1);   // Połączenie 0
    grid.connectPoints(0, 2);   // Połączenie 1
    grid.connectPoints(0, 3);   // Połączenie 2
    grid.connectPoints(0, 4);   // Połączenie 3
    grid.connectPoints(0, 5);   // Połączenie 4
    grid.connectPoints(0, 6);   // Połączenie 5
    grid.connectPoints(0, 7);   // Połączenie 6
    grid.connectPoints(0, 8);   // Połączenie 7
    grid.connectPoints(1, 21);  // Połączenie 8
    grid.connectPoints(1, 22);  // Połączenie 9
    grid.connectPoints(1, 23);  // Połączenie 10
    grid.connectPoints(1, 24);  // Połączenie 11
    grid.connectPoints(1, 29);  // Połączenie 12
    grid.connectPoints(1, 30);  // Połączenie 13
    grid.connectPoints(1, 31);  // Połączenie 14
    grid.connectPoints(2, 21);  // Połączenie 15
    grid.connectPoints(2, 22);  // Połączenie 16
    grid.connectPoints(2, 23);  // Połączenie 17
    grid.connectPoints(2, 24);  // Połączenie 18
    grid.connectPoints(2, 29);  // Połączenie 19
    grid.connectPoints(2, 30);  // Połączenie 20
    grid.connectPoints(2, 31);  // Połączenie 21
    grid.connectPoints(3, 21);  // Połączenie 22
    grid.connectPoints(3, 22);  // Połączenie 23
    grid.connectPoints(3, 23);  // Połączenie 24
    grid.connectPoints(3, 24);  // Połączenie 25
    grid.connectPoints(3, 29);  // Połączenie 26
    grid.connectPoints(3, 30);  // Połączenie 27
    grid.connectPoints(3, 31);  // Połączenie 28
    grid.connectPoints(4, 21);  // Połączenie 29
    grid.connectPoints(4, 22);  // Połączenie 30
    grid.connectPoints(4, 23);  // Połączenie 31
    grid.connectPoints(4, 24);  // Połączenie 32
    grid.connectPoints(4, 29);  // Połączenie 33
    grid.connectPoints(4, 30);  // Połączenie 34
    grid.connectPoints(4, 31);  // Połączenie 35
    grid.connectPoints(5, 25);  // Połączenie 36
    grid.connectPoints(5, 26);  // Połączenie 37
    grid.connectPoints(5, 27);  // Połączenie 38
    grid.connectPoints(5, 28);  // Połączenie 39
    grid.connectPoints(5, 29);  // Połączenie 40
    grid.connectPoints(5, 30);  // Połączenie 41
    grid.connectPoints(5, 31);  // Połączenie 42
    grid.connectPoints(6, 25);  // Połączenie 43
    grid.connectPoints(6, 26);  // Połączenie 44
    grid.connectPoints(6, 27);  // Połączenie 45
    grid.connectPoints(6, 28);  // Połączenie 46
    grid.connectPoints(6, 29);  // Połączenie 47
    grid.connectPoints(6, 30);  // Połączenie 48
    grid.connectPoints(6, 31);  // Połączenie 49
    grid.connectPoints(7, 25);  // Połączenie 50
    grid.connectPoints(7, 26);  // Połączenie 51
    grid.connectPoints(7, 27);  // Połączenie 52
    grid.connectPoints(7, 28);  // Połączenie 53
    grid.connectPoints(7, 29);  // Połączenie 54
    grid.connectPoints(7, 30);  // Połączenie 55
    grid.connectPoints(7, 31);  // Połączenie 56
    grid.connectPoints(8, 25);  // Połączenie 57
    grid.connectPoints(8, 26);  // Połączenie 58
    grid.connectPoints(8, 27);  // Połączenie 59
    grid.connectPoints(8, 28);  // Połączenie 60
    grid.connectPoints(8, 29);  // Połączenie 61
    grid.connectPoints(8, 30);  // Połączenie 62
    grid.connectPoints(8, 31);  // Połączenie 63
    grid.connectPoints(9, 13);  // Połączenie 64
    grid.connectPoints(9, 14);  // Połączenie 65
    grid.connectPoints(9, 15);  // Połączenie 66
    grid.connectPoints(9, 16);  // Połączenie 67
    grid.connectPoints(9, 17);  // Połączenie 68
    grid.connectPoints(9, 18);  // Połączenie 69
    grid.connectPoints(9, 19);  // Połączenie 70
    grid.connectPoints(9, 20);  // Połączenie 71
    grid.connectPoints(10, 13); // Połączenie 72
    grid.connectPoints(10, 14); // Połączenie 73
    grid.connectPoints(10, 15); // Połączenie 74
    grid.connectPoints(10, 16); // Połączenie 75
    grid.connectPoints(10, 17); // Połączenie 76
    grid.connectPoints(10, 18); // Połączenie 77
    grid.connectPoints(10, 19); // Połączenie 78
    grid.connectPoints(10, 20); // Połączenie 79
    grid.connectPoints(11, 13); // Połączenie 80
    grid.connectPoints(11, 14); // Połączenie 81
    grid.connectPoints(11, 15); // Połączenie 82
    grid.connectPoints(11, 16); // Połączenie 83
    grid.connectPoints(11, 17); // Połączenie 84
    grid.connectPoints(11, 18); // Połączenie 85
    grid.connectPoints(11, 19); // Połączenie 86
    grid.connectPoints(11, 20); // Połączenie 87
    grid.connectPoints(12, 13); // Połączenie 88
    grid.connectPoints(12, 14); // Połączenie 89
    grid.connectPoints(12, 15); // Połączenie 90
    grid.connectPoints(12, 16); // Połączenie 91
    grid.connectPoints(12, 17); // Połączenie 92
    grid.connectPoints(12, 18); // Połączenie 93
    grid.connectPoints(12, 19); // Połączenie 94
    grid.connectPoints(12, 20); // Połączenie 95
    grid.connectPoints(13, 21); // Połączenie 96
    grid.connectPoints(13, 22); // Połączenie 97
    grid.connectPoints(13, 23); // Połączenie 98
    grid.connectPoints(13, 24); // Połączenie 99
    grid.connectPoints(14, 21); // Połączenie 100
    grid.connectPoints(14, 22); // Połączenie 101
    grid.connectPoints(14, 23); // Połączenie 102
    grid.connectPoints(14, 24); // Połączenie 103
    grid.connectPoints(15, 21); // Połączenie 104
    grid.connectPoints(15, 22); // Połączenie 105
    grid.connectPoints(15, 23); // Połączenie 106
    grid.connectPoints(15, 24); // Połączenie 107
    grid.connectPoints(16, 21); // Połączenie 108
    grid.connectPoints(16, 22); // Połączenie 109
    grid.connectPoints(16, 23); // Połączenie 110
    grid.connectPoints(16, 24); // Połączenie 111
    grid.connectPoints(17, 25); // Połączenie 112
    grid.connectPoints(17, 26); // Połączenie 113
    grid.connectPoints(17, 27); // Połączenie 114
    grid.connectPoints(17, 28); // Połączenie 115
    grid.connectPoints(18, 25); // Połączenie 116
    grid.connectPoints(18, 26); // Połączenie 117
    grid.connectPoints(18, 27); // Połączenie 118
    grid.connectPoints(18, 28); // Połączenie 119
    grid.connectPoints(19, 25); // Połączenie 120
    grid.connectPoints(19, 26); // Połączenie 121
    grid.connectPoints(19, 27); // Połączenie 122
    grid.connectPoints(19, 28); // Połączenie 123
    grid.connectPoints(20, 25); // Połączenie 124
    grid.connectPoints(20, 26); // Połączenie 125
    grid.connectPoints(20, 27); // Połączenie 126
    grid.connectPoints(20, 28); // Połączenie 127

    return grid;
}
// algoorytm gnp

// erdos-renyi

// n liczba wierzch
// p prawdopodobnstwo kraw
// r losowa liczb

// r < p

// graph viiizzzzzzz

// houssse if graphgs
