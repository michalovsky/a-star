#include "PathFinding.h"

#include <cmath>
#include <SFML/Graphics.hpp>

PathFinding::PathFinding()
{
}


PathFinding::~PathFinding()
{
	delete[] nodes;
}

void PathFinding::solveAStar()
{
	//reset nodes
	for (int y = 0; y < mapHeight; y++)
	{
		for (int x = 0; x < mapWidth; x++)
		{
			nodes[y*mapWidth + x].visited = false;
			nodes[y*mapWidth + x].globalGoal = INFINITY;
			nodes[y*mapWidth + x].localGoal = INFINITY;
			nodes[y*mapWidth + x].parent = nullptr;
		}
	}

	auto distance = [](Node *a, Node *b)
	{
		return sqrtf((a->x - b->x)*(a->x - b->x) + (a->y - b->y)*(a->y - b->y));
	};

	auto heuristic = [distance](Node *a, Node *b)
	{
		return distance(a, b);
	};

	//start conditions
	Node * nodeCurrent = nodeStart;
	nodeStart->localGoal = 0.0f;
	nodeStart->globalGoal = heuristic(nodeStart, nodeEnd);

	//not visited list
	std::list<Node*> listNotTestedNodes;
	listNotTestedNodes.push_back(nodeStart);

	//algorith
	while (!listNotTestedNodes.empty() && nodeCurrent!=nodeEnd)
	{
		//sorting
		listNotTestedNodes.sort([](const Node * lhs, const Node * rhs) { return lhs->globalGoal < rhs->globalGoal; });
		//visited nodes
		while (!listNotTestedNodes.empty() && listNotTestedNodes.front()->visited)
			listNotTestedNodes.pop_front();

		//if no more nodes to visit end
		if (listNotTestedNodes.empty())
			break;

		nodeCurrent = listNotTestedNodes.front();
		nodeCurrent->visited = true; // only explore a node once

		for (auto nodeNeighbour : nodeCurrent->neighbours)
		{
			//if neighbour is not visited, is not obstacle add to not tested
			if (!nodeNeighbour->visited && !nodeNeighbour->obstacle)
				listNotTestedNodes.push_back(nodeNeighbour);

			//neighbours potential lowest parentt distance
			float possiblyLowerGoal = nodeCurrent->localGoal + distance(nodeCurrent, nodeNeighbour);

			//if choosing to path through this node is a lower distance than what
			// the neighbour currently has set, update the neighbour to use this node 
			if (possiblyLowerGoal < nodeNeighbour->localGoal)
			{
				nodeNeighbour->parent = nodeCurrent;
				nodeNeighbour->localGoal = possiblyLowerGoal;

				//the best path length to the neibour being tested has changed so update global score
				nodeNeighbour->globalGoal = nodeNeighbour->localGoal + heuristic(nodeNeighbour, nodeEnd);
			}
		}
	}
}

void PathFinding::createMap()
{
	nodes = new Node[mapWidth*mapHeight];
	for (int y = 0; y < mapHeight; y++)
	{
		for (int x = 0; x < mapWidth; x++)
		{
			nodes[y*mapWidth + x].x = x;
			nodes[y*mapWidth + x].y = y;
			nodes[y*mapWidth + x].obstacle = false;
			nodes[y*mapWidth + x].parent = nullptr;
			nodes[y*mapWidth + x].visited = false;
            const auto position = sf::Vector2f{5.f + (x * 5.f) + 40.f * x, 5.f + (y * 5.f) + 40.f * y};
            const auto size = sf::Vector2f{40.f, 40.f};
			nodes[y*mapWidth + x].rect = std::make_unique<graphics::RectangleSfml>(position, size, sf::Color::Blue);
		}
	}

	//connections
	for (int y = 0; y < mapHeight; y++)
	{
		for (int x = 0; x < mapWidth; x++)
		{
			if (y > 0)
			{
				nodes[y*mapWidth + x].neighbours.push_back(&nodes[(y - 1)*mapWidth + (x + 0)]);
				sf::VertexArray lines(sf::Lines, 2);
				lines[0].position = sf::Vector2f(nodes[y * mapWidth + x].rect->getX() + 20, nodes[y * mapWidth + x].rect->getY() + 10);
				lines[1].position = sf::Vector2f(nodes[(y - 1) * mapWidth + x].rect->getX() + 20, nodes[(y - 1) * mapWidth + x].rect->getY() + 30);
				connections.push_back(lines);
			}
			if (y < mapHeight - 1)
			{
				nodes[y*mapWidth + x].neighbours.push_back(&nodes[(y + 1)*mapWidth + (x + 0)]);
				sf::VertexArray lines(sf::Lines, 2);
				lines[0].position = sf::Vector2f(nodes[y * mapWidth + x].rect->getX() + 20, nodes[y * mapWidth + x].rect->getY() + 30);
				lines[1].position = sf::Vector2f(nodes[(y + 1) * mapWidth + x].rect->getX() + 20, nodes[(y + 1) * mapWidth + x].rect->getY() + 10);
				connections.push_back(lines);
			}
			if (x > 0)
			{
				nodes[y*mapWidth + x].neighbours.push_back(&nodes[(y + 0)*mapWidth + (x - 1)]);
				sf::VertexArray lines(sf::Lines, 2);
				lines[0].position = sf::Vector2f(nodes[y * mapWidth + x].rect->getX() + 10, nodes[y * mapWidth + x].rect->getY() + 20);
				lines[1].position = sf::Vector2f(nodes[(y)* mapWidth + x - 1].rect->getX() + 30, nodes[y * mapWidth + x - 1].rect->getY() + 20);
				connections.push_back(lines);
			}
			if (x < mapWidth - 1)
			{
				nodes[y*mapWidth + x].neighbours.push_back(&nodes[(y + 0)*mapWidth + (x + 1)]);
				sf::VertexArray lines(sf::Lines, 2);
				lines[0].position = sf::Vector2f(nodes[y * mapWidth + x].rect->getX() + 30, nodes[y * mapWidth + x].rect->getY() + 20);
				lines[1].position = sf::Vector2f(nodes[(y)* mapWidth + x + 1].rect->getX() + 10, nodes[(y)* mapWidth + x + 1].rect->getY() + 20);
				connections.push_back(lines);
			}
		}
	}

	nodeStart = &nodes[0];
	nodeEnd = &nodes[mapHeight / 2 * mapWidth + mapWidth + 2];

}

void PathFinding::updateMapByUser(sf::RenderWindow & window)
{
	sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		bool changeObstacle = true;
		bool changeStartNode = false;
		bool changeEndNode = false;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
		{
			changeObstacle = false;
			changeEndNode = true;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
		{
			changeObstacle = false;
			changeStartNode = true;
		}

		for (int y = 0; y < mapHeight; y++)
		{
			for (int x = 0; x < mapWidth; x++)
			{
				int xpos = (int)nodes[y * mapWidth + x].rect->getX();
				int dx = xpos + 40;
				int ypos = (int)nodes[y * mapWidth + x].rect->getY();
				int dy = ypos + 40;

				if (mousePosition.x >= xpos && mousePosition.x <= dx && mousePosition.y >= ypos && mousePosition.y <= dy)
				{
					if(changeObstacle) nodes[y * mapWidth + x].obstacle = !(nodes[y * mapWidth + x].obstacle);
					else if (changeStartNode) nodeStart = &nodes[y * mapWidth + x];
					else if (changeEndNode) nodeEnd = &nodes[y * mapWidth + x];
				}
			}
		}
		if (changeEndNode || changeObstacle || changeStartNode) solveAStar();
	}
}

void PathFinding::drawMap(sf::RenderWindow & window)
{
	for (int y = 0; y < mapHeight; y++)
	{
		for (int x = 0; x < mapWidth; x++)
		{
			if (&nodes[y * mapWidth + x] == nodeEnd) nodes[y * mapWidth + x].rect->changeColor(sf::Color::Red);
			else if (&nodes[y * mapWidth + x] == nodeStart) nodes[y * mapWidth + x].rect->changeColor(sf::Color::Green);
			else if (nodes[y * mapWidth + x].visited) nodes[y * mapWidth + x].rect->changeColor(sf::Color::Blue);
			else if (!nodes[y * mapWidth + x].obstacle) nodes[y * mapWidth + x].rect->changeColor(sf::Color(65, 105, 225, 255));
			else nodes[y * mapWidth + x].rect->changeColor(sf::Color(105,105,105,255));
			nodes[y * mapWidth + x].rect->draw(window);
		}
	}

	if (nodeEnd != nullptr)
	{
		bool notFirst = false;
		Node *previous = nodeEnd;
		while (previous->parent != nullptr)
		{
			previous->rect->changeColor(sf::Color::Yellow);
			if (notFirst)
            {
                previous->rect->draw(window);
            }
			previous = previous->parent;
			notFirst = true;
		}
	}

	for (auto & connection : connections)
	{
		window.draw(connection);
	}
}