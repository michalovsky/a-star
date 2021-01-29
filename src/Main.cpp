#include <iostream>
#include <SFML/Graphics.hpp>
#include "PathFinding.h"
void clearWindow(sf::RenderWindow & window);

int main()
{
	sf::Clock clock;
	sf::RenderWindow window(sf::VideoMode(1000, 800), "PathFinding A*");
	window.setPosition(sf::Vector2i(10, 50));
	window.setFramerateLimit(60);
	PathFinding path;
	path.createMap();

	while (window.isOpen())
	{
		sf::Time elapsed = clock.getElapsedTime();
		clearWindow(window);

		if (elapsed.asSeconds() > 0.2)
		{
			clock.restart();
			path.updateMapByUser(window);
		}

		path.drawMap(window);

		window.display();

	}

	return 0;
}



void clearWindow(sf::RenderWindow & window)
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
