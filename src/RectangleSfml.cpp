#include "RectangleSfml.h"

namespace graphics
{
RectangleSfml::RectangleSfml(const sf::Vector2f& position, const sf::Vector2f& size, sf::Color color)
{
    rect.setSize(size);
    rect.setFillColor(sf::Color::White);
    rect.setOutlineColor(sf::Color::Black);
    rect.setPosition(position);
    rect.setFillColor(color);
}

void RectangleSfml::changeColor(sf::Color color)
{
    rect.setFillColor(color);
}

bool RectangleSfml::isIntersecting(const sf::Vector2f& position) const
{
    const sf::Vector2f rectPosition = rect.getPosition();
    const float width = rect.getSize().x;
    const float height = rect.getSize().y;

    return (position.x >= rectPosition.x && position.x <= rectPosition.x + width &&
            position.y >= rectPosition.y && position.y <= rectPosition.y + height);
}

void RectangleSfml::draw(sf::RenderWindow& window)
{
    window.draw(rect);
}

float RectangleSfml::getX() const
{
    return rect.getPosition().x;
}

float RectangleSfml::getY() const
{
    return rect.getPosition().y;
}
}