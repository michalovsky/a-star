#include <SFML/Graphics.hpp>
#include <iostream>

#include "PathFinding.h"
void clearWindow(sf::RenderWindow& window);

int main()
{
    sf::Clock clock;
    sf::RenderWindow window(sf::VideoMode(910, 910), "PathFinding A*");
    window.setFramerateLimit(60);
    PathFinding pathFinding{window};
    pathFinding.createMap();

    while (window.isOpen())
    {
        sf::Time elapsed = clock.getElapsedTime();
        clearWindow(window);

        if (elapsed.asSeconds() > 0.1)
        {
            clock.restart();
            pathFinding.updateMapByUser(window);
        }

        pathFinding.drawMap(window);

        window.display();
    }

    return 0;
}

void clearWindow(sf::RenderWindow& window)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window.close();
        }

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
        {
            window.close();
        }
    }
    window.clear();
}
