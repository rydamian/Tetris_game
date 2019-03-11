#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Block.h"
#include <iostream>


//class for complex pieces (piece made from blocks)
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
		name = "Unknown name";
	}

	//-----------------------------------------------------------------------------------------------------
	//// set new origin point without change of position on screen - currently not in use so in comments

	//void set_new_origin(sf::Vector2f center_point) 
	//{
	//	//sf::Transform transf = this->getTransform();
	//	//sf::Vector2f original_origin = transf.transformPoint(this->getOrigin());	// transform current origin point of piece into global (window) coordinates
	//	//sf::Vector2f center_global = transf.transformPoint(center_point);			// transform new origin point of piece into global (window) coordinates

	//	this->setOrigin(center_point);

	//			//Console check:
	//			//center_point = transf.transformPoint(this->getOrigin());

	//			//sf::Vector2f m = (center_point - original_origin);
	//			//std::cout << "move " << m.x << " " << m.y << std::endl;

	//	//this->move(center_global - original_origin);
	//}


	//-----------------------------------------------------------------------------------------------------
	// collision check with bound in global coordinates
	bool check_piece_collision(sf::FloatRect bound)
	{
		for (auto block : piece_blocks)  // check collision with every single block
		{
			sf::Transform transf = this->getTransform();
			sf::Vector2f glob_coords = transf.transformPoint(block.getPosition()); //transform origin point of block into global (window) coordinates

			sf::FloatRect block_bounds = block.getGB(glob_coords,this->getRotation());
			
			if (block_bounds.intersects(bound)) return true;
		}

		return false;
	}

	//-----------------------------------------------------------------------------------------------------
	//Transformation methods

	void rotateRight()
	{
		this->rotate(90);
	}

	void rotateLeft()
	{
		this->rotate(-90);
	}

	void moveLeft()
	{
		this->move(sf::Vector2f(-BLOCK_SIZE, 0));
	}

	void moveRight()
	{
		this->move(sf::Vector2f(BLOCK_SIZE, 0));
	}

	void moveDown()
	{
		this->move(sf::Vector2f(0, BLOCK_SIZE));
	}

	//-----------------------------------------------------------------------------------------------------

	std::string get_name()
	{
		return name;
	}

protected:
	std::string name;

private:
	// vector with single Blocks
	std::vector<Block> piece_blocks;

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