#pragma once

#include "MapRenderer.h"
#include <memory>
#include "Node.h"

namespace graphics
{
class MapBuilder
{
public:
    MapBuilder(std::shared_ptr<MapRenderer>);

    void buildLinesBetweenNodes(int mapWidth, int mapHeight, std::vector<Node>);

private:
    std::shared_ptr<MapRenderer> mapRenderer;
};
}