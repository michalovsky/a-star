#pragma once

#include <vector>

#include "GraphicsId.h"

struct Node
{
    int x, y;
    float distanceFromStartingNode;
    float distanceFromEndNode;
    bool obstacle = false;
    bool visited = false;
    Node* parent = nullptr;
    std::vector<Node*> neighbours;
    graphics::GraphicsId graphicsId;
};
