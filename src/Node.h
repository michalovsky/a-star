#pragma once

#include "RectangleSfml.h"
#include <vector>
#include <memory>

struct Node
{
	bool obstacle = false;
	bool visited = false;
	float globalGoal; //dist to goal so far
	float localGoal; //dist to goal if alternative
	int x, y;
	Node * parent;
	std::vector<Node *> neighbours;
	std::unique_ptr<graphics::RectangleSfml> rect = nullptr;
};

