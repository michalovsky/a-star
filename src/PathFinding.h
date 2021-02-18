#pragma once

#include "MapBuilder.h"
#include "MapRenderer.h"
#include "Node.h"

class PathFinding
{
public:
    PathFinding(sf::RenderWindow&);

    void createMap();
    void drawMap(sf::RenderWindow& window);
    void updateMapByUser(sf::RenderWindow& window);
    void solveAStar();

private:
    Node* nodeStart = nullptr;
    Node* nodeEnd = nullptr;
    std::shared_ptr<graphics::MapRenderer> mapRenderer;
    std::unique_ptr<graphics::MapBuilder> mapBuilder;
    constexpr static int mapWidth = 20;
    constexpr static int mapHeight = 20;
    std::vector<Node> nodes;
};
