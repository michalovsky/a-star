#include "LineSfml.h"

namespace graphics
{

LineSfml::LineSfml(const sf::Vector2f& startPosition, const sf::Vector2f& endPosition) : line{sf::Lines, 2}
{
    line[0].position = startPosition;
    line[1].position = endPosition;
}

void LineSfml::draw(sf::RenderWindow& window) const
{
    window.draw(line);
}

}