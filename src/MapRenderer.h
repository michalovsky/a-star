#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <boost/functional/hash.hpp>

#include "GraphicsId.h"
#include "LineSfml.h"
#include "RectangleSfml.h"

namespace graphics
{
class MapRenderer
{
public:
    MapRenderer(sf::RenderWindow&);

    GraphicsId acquireRect(const sf::Vector2f& position, const sf::Vector2f& size, const sf::Color&);
    void addLine(const sf::Vector2f& startPosition, const sf::Vector2f& endPosition);
    void setColor(const GraphicsId&, const sf::Color&);
    sf::Vector2f getRectPosition(const GraphicsId&) const;
    void renderAll();

private:
    sf::RenderWindow& window;
    std::unordered_map<GraphicsId, RectangleSfml, boost::hash<GraphicsId>> rects;
    std::vector<LineSfml> lines;
};
}