#pragma once

#include "MapBuilder.h"
#include "MapRenderer.h"
#include "Node.h"
#include "UserInputReader.h"

class PathFinding
{
public:
    static void solveAStar(std::vector<Node>& nodes, Node* nodeStart, Node* nodeEnd);
};
