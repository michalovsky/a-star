#include "PathFinding.h"

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

PathFinding::PathFinding(std::shared_ptr<sf::RenderWindow> window)
    : mapRenderer{std::make_shared<graphics::MapRenderer>(window)},
      mapBuilder{std::make_unique<MapBuilder>(mapRenderer)},
      inputReader{std::make_unique<input::UserInputReader>(window)}
{
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

            // neighbours potential lowest parent distance
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
    nodes = mapBuilder->buildMap(mapWidth, mapHeight);
    nodeStart = &nodes[0];
    nodeEnd = &nodes[mapHeight / 2 * mapWidth + mapWidth + 2];
}

void PathFinding::updateMapByUser(sf::RenderWindow& window)
{
    const auto mousePosition = inputReader->readMousePosition();
    const auto userKeysInput = inputReader->readUserInput();

    if (not userKeysInput.at(input::InputKey::MouseLeft))
    {
        return;
    }

    for (auto& node : nodes)
    {
        if (mapRenderer->intersects(node.graphicsId, mousePosition))
        {
            if (userKeysInput.at(input::InputKey::ControlLeft))
            {
                nodeEnd = &node;
            }
            else if (userKeysInput.at(input::InputKey::ShiftLeft))
            {
                nodeStart = &node;
            }
            else
            {
                node.obstacle = !node.obstacle;
            }
        }
    }

    solveAStar();
}

void PathFinding::drawMap(sf::RenderWindow& window)
{
    for (const auto& node : nodes)
    {
        if (&node == nodeEnd)
        {
            mapRenderer->setColor(node.graphicsId, sf::Color::Red);
        }
        else if (&node == nodeStart)
        {
            mapRenderer->setColor(node.graphicsId, sf::Color::Green);
        }
        else if (node.visited)
        {
            mapRenderer->setColor(node.graphicsId, sf::Color::Blue);
        }
        else if (!node.obstacle)
        {
            mapRenderer->setColor(node.graphicsId, sf::Color(65, 105, 225, 255));
        }
        else
        {
            mapRenderer->setColor(node.graphicsId, sf::Color(0, 0, 0, 255));
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