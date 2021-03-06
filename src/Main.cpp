#include "AStarAppFactory.h"

int main()
{
    const int mapWidth = 20;
    const int mapHeight = 20;
    auto aStarApp = AStarAppFactory::createAStarApp(mapWidth, mapHeight);
    aStarApp->run();
    return 0;
}