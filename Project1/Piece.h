#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Block.h"
#include <iostream>

//reaction time in ms - for movement
constexpr int REACT_TIME{ 150 };

//class for complex pieces (made from blocks)
class Piece : public sf::Transformable, public sf::Drawable
{
public:

	// create single piece - quantity of blocks and each coordinates needed, also origin (center point) of piece, texture and color
	Piece(int quantity, std::vector<sf::Vector2f> coords, sf::Vector2f center_point, sf::Texture &texture, sf::Color color)
	{
		if (quantity != coords.size())
		{
			std::cerr << "Wrong number of coordinates";
		}

		for (int i = 0; i < quantity; i++)
		{
			piece_blocks.push_back(Block(texture, color));
			piece_blocks[i].setPosition(coords[i]);
		}

		this->setOrigin(center_point);
	}

	//-----------------------------------------------------------------------------------------------------
	//Transformation methods

	void rotateRight()
	{
		sf::Time elapsed;
		elapsed = clock_rotate.getElapsedTime();

		if (elapsed.asMilliseconds() >= REACT_TIME)
		{
			this->rotate(90);
			clock_rotate.restart();
		}
	}

	void moveLeft()
	{
		sf::Time elapsed;
		elapsed = clock_move_Hor.getElapsedTime();

		if (elapsed.asMilliseconds() >= REACT_TIME)
		{
			this->move(sf::Vector2f(-BLOCK_SIZE, 0));
			clock_move_Hor.restart();
		}
	}

	void moveRight()
	{
		sf::Time elapsed;
		elapsed = clock_move_Hor.getElapsedTime();

		if (elapsed.asMilliseconds() >= REACT_TIME)
		{
			this->move(sf::Vector2f(BLOCK_SIZE, 0));
			clock_move_Hor.restart();
		}
	}

	void moveDown()
	{
		sf::Time elapsed;
		elapsed = clock_move_Ver.getElapsedTime();

		if (elapsed.asMilliseconds() >= 15)
		{
			this->move(sf::Vector2f(0, BLOCK_SIZE));
			clock_move_Ver.restart();
		}
	}

	//-----------------------------------------------------------------------------------------------------

private:

	// vector with single Blocks
	std::vector<Block> piece_blocks;


	// transformation clocks (one for every type of transformation - to make for example rotation and horizontal move at once) - control of movement speed
	sf::Clock clock_move_Hor;
	sf::Clock clock_move_Ver;
	sf::Clock clock_rotate;

	// virtual method from Drawable class (drawing on window)
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();

		//draw each block
		for (int i = 0; i < piece_blocks.size(); i++)
		{
			target.draw(piece_blocks[i], states);
		}
	}


};