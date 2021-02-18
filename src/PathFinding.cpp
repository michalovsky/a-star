#include "PathFinding.h"

#include <SFML/Graphics.hpp>
#include <cmath>

PathFinding::PathFinding(sf::RenderWindow& windowInit)
    : mapRenderer{std::make_unique<graphics::MapRenderer>(windowInit)}
{
}

PathFinding::~PathFinding()
{
    delete[] nodes;
}

void PathFinding::solveAStar()
{
    // reset nodes
    for (int y = 0; y < mapHeight; y++)
    {
        for (int x = 0; x < mapWidth; x++)
        {
            nodes[y * mapWidth + x].visited = false;
            nodes[y * mapWidth + x].globalGoal = INFINITY;
            nodes[y * mapWidth + x].localGoal = INFINITY;
            nodes[y * mapWidth + x].parent = nullptr;
        }
    }

    auto distance = [](Node* a, Node* b) {
        return sqrtf((a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y));
    };

    auto heuristic = [distance](Node* a, Node* b) { return distance(a, b); };

    // start conditions
    Node* nodeCurrent = nodeStart;
    nodeStart->localGoal = 0.0f;
    nodeStart->globalGoal = heuristic(nodeStart, nodeEnd);

    // not visited list
    std::list<Node*> listNotTestedNodes;
    listNotTestedNodes.push_back(nodeStart);

    // algorithm
    while (!listNotTestedNodes.empty() && nodeCurrent != nodeEnd)
    {
        // sorting
        listNotTestedNodes.sort(
            [](const Node* lhs, const Node* rhs) { return lhs->globalGoal < rhs->globalGoal; });
        // visited nodes
        while (!listNotTestedNodes.empty() && listNotTestedNodes.front()->visited)
            listNotTestedNodes.pop_front();

        // if no more nodes to visit end
        if (listNotTestedNodes.empty())
            break;

        nodeCurrent = listNotTestedNodes.front();
        nodeCurrent->visited = true; // only explore a node once

        for (auto nodeNeighbour : nodeCurrent->neighbours)
        {
            // if neighbour is not visited, is not obstacle add to not tested
            if (!nodeNeighbour->visited && !nodeNeighbour->obstacle)
                listNotTestedNodes.push_back(nodeNeighbour);

            // neighbours potential lowest parentt distance
            float possiblyLowerGoal = nodeCurrent->localGoal + distance(nodeCurrent, nodeNeighbour);

            // if choosing to path through this node is a lower distance than what
            // the neighbour currently has set, update the neighbour to use this node
            if (possiblyLowerGoal < nodeNeighbour->localGoal)
            {
                nodeNeighbour->parent = nodeCurrent;
                nodeNeighbour->localGoal = possiblyLowerGoal;

                // the best path length to the neibour being tested has changed so update global score
                nodeNeighbour->globalGoal = nodeNeighbour->localGoal + heuristic(nodeNeighbour, nodeEnd);
            }
        }
    }
}

void PathFinding::createMap()
{
    nodes = new Node[mapWidth * mapHeight];
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

    // connections
    for (int y = 0; y < mapHeight; y++)
    {
        for (int x = 0; x < mapWidth; x++)
        {
            if (y > 0)
            {
                nodes[y * mapWidth + x].neighbours.push_back(&nodes[(y - 1) * mapWidth + (x + 0)]);

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
                nodes[y * mapWidth + x].neighbours.push_back(&nodes[(y + 1) * mapWidth + (x + 0)]);

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
                nodes[y * mapWidth + x].neighbours.push_back(&nodes[(y + 0) * mapWidth + (x - 1)]);

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
                nodes[y * mapWidth + x].neighbours.push_back(&nodes[(y + 0) * mapWidth + (x + 1)]);

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

    nodeStart = &nodes[0];
    nodeEnd = &nodes[mapHeight / 2 * mapWidth + mapWidth + 2];
}

void PathFinding::updateMapByUser(sf::RenderWindow& window)
{
    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        bool changeObstacle = true;
        bool changeStartNode = false;
        bool changeEndNode = false;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
        {
            changeObstacle = false;
            changeEndNode = true;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
        {
            changeObstacle = false;
            changeStartNode = true;
        }

        for (int y = 0; y < mapHeight; y++)
        {
            for (int x = 0; x < mapWidth; x++)
            {
                const auto rectPosition = mapRenderer->getRectPosition(nodes[y * mapWidth + x].graphicsId);

                int xpos = rectPosition.x;
                int dx = xpos + 40;
                int ypos = rectPosition.y;
                int dy = ypos + 40;

                if (mousePosition.x >= xpos && mousePosition.x <= dx && mousePosition.y >= ypos &&
                    mousePosition.y <= dy)
                {
                    if (changeObstacle)
                    {
                        nodes[y * mapWidth + x].obstacle = !(nodes[y * mapWidth + x].obstacle);
                    }
                    else if (changeStartNode)
                    {
                        nodeStart = &nodes[y * mapWidth + x];
                    }
                    else if (changeEndNode)
                    {
                        nodeEnd = &nodes[y * mapWidth + x];
                    }
                }
            }
        }
        if (changeEndNode || changeObstacle || changeStartNode)
            solveAStar();
    }
}

void PathFinding::drawMap(sf::RenderWindow& window)
{
    for (int y = 0; y < mapHeight; y++)
    {
        for (int x = 0; x < mapWidth; x++)
        {
            if (&nodes[y * mapWidth + x] == nodeEnd)
            {
                mapRenderer->setColor(nodes[y * mapWidth + x].graphicsId, sf::Color::Red);
            }
            else if (&nodes[y * mapWidth + x] == nodeStart)
            {
                mapRenderer->setColor(nodes[y * mapWidth + x].graphicsId, sf::Color::Green);
            }
            else if (nodes[y * mapWidth + x].visited)
            {
                mapRenderer->setColor(nodes[y * mapWidth + x].graphicsId, sf::Color::Blue);
            }
            else if (!nodes[y * mapWidth + x].obstacle)
            {
                mapRenderer->setColor(nodes[y * mapWidth + x].graphicsId, sf::Color(65, 105, 225, 255));
            }
            else
            {
                mapRenderer->setColor(nodes[y * mapWidth + x].graphicsId, sf::Color(105, 105, 105, 255));
            }
        }
    }

    if (nodeEnd != nullptr)
    {
        Node* previous = nodeEnd;
        while (previous->parent != nullptr)
        {
            if (previous != nodeEnd)
            {
                mapRenderer->setColor(previous->graphicsId, sf::Color::Yellow);
            }
            previous = previous->parent;
        }
    }

    mapRenderer->renderAll();
}