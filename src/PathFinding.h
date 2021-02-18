#pragma once
#include "Node.h"
#include <iostream>
#include <list>

class PathFinding
{
public:
	PathFinding();
	~PathFinding();
	void createMap();
	void drawMap(sf::RenderWindow & window);
	void updateMapByUser(sf::RenderWindow & window);
	void solveAStar();
	Node * nodes;
	Node * nodeStart = nullptr;
	Node * nodeEnd = nullptr;
	std::vector<sf::VertexArray> connections;
	int mapWidth = 20;
	int mapHeight = 20;
};

