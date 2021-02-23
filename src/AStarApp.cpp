#include "AStarApp.h"

#include <SFML/Window/Event.hpp>

namespace
{
const auto timeAfterUserCanClickAgain = 0.2;
}

AStarApp::AStarApp(std::shared_ptr<sf::RenderWindow> windowInit,
                   std::shared_ptr<graphics::MapRenderer> rendererInit,
                   std::unique_ptr<MapBuilder> mapBuilderInit,
                   std::unique_ptr<input::UserInputReader> inputReaderInit, int mapWidthInit,
                   int mapHeightInit)
    : window{std::move(windowInit)},
      mapRenderer{std::move(rendererInit)},
      mapBuilder{std::move(mapBuilderInit)},
      inputReader{std::move(inputReaderInit)},
      mapWidth{mapWidthInit},
      mapHeight{mapHeightInit}
{
}

void AStarApp::run()
{
    createMap();
    PathFinding::solveAStar(nodes, nodeStart, nodeEnd);
    while (window->isOpen())
    {
        clearWindow();
        update();
        drawWindow();
    }
}

void AStarApp::createMap()
{
    nodes = mapBuilder->buildMap(mapWidth, mapHeight);
    nodeStart = &nodes[0];
    nodeEnd = &nodes[mapHeight / 2 * mapWidth + mapWidth + 2];
}

void AStarApp::update()
{
    const auto mousePosition = inputReader->readMousePosition();
    const auto userKeysInput = inputReader->readUserInput();

    if (windowNeedToBeClosed(userKeysInput))
    {
        closeWindow();
        return;
    }

    if (not userKeysInput.at(input::InputKey::MouseLeft))
    {
        return;
    }

    for (auto& node : nodes)
    {
        if (mapRenderer->intersects(node.graphicsId, mousePosition) &&
            clock.getElapsedTime().asSeconds() >= timeAfterUserCanClickAgain)
        {
            clock.restart();

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

            PathFinding::solveAStar(nodes, nodeStart, nodeEnd);
        }
    }
}

void AStarApp::clearWindow()
{
    window->clear(sf::Color::Black);
}

void AStarApp::closeWindow()
{
    window->close();
}

void AStarApp::drawWindow()
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
    window->display();
}

bool AStarApp::windowNeedToBeClosed(const std::unordered_map<input::InputKey, bool>& userKeysInput)
{
    sf::Event event{};
    while (window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            return true;
        }
    }
    return userKeysInput.at(input::InputKey::Escape);
}
