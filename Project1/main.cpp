#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <string>

#include <iostream>
#include <vector>

#include "Block.h"
#include "Piece.h"


int main()
{
	sf::RenderWindow game_window(sf::VideoMode(800, 600, 32), "Tetris v.1.0", sf::Style::Titlebar | sf::Style::Close);
	sf::Event event;

	sf::Texture block_texture;
	if (!block_texture.loadFromFile("iceblock.jpg"))
	{
		std::cerr << "Falied to load texture";
	}

	sf::Vector2f p1(0, 20);   // moze wpisac ta stala rozmiaru klocka wszedzie gdzie 20?
	sf::Vector2f p2(20, 20);
	sf::Vector2f p3(40, 20);
	sf::Vector2f p4(20, 0);

	std::vector< sf::Vector2f> wsp;
	wsp.push_back(p1);
	wsp.push_back(p2);
	wsp.push_back(p3);
	wsp.push_back(p4);


	Piece first_p(4, wsp, sf::Vector2f(20, 20), block_texture, sf::Color::Red);

	first_p.setPosition(200, 20); // moze wpisac ta stala rozmiaru klocka?
	
	sf::Clock game_time; // zegarek do ruchu w dol


	while (game_window.isOpen())
	{
		game_window.clear(sf::Color::Black);

		while(game_window.pollEvent(event))
		{

			switch (event.type)
			{
			case sf::Event::Closed:
				{
					game_window.close();
					break;
				}
			}

		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			first_p.rotateRight();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			first_p.moveRight();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			first_p.moveLeft();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			first_p.moveDown();
		}



		sf::Time elapsed = game_time.getElapsedTime();
		if (static_cast<int>(elapsed.asMilliseconds()) > 1000)
		{
			first_p.moveDown();
			game_time.restart();
		}


		game_window.draw(first_p);

		game_window.display();



	}
}