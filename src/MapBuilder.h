#pragma once

#include <memory>

#include "MapRenderer.h"
#include "Node.h"

class MapBuilder
{
public:
    MapBuilder(std::shared_ptr<graphics::MapRenderer>);

    std::vector<Node> buildMap(int mapWidth, int mapHeight);

private:
    std::vector<Node> createNodes(int mapWidth, int mapHeight);
    void setNeighboursForNodes(int mapWidth, int mapHeight, std::vector<Node>&) const;
    void buildLinesBetweenNodes(int mapWidth, int mapHeight, const std::vector<Node>&);
    bool nodeHasNeighbourAbove(int y) const;
    bool nodeHasNeighbourBelow(int mapHeight, int y) const;
    bool nodeHasNeighbourOnLeft(int x) const;
    bool nodeHasNeighbourOnRight(int mapWidth, int x) const;
    void addLineToNodeAbove(const Node& node, const Node& nodeAbove);
    void addLineToNodeBelow(const Node& node, const Node& nodeBelow);
    void addLineToNodeOnLeft(const Node& node, const Node& nodeOnLeft);
    void addLineToNodeOnRight(const Node& node, const Node& nodeOnRight);
    void addLineBetweenNodes(const Node& startNode, const Node& endNode, const sf::Vector2f& startPositionOffset,
                             const sf::Vector2f& endPositionOffset);

    std::shared_ptr<graphics::MapRenderer> mapRenderer;
};