#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>


constexpr float BLOCK_SIZE{ 20 };

//20x20 (BLOCK_SIZE x BLOCK_SIZE) pix single block:
class Block : public sf::Transformable, public sf::Drawable
{
public:

	// create single block with specific texture and color
	Block(sf::Texture &texture, sf::Color color) : single_block(sf::Vector2f(BLOCK_SIZE, BLOCK_SIZE))
	{
		single_block.setTexture(&texture);
		single_block.setTextureRect(sf::IntRect(0, 0, texture.getSize().x, texture.getSize().y));
		single_block.setFillColor(color);
	}

	//-----------------------------------------------------------------------------------------------------
	// returns global bounds (for collisions) in global coordinates (glob_coords - origin point in global coordinates, rot - rotation)
	sf::FloatRect getGB(sf::Vector2f glob_coords, float rot)
	{
		sf::RectangleShape single_block_glob = single_block;
		single_block_glob.setPosition(glob_coords);
		single_block_glob.setRotation(rot);

		return single_block_glob.getGlobalBounds();
	}

	//-----------------------------------------------------------------------------------------------------

private:

	sf::RectangleShape single_block;

	// virtual method from Drawable class (drawing on window)
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		target.draw(single_block, states);
	}
};