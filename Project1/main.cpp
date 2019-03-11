#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <iostream>
#include <vector>
#include <memory>

#include "Block.h"
#include "Piece.h"
#include "GamePieces.h"


//reaction time in ms - for player moves
constexpr int REACT_TIME{ 150 };

//speed of vertical movement, 800ms at the begining, value lower at later phase of game
int vertical_move_time = 800;

//texture for single block
sf::Texture block_texture;

//function - returns random piece (with random type, color and horizontal position)
std::unique_ptr<Piece> random_piece();

int main()
{

	sf::RenderWindow game_window(sf::VideoMode(800, 600, 32), "Tetris v.1.0", sf::Style::Titlebar | sf::Style::Close);
	sf::Event event;

	
	if (!block_texture.loadFromFile("iceblock.jpg"))	// Load before unique_ptr definition!
	{
		std::cerr << "Falied to load texture";
	}


	std::unique_ptr<Piece> current_piece = random_piece();

	
	// clocks ----------------------------------------------------------------------
	
	sf::Clock game_time;		// clock for moveDown (game)
	sf::Clock clock_move_Hor;	// clock for horizontal movement (player)
	sf::Clock clock_move_Ver;	// clock for moveDown (player)
	sf::Clock clock_rotate;		// clock for rotation (player)


	// board edges -------------------------------------------------------------------

	sf::RectangleShape right_edge(sf::Vector2f(5, 600));
	sf::RectangleShape left_edge(sf::Vector2f(20, 600));

	right_edge.setFillColor(sf::Color::Yellow);

	left_edge.setPosition(-BLOCK_SIZE - 1 , 0); // -1 - safety margin for inaccurate transformation
	right_edge.setPosition(BLOCK_SIZE *16 + 1, 0);// +1 - safety margin for inaccurate transformation
	
	sf::FloatRect right_bound = right_edge.getGlobalBounds();
	sf::FloatRect left_bound = left_edge.getGlobalBounds();

	// game loop ----------------------------------------------------------------------

	while (game_window.isOpen())
	{
		game_window.clear(sf::Color::Black);
		game_window.draw(right_edge);

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
				if (current_piece->check_piece_collision(right_bound) || current_piece->check_piece_collision(left_bound)) // check collision
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
				if (current_piece->check_piece_collision(right_bound) || current_piece->check_piece_collision(left_bound)) // check collision
				{
					current_piece->moveLeft(); // if collision - return to base state (before player move action)
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
				if (current_piece->check_piece_collision(right_bound) || current_piece->check_piece_collision(left_bound)) // check collision
				{
					current_piece->moveRight(); // if collision - return to base state (before player move action)
				}
				clock_move_Hor.restart();
			}
		}

		// vertical move - down
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			sf::Time elapsed;
			elapsed = clock_move_Ver.getElapsedTime();

			if (elapsed.asMilliseconds() >= 15)
			{
				current_piece->moveDown();
				clock_move_Ver.restart();
			}
		}


		// moveDown - by game itself ------------------------------------------------------------

		sf::Time elapsed = game_time.getElapsedTime();
		if (static_cast<int>(elapsed.asMilliseconds()) > vertical_move_time)
		{
			current_piece->moveDown();
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
	srand(time(NULL));
	int color_nr = rand() % 4;										// random piece kolor
	int pos_x = (rand() % 13) * BLOCK_SIZE + 2 * BLOCK_SIZE;		// random piece horizontal position (board size -> 320, range -> 40 - 280) - for 20x20 block size
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