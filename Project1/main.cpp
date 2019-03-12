#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <iostream>
#include <vector>
#include <memory>

#include "Block.h"
#include "Piece.h"
#include "GamePieces.h"
#include "Board.h"
#include <Windows.h>


//reaction time in ms - for player moves
constexpr int REACT_TIME{ 150 };

//speed of vertical movement, 800ms at the begining, value lower at later phase of game
int vertical_move_time = 800;

//texture for single block
sf::Texture block_texture;

//function - returns random piece (with random type, color and horizontal position)
std::unique_ptr<Piece> random_piece();
void removeConsoleWindow();
int main()
{
	removeConsoleWindow();
	srand(time(NULL)); // seed for random piece generator (random_piece function)

	sf::RenderWindow game_window(sf::VideoMode(800, 600, 32), "Tetris v.1.0", sf::Style::Titlebar | sf::Style::Close);
	sf::Event event;

	
	if (!block_texture.loadFromFile("iceblock.jpg"))	// Load before unique_ptr current_piece definition!
	{
		std::cerr << "Falied to load texture";
	}


	std::unique_ptr<Piece> current_piece = random_piece(); // first random block
	Board game_board;
	std::vector<sf::FloatRect> block_bounds; // vector for bounds of blocks on board
	
	// clocks ----------------------------------------------------------------------
	
	sf::Clock game_time;		// clock for moveDown (game)
	sf::Clock clock_move_Hor;	// clock for horizontal movement (player)
	sf::Clock clock_move_Ver;	// clock for moveDown (player)
	sf::Clock clock_rotate;		// clock for rotation (player)


	// board edges -------------------------------------------------------------------

	sf::RectangleShape right_edge(sf::Vector2f(5, 600));
	sf::RectangleShape left_edge(sf::Vector2f(20, 600));
	sf::RectangleShape bottom_edge(sf::Vector2f(320, 20));

	right_edge.setFillColor(sf::Color::Yellow);

	left_edge.setPosition(-BLOCK_SIZE - 1 , 0); // -1 - safety margin for inaccurate transformation
	right_edge.setPosition(BLOCK_SIZE * 16 + 1, 0); // +1 - safety margin for inaccurate transformation
	bottom_edge.setPosition(0, BLOCK_SIZE * 30 + 1); // +1 - safety margin for inaccurate transformation
	
	sf::FloatRect right_bound = right_edge.getGlobalBounds();
	sf::FloatRect left_bound = left_edge.getGlobalBounds();
	sf::FloatRect bottom_bound = bottom_edge.getGlobalBounds();

	// game loop ----------------------------------------------------------------------

	while (game_window.isOpen())
	{
		game_window.clear(sf::Color::Black);
		game_window.draw(right_edge);
		game_window.draw(game_board);

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

		// player actions ----------------------------------------------------------------------
		
		// piece rotation
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			sf::Time elapsed;
			elapsed = clock_rotate.getElapsedTime();

			if (elapsed.asMilliseconds() >= REACT_TIME) // only one rotation per REACT_TIME
			{
				current_piece->rotateRight();

				if (current_piece->check_piece_collision(right_bound) || current_piece->check_piece_collision(left_bound)) // check collision with side edges
				{
					// if collision - move into new position:
					if (current_piece->get_name() == "I-type")
					{
						current_piece->move(2*BLOCK_SIZE, 0);
					}
					else
					{
						current_piece->move(BLOCK_SIZE, 0);
					}

					if (current_piece->check_piece_collision(right_bound) || current_piece->check_piece_collision(left_bound)) // check collision again in new position
					{
						// if collision - move into new position:
						if (current_piece->get_name() == "I-type")
						{
							current_piece->move(-4* BLOCK_SIZE, 0);
						}
						else
						{
							current_piece->move(-2* BLOCK_SIZE, 0);
						}
						

						if (current_piece->check_piece_collision(right_bound) || current_piece->check_piece_collision(left_bound)) // check collision again in new position
						{
							// if another collision - return to base state (before player rotate action)
							if (current_piece->get_name() == "I-type")
							{
								current_piece->move(2* BLOCK_SIZE, 0);
							}
							else
							{
								current_piece->move(BLOCK_SIZE, 0);
							}
							
							current_piece->rotateLeft();
						}
					}
				}


				for (auto bound : block_bounds) // check collision with other pieces on board
				{
					if (current_piece->check_piece_collision(bound)) // check collision
					{
						current_piece->rotateLeft();	// if collision - return to base state (before player move action)
						break; // break "for" at fist collision
					}
				}

				clock_rotate.restart();
			}	
		}

		// horizontal move - right
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			sf::Time elapsed;
			elapsed = clock_move_Hor.getElapsedTime();

			if (elapsed.asMilliseconds() >= REACT_TIME) // only one move per REACT_TIME
			{
				current_piece->moveRight();

				if (current_piece->check_piece_collision(right_bound) || current_piece->check_piece_collision(left_bound)) // check collision  with side edges
				{
					current_piece->moveLeft(); // if collision - return to base state (before player move action)
				}
				

				for (auto bound : block_bounds) // check collision with other pieces on board
				{
					if (current_piece->check_piece_collision(bound)) // check collision
					{
						current_piece->moveLeft();	// if collision - return to base state (before player move action)
						break; // break "for" at fist collision
					}
				}

				clock_move_Hor.restart();
			}

			
		}

		// horizontal move - left
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			sf::Time elapsed;
			elapsed = clock_move_Hor.getElapsedTime();

			if (elapsed.asMilliseconds() >= REACT_TIME) // only one move per REACT_TIME
			{
				current_piece->moveLeft();

				if (current_piece->check_piece_collision(right_bound) || current_piece->check_piece_collision(left_bound)) // check collision with side edges
				{
					current_piece->moveRight(); // if collision - return to base state (before player move action)
				}
				

				for (auto bound : block_bounds) // check collision  with other pieces on board
				{
					if (current_piece->check_piece_collision(bound)) // check collision
					{
						current_piece->moveRight();		// if collision - return to base state (before player move action)
						break; // break "for" at fist collision
					}
				}

				clock_move_Hor.restart();
			}
		}

		// vertical move - down
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			sf::Time elapsed;
			elapsed = clock_move_Ver.getElapsedTime();

			if (elapsed.asMilliseconds() >= 25)
			{
				current_piece->moveDown(); 
				if (current_piece->check_piece_collision(bottom_bound)) // check collision with bottom bound
				{
					current_piece->moveUp();															// if collision - return to base state (before player move action)
					std::vector<sf::Vector2f> block_coords = current_piece->get_global_block_coords();	// get global coords of every block in piece
					for (auto single_block_coords : block_coords)										// push blocks into board
					{
						game_board.push_block(single_block_coords, Block(block_texture, current_piece->get_color()));
					}
					game_board.line_clear(); // clear full lines (rows)
					block_bounds = game_board.get_block_bounds(); // update block bounds (blocks on board)
					current_piece = random_piece(); // new random piece
				}

				for (auto bound : block_bounds) // check collision  with other pieces
				{
					if (current_piece->check_piece_collision(bound)) // check collision
					{
						current_piece->moveUp();															// if collision - return to base state (before player move action)
						std::vector<sf::Vector2f> block_coords = current_piece->get_global_block_coords();	// get global coords of every block in piece
						for (auto single_block_coords : block_coords)										// push blocks into board
						{
							game_board.push_block(single_block_coords, Block(block_texture, current_piece->get_color()));
						}
						game_board.line_clear(); // clear full lines (rows) 
						block_bounds = game_board.get_block_bounds();// update block bounds (blocks on board)
						current_piece = random_piece(); // new random piece
						break; // break "for" at fist collision
					}
				}
				

				clock_move_Ver.restart();
			}
		}

		// moveDown - by game itself ------------------------------------------------------------

		sf::Time elapsed = game_time.getElapsedTime();
		if (static_cast<int>(elapsed.asMilliseconds()) > vertical_move_time)
		{
			current_piece->moveDown();
			if (current_piece->check_piece_collision(bottom_bound)) // check collision
			{
				current_piece->moveUp();															// if collision - return to previous state
				std::vector<sf::Vector2f> block_coords = current_piece->get_global_block_coords();	// get global coords of every block in piece
				for (auto single_block_coords : block_coords)										// push blocks into board
				{
					game_board.push_block(single_block_coords, Block(block_texture, current_piece->get_color()));
				}
				game_board.line_clear(); // clear full lines (rows)
				block_bounds = game_board.get_block_bounds(); // update block bounds (blocks on board)
				current_piece = random_piece(); // new random piece
			}

			for (auto bound : block_bounds) // check collision  with other pieces
			{
				if (current_piece->check_piece_collision(bound)) // check collision
				{
					current_piece->moveUp();															// if collision - return to base state (before player move action)
					std::vector<sf::Vector2f> block_coords = current_piece->get_global_block_coords();	// get global coords of every block in piece
					for (auto single_block_coords : block_coords)										// push blocks into board
					{
						game_board.push_block(single_block_coords, Block(block_texture, current_piece->get_color()));
					}
					game_board.line_clear(); // clear full lines (rows)
					block_bounds = game_board.get_block_bounds(); // update block bounds (blocks on board)
					current_piece = random_piece(); // new random piece
					break; // break "for" at fist collision
				}
			}

			game_time.restart();
		}


		// --------------------------------------------------------------------------------------

		game_window.draw(*current_piece);
		game_window.display();
	}
}

// Other functions --------------------------------------------------------------------------------

//function returns pointer to random piece (with random type, color and horizontal position)
std::unique_ptr<Piece> random_piece()
{
	sf::Color colors_tab[]{ sf::Color::Blue, sf::Color::Yellow, sf::Color::Green, sf::Color::Red };
	
	int color_nr = rand() % 4;										// random piece kolor
	int pos_x = (rand() % 14) * BLOCK_SIZE + BLOCK_SIZE;			// random piece horizontal position (board size -> 320, range -> 20 - 280) - for 20x20 block size
	int piece_type = rand() % 7 + 1;								// random piece_type (T,S,Z etc.)

	

	if (piece_type == 1)
	{
		std::unique_ptr<PieceT> p1{ new PieceT(pos_x, 40, block_texture, colors_tab[color_nr]) };
		return p1;
	}
	else if (piece_type == 2)
	{
		std::unique_ptr<PieceL> p2{ new PieceL(pos_x, 40, block_texture, colors_tab[color_nr]) };
		return p2;
	}
	else if (piece_type == 3)
	{
		std::unique_ptr<PieceJ> p3{ new PieceJ(pos_x, 40, block_texture, colors_tab[color_nr]) };
		return p3;
	}
	else if (piece_type == 4)
	{
		std::unique_ptr<PieceS> p4{ new PieceS(pos_x, 40, block_texture, colors_tab[color_nr]) };
		return p4;
	}
	else if (piece_type == 5)
	{
		std::unique_ptr<PieceZ> p5{ new PieceZ(pos_x, 40, block_texture, colors_tab[color_nr]) };
		return p5;
	}
	else if (piece_type == 6)
	{
		std::unique_ptr<PieceO> p6{ new PieceO(pos_x, 40, block_texture, colors_tab[color_nr]) };
		return p6;
	}
	else if (piece_type == 7)
	{
		std::unique_ptr<PieceI> p7{ new PieceI(pos_x, 40, block_texture, colors_tab[color_nr]) };
		return p7;
	}

}

void removeConsoleWindow()
{
	ShowWindow(FindWindowA("ConsoleWindowClass", NULL), false);
	FreeConsole();
}
