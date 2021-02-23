#pragma once

#include "Node.h"

class PathFinding
{
public:
    static void solveAStar(std::vector<Node>& nodes, Node* startNode, Node* endNode);

private:
    static void resetNodes(std::vector<Node>& nodes);
    static bool nodeIsNotAlreadyVisited(const Node* neighbourNode);
    static bool nodeIsNotObstacle(const Node* neighbourNode);
};
