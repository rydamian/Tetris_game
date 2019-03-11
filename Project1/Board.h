#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Block.h"

#include <vector>
#include <memory>
#include <iostream>

class Board : public sf::Drawable
{
public:
	// create board - board_rows x board_collumns size
	Board()
	{
		for (int i = 0; i < board_rows; i++)		
		{
			game_board.push_back(std::vector< std::unique_ptr<Block> >());
			for (int j = 0; j < board_columns; j++)	
			{
				game_board[i].push_back(std::unique_ptr<Block>{ nullptr });
			}
		}
	}

	// -----------------------------------------------------------------------------------------------------
	// Push block into board - coords - global coordinates of block in pix
	void push_block(sf::Vector2f coords, Block block)
	{
		block.setPosition(coords.x, coords.y);
		int row = coords.y / BLOCK_SIZE;
		int column = coords.x / BLOCK_SIZE;
		game_board[row][column] = std::unique_ptr<Block>{ new Block{ block } };
	}


private:
	std::vector< std::vector< std::unique_ptr<Block> > > game_board;
	int board_rows = 30;			// 30 rows		(30 * BLOCK_SIZE = 30 * 20 = 600 - window heigth in pix)
	int board_columns = 16;			// 16 columns	(16 * BLOCK_SIZE = 16 * 20 = 320 - board witdth in pix)

	// virtual method from Drawable class (drawing on window)
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		//draw each block
		for (int i = 0; i < board_rows; i++)
		{
			for (int j = 0; j < board_columns; j++)
			{
				if (game_board[i][j])
				{
					target.draw(*game_board[i][j], states);
				}
			}
		}
	}

};

