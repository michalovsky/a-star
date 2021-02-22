#include "MapBuilder.h"

MapBuilder::MapBuilder(std::shared_ptr<graphics::MapRenderer> mapRendererInit)
    : mapRenderer{std::move(mapRendererInit)}
{
}

std::vector<Node> MapBuilder::buildMap(int mapWidth, int mapHeight)
{
    std::vector<Node> nodes(mapHeight * mapWidth);

    for (int y = 0; y < mapHeight; y++)
    {
        for (int x = 0; x < mapWidth; x++)
        {
            nodes[y * mapWidth + x].x = x;
            nodes[y * mapWidth + x].y = y;
            const auto position = sf::Vector2f{5.f + (x * 5.f) + 40.f * x, 5.f + (y * 5.f) + 40.f * y};
            const auto size = sf::Vector2f{40.f, 40.f};
            nodes[y * mapWidth + x].graphicsId = mapRenderer->acquireRect(position, size, sf::Color::Blue);
        }
    }

    // setting neighbours
    for (int y = 0; y < mapHeight; y++)
    {
        for (int x = 0; x < mapWidth; x++)
        {
            if (y > 0)
            {
                nodes[y * mapWidth + x].neighbours.push_back(&nodes[(y - 1) * mapWidth + x]);
            }
            if (y < mapHeight - 1)
            {
                nodes[y * mapWidth + x].neighbours.push_back(&nodes[(y + 1) * mapWidth + x]);
            }
            if (x > 0)
            {
                nodes[y * mapWidth + x].neighbours.push_back(&nodes[y * mapWidth + x - 1]);
            }
            if (x < mapWidth - 1)
            {
                nodes[y * mapWidth + x].neighbours.push_back(&nodes[y * mapWidth + x + 1]);
            }
        }
    }

    buildLinesBetweenNodes(mapWidth, mapHeight, nodes);

    return nodes;
}

void MapBuilder::buildLinesBetweenNodes(int mapWidth, int mapHeight, std::vector<Node>& nodes)
{
    for (int y = 0; y < mapHeight; y++)
    {
        for (int x = 0; x < mapWidth; x++)
        {
            if (y > 0)
            {
                const auto startRectPosition =
                    mapRenderer->getRectPosition(nodes[y * mapWidth + x].graphicsId);
                const auto endRectPosition =
                    mapRenderer->getRectPosition(nodes[(y - 1) * mapWidth + x].graphicsId);
                const auto startLinePosition =
                    sf::Vector2f{startRectPosition.x + 20, startRectPosition.y + 10};
                const auto endLinePosition = sf::Vector2f{endRectPosition.x + 20, endRectPosition.y + 30};
                mapRenderer->addLine(startLinePosition, endLinePosition);
            }
            if (y < mapHeight - 1)
            {
                const auto startRectPosition =
                    mapRenderer->getRectPosition(nodes[y * mapWidth + x].graphicsId);
                const auto endRectPosition =
                    mapRenderer->getRectPosition(nodes[(y + 1) * mapWidth + x].graphicsId);
                const auto startLinePosition =
                    sf::Vector2f{startRectPosition.x + 20, startRectPosition.y + 30};
                const auto endLinePosition = sf::Vector2f{endRectPosition.x + 20, endRectPosition.y + 10};
                mapRenderer->addLine(startLinePosition, endLinePosition);
            }
            if (x > 0)
            {
                const auto startRectPosition =
                    mapRenderer->getRectPosition(nodes[y * mapWidth + x].graphicsId);
                const auto endRectPosition =
                    mapRenderer->getRectPosition(nodes[y * mapWidth + x - 1].graphicsId);
                const auto startLinePosition =
                    sf::Vector2f{startRectPosition.x + 10, startRectPosition.y + 20};
                const auto endLinePosition = sf::Vector2f{endRectPosition.x + 30, endRectPosition.y + 20};
                mapRenderer->addLine(startLinePosition, endLinePosition);
            }
            if (x < mapWidth - 1)
            {
                const auto startRectPosition =
                    mapRenderer->getRectPosition(nodes[y * mapWidth + x].graphicsId);
                const auto endRectPosition =
                    mapRenderer->getRectPosition(nodes[y * mapWidth + x + 1].graphicsId);
                const auto startLinePosition =
                    sf::Vector2f{startRectPosition.x + 30, startRectPosition.y + 20};
                const auto endLinePosition = sf::Vector2f{endRectPosition.x + 10, endRectPosition.y + 20};
                mapRenderer->addLine(startLinePosition, endLinePosition);
            }
        }
    }
}
