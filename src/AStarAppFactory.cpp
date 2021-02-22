#include "AStarAppFactory.h"

#include "MapBuilder.h"
#include "MapRenderer.h"
#include "UserInputReader.h"

std::unique_ptr<AStarApp> AStarAppFactory::createAStarApp()
{
    auto window = std::make_shared<sf::RenderWindow>(sf::VideoMode(910, 910), "PathFinding A*");
    window->setFramerateLimit(60);

    auto mapRenderer = std::make_shared<graphics::MapRenderer>(window);

    return std::make_unique<AStarApp>(window, mapRenderer, std::make_unique<MapBuilder>(mapRenderer),
                                      std::make_unique<input::UserInputReader>(window));
}
