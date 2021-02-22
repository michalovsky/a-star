#include "UserInputReader.h"

#include <SFML/Window/Event.hpp>

namespace input
{

UserInputReader::UserInputReader(std::shared_ptr<sf::RenderWindow> windowInit) : window{std::move(windowInit)}
{
}

std::unordered_map<InputKey, bool> UserInputReader::readUserInput()
{
    std::unordered_map<InputKey, bool> userInput(allKeys.size());

    for (const auto& key : allKeys)
    {
        userInput[key] = false;
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        userInput[InputKey::MouseLeft] = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
    {
        userInput[InputKey::ControlLeft] = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
    {
        userInput[InputKey::ShiftLeft] = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
    {
        userInput[InputKey::Escape] = true;
    }

    return userInput;
}

sf::Vector2f UserInputReader::readMousePosition() const
{
    return sf::Vector2f(sf::Mouse::getPosition(*window));
}
}
