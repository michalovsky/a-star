#include "MapBuilder.h"

namespace
{
const auto rectWidth = 40.f;
const auto rectHeight = 40.f;
const auto rectSize = sf::Vector2f{rectWidth, rectHeight};
const auto offsetBetweenRects = 5.f;
}

MapBuilder::MapBuilder(std::shared_ptr<graphics::MapRenderer> mapRendererInit)
    : mapRenderer{std::move(mapRendererInit)}
{
}

std::vector<Node> MapBuilder::buildMap(int mapWidth, int mapHeight)
{
    auto nodes = createNodes(mapWidth, mapHeight);
    setNeighboursForNodes(mapWidth, mapHeight, nodes);
    buildLinesBetweenNodes(mapWidth, mapHeight, nodes);
    return nodes;
}

std::vector<Node> MapBuilder::createNodes(int mapWidth, int mapHeight)
{
    std::vector<Node> nodes(mapHeight * mapWidth);

    for (int y = 0; y < mapHeight; y++)
    {
        for (int x = 0; x < mapWidth; x++)
        {
            nodes[y * mapWidth + x].x = x;
            nodes[y * mapWidth + x].y = y;
            const auto position =
                sf::Vector2f{offsetBetweenRects + (x * offsetBetweenRects) + rectWidth * x,
                             offsetBetweenRects + (y * offsetBetweenRects) + rectHeight * y};
            nodes[y * mapWidth + x].graphicsId =
                mapRenderer->acquireRect(position, rectSize, sf::Color::Blue);
        }
    }
    return nodes;
}

void MapBuilder::setNeighboursForNodes(int mapWidth, int mapHeight, std::vector<Node>& nodes) const
{
    for (int y = 0; y < mapHeight; y++)
    {
        for (int x = 0; x < mapWidth; x++)
        {
            auto& node = nodes[y * mapWidth + x];
            if (nodeHasNeighbourAbove(y))
            {
                auto& nodeAbove = nodes[(y - 1) * mapWidth + x];
                node.neighbours.push_back(&nodeAbove);
            }
            if (nodeHasNeighbourBelow(mapHeight, y))
            {
                auto& nodeBelow = nodes[(y + 1) * mapWidth + x];
                node.neighbours.push_back(&nodeBelow);
            }
            if (nodeHasNeighbourOnLeft(x))
            {
                auto& nodeOnLeft = nodes[y * mapWidth + x - 1];
                node.neighbours.push_back(&nodeOnLeft);
            }
            if (nodeHasNeighbourOnRight(mapWidth, x))
            {
                auto& nodeOnRight = nodes[y * mapWidth + x + 1];
                node.neighbours.push_back(&nodeOnRight);
            }
        }
    }
}

void MapBuilder::buildLinesBetweenNodes(int mapWidth, int mapHeight, const std::vector<Node>& nodes)
{
    for (int y = 0; y < mapHeight; y++)
    {
        for (int x = 0; x < mapWidth; x++)
        {
            auto& node = nodes[y * mapWidth + x];
            if (nodeHasNeighbourAbove(y))
            {
                auto& nodeAbove = nodes[(y - 1) * mapWidth + x];
                addLineToNodeAbove(node, nodeAbove);
            }
            if (nodeHasNeighbourBelow(mapHeight, y))
            {
                auto& nodeBelow = nodes[(y + 1) * mapWidth + x];
                addLineToNodeBelow(node, nodeBelow);
            }
            if (nodeHasNeighbourOnLeft(x))
            {
                auto& nodeOnLeft = nodes[y * mapWidth + x - 1];
                addLineToNodeOnLeft(node, nodeOnLeft);
            }
            if (nodeHasNeighbourOnRight(mapWidth, x))
            {
                auto& nodeOnRight = nodes[y * mapWidth + x + 1];
                addLineToNodeOnRight(node, nodeOnRight);
            }
        }
    }
}

bool MapBuilder::nodeHasNeighbourOnRight(int mapWidth, int x) const
{
    return x < mapWidth - 1;
}

bool MapBuilder::nodeHasNeighbourOnLeft(int x) const
{
    return x > 0;
}

bool MapBuilder::nodeHasNeighbourBelow(int mapHeight, int y) const
{
    return y < mapHeight - 1;
}

bool MapBuilder::nodeHasNeighbourAbove(int y) const
{
    return y > 0;
}

void MapBuilder::addLineToNodeOnRight(const Node& node, const Node& nodeOnRight)
{
    auto startPositionOffset = sf::Vector2f{30, 20};
    auto endPositionOffset = sf::Vector2f{10, 20};
    addLineBetweenNodes(node, nodeOnRight, startPositionOffset, endPositionOffset);
}

void MapBuilder::addLineToNodeOnLeft(const Node& node, const Node& nodeOnLeft)
{
    auto startPositionOffset = sf::Vector2f{10, 20};
    auto endPositionOffset = sf::Vector2f{30, 20};
    addLineBetweenNodes(node, nodeOnLeft, startPositionOffset, endPositionOffset);
}

void MapBuilder::addLineToNodeBelow(const Node& node, const Node& nodeBelow)
{
    auto startPositionOffset = sf::Vector2f{20, 30};
    auto endPositionOffset = sf::Vector2f{20, 10};
    addLineBetweenNodes(node, nodeBelow, startPositionOffset, endPositionOffset);
}

void MapBuilder::addLineToNodeAbove(const Node& node, const Node& nodeAbove)
{
    auto startPositionOffset = sf::Vector2f{20, 10};
    auto endPositionOffset = sf::Vector2f{20, 30};
    addLineBetweenNodes(node, nodeAbove, startPositionOffset, endPositionOffset);
}

void MapBuilder::addLineBetweenNodes(const Node& startNode, const Node& endNode,
                                     const sf::Vector2f& startPositionOffset,
                                     const sf::Vector2f& endPositionOffset)
{
    const auto startRectPosition = mapRenderer->getRectPosition(startNode.graphicsId);
    const auto endRectPosition = mapRenderer->getRectPosition(endNode.graphicsId);
    const auto startLinePosition = sf::Vector2f{startRectPosition.x + startPositionOffset.x,
                                                startRectPosition.y + startPositionOffset.y};
    const auto endLinePosition =
        sf::Vector2f{endRectPosition.x + endPositionOffset.x, endRectPosition.y + endPositionOffset.y};
    mapRenderer->addLine(startLinePosition, endLinePosition);
}
