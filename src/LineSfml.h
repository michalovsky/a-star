#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/VertexArray.hpp>

namespace graphics
{
class LineSfml
{
public:
    LineSfml(const sf::Vector2f& startPosition, const sf::Vector2f& endPosition);

    void draw(sf::RenderWindow&) const;

private:
    sf::VertexArray line;
};
}