#include "PathFinding.h"

#include <cmath>

void PathFinding::solveAStar(std::vector<Node>& nodes, Node* nodeStart, Node* nodeEnd)
{
    // reset nodes
    for (auto& node : nodes)
    {
        node.visited = false;
        node.globalGoal = INFINITY;
        node.localGoal = INFINITY;
        node.parent = nullptr;
    }

    auto distance = [](Node* a, Node* b) {
        return sqrtf((a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y));
    };

    auto heuristic = [distance](Node* a, Node* b) { return distance(a, b); };

    // start conditions
    Node* nodeCurrent = nodeStart;
    nodeStart->localGoal = 0.0f;
    nodeStart->globalGoal = heuristic(nodeStart, nodeEnd);

    // not visited list
    std::list<Node*> listNotTestedNodes;
    listNotTestedNodes.push_back(nodeStart);

    // algorithm
    while (!listNotTestedNodes.empty() && nodeCurrent != nodeEnd)
    {
        // sorting
        listNotTestedNodes.sort(
            [](const Node* lhs, const Node* rhs) { return lhs->globalGoal < rhs->globalGoal; });
        // visited nodes
        while (!listNotTestedNodes.empty() && listNotTestedNodes.front()->visited)
        {
            listNotTestedNodes.pop_front();
        }

        // if no more nodes to visit end
        if (listNotTestedNodes.empty())
        {
            break;
        }

        nodeCurrent = listNotTestedNodes.front();
        nodeCurrent->visited = true; // only explore a node once

        for (auto nodeNeighbour : nodeCurrent->neighbours)
        {
            // if neighbour is not visited, is not obstacle add to not tested
            if (!nodeNeighbour->visited && !nodeNeighbour->obstacle)
            {
                listNotTestedNodes.push_back(nodeNeighbour);
            }

            // neighbours potential lowest parent distance
            float possiblyLowerGoal = nodeCurrent->localGoal + distance(nodeCurrent, nodeNeighbour);

            // if choosing to path through this node is a lower distance than what
            // the neighbour currently has set, update the neighbour to use this node
            if (possiblyLowerGoal < nodeNeighbour->localGoal)
            {
                nodeNeighbour->parent = nodeCurrent;
                nodeNeighbour->localGoal = possiblyLowerGoal;

                // the best path length to the neighbour being tested has changed so update global score
                nodeNeighbour->globalGoal = nodeNeighbour->localGoal + heuristic(nodeNeighbour, nodeEnd);
            }
        }
    }
}
