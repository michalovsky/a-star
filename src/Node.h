#pragma once

#include <memory>
#include <vector>

#include "GraphicsId.h"

struct Node
{
    bool obstacle = false;
    bool visited = false;
    float globalGoal; // dist to goal so far
    float localGoal;  // dist to goal if alternative
    int x, y;
    Node* parent = nullptr;
    std::vector<Node*> neighbours;
    graphics::GraphicsId graphicsId;
};
