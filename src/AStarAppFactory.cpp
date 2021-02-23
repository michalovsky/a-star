#include "AStarAppFactory.h"

#include "MapBuilder.h"
#include "MapRenderer.h"
#include "UserInputReader.h"

std::unique_ptr<AStarApp> AStarAppFactory::createAStarApp(int mapWidth, int mapHeight)
{
    auto window = std::make_shared<sf::RenderWindow>(sf::VideoMode(45.5 * mapWidth, 45.5 * mapHeight),
                                                     "PathFinding A*");
    auto mapRenderer = std::make_shared<graphics::MapRenderer>(window);
    return std::make_unique<AStarApp>(window, mapRenderer, std::make_unique<MapBuilder>(mapRenderer),
                                      std::make_unique<input::UserInputReader>(window), mapWidth, mapHeight);
}
