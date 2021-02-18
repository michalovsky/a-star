#pragma once

#include <iostream>
#include <list>

#include "MapRenderer.h"
#include "Node.h"

class PathFinding
{
public:
    PathFinding(sf::RenderWindow&);
    ~PathFinding();

    void createMap();
    void drawMap(sf::RenderWindow& window);
    void updateMapByUser(sf::RenderWindow& window);
    void solveAStar();
    Node* nodes;
    Node* nodeStart = nullptr;
    Node* nodeEnd = nullptr;
    std::vector<sf::VertexArray> connections;
    int mapWidth = 20;
    int mapHeight = 20;
    std::unique_ptr<graphics::MapRenderer> mapRenderer;
};
