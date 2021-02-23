#pragma once

#include "MapBuilder.h"
#include "MapRenderer.h"
#include "Node.h"
#include "PathFinding.h"
#include "UserInputReader.h"

class AStarApp
{
public:
    AStarApp(std::shared_ptr<sf::RenderWindow>, std::shared_ptr<graphics::MapRenderer>,
             std::unique_ptr<MapBuilder>, std::unique_ptr<input::UserInputReader>, int mapWidth,
             int mapHeight);

    void run();

private:
    void createMap();
    void update();
    void clearWindow();
    void closeWindow();
    void drawWindow();
    bool windowNeedToBeClosed(const std::unordered_map<input::InputKey, bool>&);

    std::shared_ptr<sf::RenderWindow> window;
    std::shared_ptr<graphics::MapRenderer> mapRenderer;
    std::unique_ptr<MapBuilder> mapBuilder;
    std::unique_ptr<input::UserInputReader> inputReader;
    int mapWidth, mapHeight;
    std::vector<Node> nodes;
    Node* nodeStart = nullptr;
    Node* nodeEnd = nullptr;
    sf::Clock clock;
};