#include "AStarAppFactory.h"

int main()
{
    auto aStarApp = AStarAppFactory::createAStarApp();
    aStarApp->run();
    return 0;
}