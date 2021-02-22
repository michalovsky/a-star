#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <memory>
#include <optional>

#include "InputKey.h"

namespace input
{
class UserInputReader
{
public:
    UserInputReader(std::shared_ptr<sf::RenderWindow>);

    std::unordered_map<InputKey, bool> readUserInput();
    sf::Vector2f readMousePosition() const;

private:
    std::shared_ptr<sf::RenderWindow> window;
};
}
