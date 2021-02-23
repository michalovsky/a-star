#include "PathFinding.h"

#include <cmath>
#include <list>

void PathFinding::solveAStar(std::vector<Node>& nodes, Node* startNode, Node* endNode)
{
    resetNodes(nodes);

    auto distance = [](Node* a, Node* b) {
        return sqrtf((a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y));
    };

    auto heuristic = [distance](Node* a, Node* b) { return distance(a, b); };

    Node* currentNode = startNode;
    startNode->distanceFromStartingNode = 0.0f;
    startNode->distanceFromEndNode = heuristic(startNode, endNode);

    std::list<Node*> nodesToVisit;
    nodesToVisit.push_back(startNode);

    while (not nodesToVisit.empty() && currentNode != endNode)
    {
        nodesToVisit.sort([](const Node* lhs, const Node* rhs) {
            return lhs->distanceFromEndNode < rhs->distanceFromEndNode;
        });

        while (not nodesToVisit.empty() && nodesToVisit.front()->visited)
        {
            nodesToVisit.pop_front();
        }

        if (nodesToVisit.empty())
        {
            break;
        }

        currentNode = nodesToVisit.front();
        currentNode->visited = true;

        for (auto neighbourNode : currentNode->neighbours)
        {
            if (nodeIsNotAlreadyVisited(neighbourNode) && nodeIsNotObstacle(neighbourNode))
            {
                nodesToVisit.push_back(neighbourNode);
            }

            float possiblyLowerDistance =
                currentNode->distanceFromStartingNode + distance(currentNode, neighbourNode);

            if (possiblyLowerDistance < neighbourNode->distanceFromStartingNode)
            {
                neighbourNode->parent = currentNode;
                neighbourNode->distanceFromStartingNode = possiblyLowerDistance;

                neighbourNode->distanceFromEndNode =
                    neighbourNode->distanceFromStartingNode + heuristic(neighbourNode, endNode);
            }
        }
    }
}

void PathFinding::resetNodes(std::vector<Node>& nodes)
{
    for (auto& node : nodes)
    {
        node.visited = false;
        node.distanceFromEndNode = INFINITY;
        node.distanceFromStartingNode = INFINITY;
        node.parent = nullptr;
    }
}

bool PathFinding::nodeIsNotObstacle(const Node* neighbourNode)
{
    return not neighbourNode->obstacle;
}

bool PathFinding::nodeIsNotAlreadyVisited(const Node* neighbourNode)
{
    return not neighbourNode->visited;
}
