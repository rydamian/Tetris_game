#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>


constexpr int BLOCK_SIZE{ 20 };

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

private:

	sf::RectangleShape single_block;

	// virtual method from Drawable class (drawing on window)
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		target.draw(single_block, states);
	}
};