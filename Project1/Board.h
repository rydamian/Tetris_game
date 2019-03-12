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


	// -----------------------------------------------------------------------------------------------------
	// Returns vector of bounds (bounds of every block on board)
	std::vector<sf::FloatRect> get_block_bounds()
	{
		std::vector<sf::FloatRect> bounds;

		for (int i = 0; i < board_rows; i++)
		{
			for (int j = 0; j < board_columns; j++)
			{
				if (game_board[i][j])
				{
					sf::FloatRect block_bound;
					block_bound = (*game_board[i][j]).getGB(((*game_board[i][j])).getPosition(), 0);
					bounds.push_back(block_bound);
				}
			}
		}

		return bounds;
	}

	// -----------------------------------------------------------------------------------------------------
	// Clear line (row) - when full of blocks
	void line_clear()
	{
		for (int i = board_rows - 1; i >= 0 ; i--) // check from bottom to top - every row
		{
			int row_count = 0;
			for (int j = 0; j < board_columns; j++) // check every column in row
			{
				if (game_board[i][j])
				{
					row_count++;
				}
			}

			if (row_count == 16) // if row is full - clear row
			{
				for (int k = i; k >= 1; k--) // from bottom to top  - every row
				{
					for (int m = 0; m < board_columns; m++) // every column in row
					{
						game_board[k][m] = std::move(game_board[k - 1][m]); // move line one row down (std::move - because of unique_ptr)
						if (game_board[k][m])
						{
							(*game_board[k][m]).move(0, BLOCK_SIZE); //change single block coordinates
						}

					}
				}
				i++; //check row again (this was one row up before!!)
			}
		}
	}

	// -----------------------------------------------------------------------------------------------------

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

