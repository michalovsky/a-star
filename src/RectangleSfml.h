#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

namespace graphics
{
class RectangleSfml
{
public:
    RectangleSfml(const sf::Vector2f& position, const sf::Vector2f& size, sf::Color color);
    RectangleSfml() = default;

    void changeColor(sf::Color);
    bool isIntersecting(const sf::Vector2f& position) const;
    void draw(sf::RenderWindow&) const;
    sf::Vector2f getPosition() const;
    float getX() const;
    float getY() const;

private:
    sf::RectangleShape rect;
};
}