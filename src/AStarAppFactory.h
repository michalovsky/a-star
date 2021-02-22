#pragma once

#include "AStarApp.h"

class AStarAppFactory
{
public:
    static std::unique_ptr<AStarApp> createAStarApp();
};