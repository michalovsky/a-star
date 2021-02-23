#pragma once

#include <memory>

#include "MapRenderer.h"
#include "Node.h"

class MapBuilder
{
public:
    MapBuilder(std::shared_ptr<graphics::MapRenderer>);

    std::vector<Node> buildMap(int mapWidth, int mapHeight);

private:
    void buildLinesBetweenNodes(int mapWidth, int mapHeight, std::vector<Node>&);

    std::shared_ptr<graphics::MapRenderer> mapRenderer;
};