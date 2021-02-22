#include "MapRenderer.h"

#include <utility>

#include "GraphicsIdGenerator.h"

namespace graphics
{

MapRenderer::MapRenderer(std::shared_ptr<sf::RenderWindow> windowInit) : window{std::move(windowInit)} {}

GraphicsId MapRenderer::acquireRect(const sf::Vector2f& position, const sf::Vector2f& size,
                                    const sf::Color& color)
{
    const auto id = GraphicsIdGenerator::generateId();
    rects.insert(std::make_pair(id, RectangleSfml{position, size, color}));
    return id;
}

void MapRenderer::addLine(const sf::Vector2f& startPosition, const sf::Vector2f& endPosition)
{
    auto line = LineSfml{startPosition, endPosition};
    lines.emplace_back(line);
}

void MapRenderer::setColor(const GraphicsId& id, const sf::Color& color)
{
    if (rects.count(id))
    {
        rects[id].changeColor(color);
    }
}

sf::Vector2f MapRenderer::getRectPosition(const GraphicsId& id) const
{
    return rects.at(id).getPosition();
}

bool MapRenderer::intersects(const GraphicsId& id, const sf::Vector2f& position) const
{
    return rects.at(id).isIntersecting(position);
}

void MapRenderer::renderAll()
{
    for (const auto& rect : rects)
    {
        rect.second.draw(*window);
    }

    for (const auto& line : lines)
    {
        line.draw(*window);
    }
}

}