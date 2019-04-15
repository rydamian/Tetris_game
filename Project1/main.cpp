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

//window size
constexpr int WINDOW_HEIGTH = 600;
constexpr int WINDOW_WIDTH = 600;

//reaction time in ms - for player moves
constexpr int REACT_TIME{ 150 };

//how many point for one line clear
constexpr int POINTS_FOR_CLEAR{ 100 };

//speed of vertical movement, 825ms at the begining, value lower at later phase of game (-50 ms per 30 s)
int vertical_move_time = 825;
constexpr int VERTICAL_MOVE_DIFF = 50;
constexpr int SPEEDING_TIME = 30;

//texture for single block
sf::Texture block_texture;

//function - returns random piece (with random type, color and horizontal position)
std::unique_ptr<Piece> random_piece();

//another functions
void removeConsoleWindow();
void update_score(int lines_cleared, int & points, sf::Text &scoring);

int main()
{
	removeConsoleWindow();
	srand(time(NULL)); // seed for random piece generator (random_piece function)

	sf::RenderWindow game_window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGTH, 32), "Tetris v.1.0", sf::Style::Titlebar | sf::Style::Close);
	sf::Event event;
	int points = 0; // game score
	
	if (!block_texture.loadFromFile("iceblock.jpg"))	// Load before unique_ptr current_piece definition!
	{
		std::cerr << "Falied to load texture";
	}

	std::unique_ptr<Piece> current_piece = random_piece(); // first random block
	std::unique_ptr<Piece> next_piece = random_piece(); // second random block
	std::unique_ptr<Piece> next_piece_picture_obj{ new Piece{*next_piece} }; // copy of next piece (for displaying on right side)
	(*next_piece_picture_obj).setPosition(sf::Vector2f(420,290)); // set position on right side of screen

	Board game_board;
	std::vector<sf::FloatRect> block_bounds; // vector for bounds of blocks on board
	
	// clocks ----------------------------------------------------------------------
	
	sf::Clock game_time;		// clock for moveDown (game)
	sf::Clock game_speed;		// clock for vertical movement speed (game)
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

	// Right side of window - scoring and next block -----------------------------------------------------

	sf::Font font;
	if (!font.loadFromFile("neuropol.ttf"))
	{
		std::cerr << "Falied to load font";
	}

	sf::Text scoring; // number of points
	scoring.setFont(font);
	scoring.setString(std::to_string(points));
	scoring.setCharacterSize(24);
	scoring.setFillColor(sf::Color::Red);
	scoring.setPosition(350, 75);

	sf::Text score_text; // "Score:"
	score_text = scoring;
	score_text.setString("Score:");
	score_text.setPosition(350, 25);

	sf::Text next_text; // "Next block:"
	next_text = scoring;
	next_text.setString("Next block:");
	next_text.setPosition(350, 190);

	// game loop ----------------------------------------------------------------------

	while (game_window.isOpen())
	{

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

		if (!game_board.is_game_over()) // if game is not over
		{
			game_window.clear(sf::Color::Black);
			game_window.draw(right_edge);
			game_window.draw(game_board);
			game_window.draw(scoring);
			game_window.draw(score_text);
			game_window.draw(next_text);
			game_window.draw(*current_piece);
			game_window.draw(*next_piece_picture_obj);


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
							current_piece->move(2 * BLOCK_SIZE, 0);
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
								current_piece->move(-4 * BLOCK_SIZE, 0);
							}
							else
							{
								current_piece->move(-2 * BLOCK_SIZE, 0);
							}


							if (current_piece->check_piece_collision(right_bound) || current_piece->check_piece_collision(left_bound)) // check collision again in new position
							{
								// if another collision - return to base state (before player rotate action)
								if (current_piece->get_name() == "I-type")
								{
									current_piece->move(2 * BLOCK_SIZE, 0);
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
							break; // break "for" at first collision
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

			// vertical move - down (player)
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
						int lines_cleared = game_board.line_clear(); // clear full lines (rows)
						if (lines_cleared) // add points (game score)
						{
							update_score(lines_cleared, points, scoring);
						}
						block_bounds = game_board.get_block_bounds(); // update block bounds (blocks on board)
						current_piece = std::move(next_piece); // change next_piece to current_piece
						next_piece = random_piece(); // new random piece
						next_piece_picture_obj = std::unique_ptr<Piece>{ new Piece{*next_piece} }; // copy of next piece (for displaying on right side)
						(*next_piece_picture_obj).setPosition(sf::Vector2f(420, 290)); // set position on right side of screen
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
							int lines_cleared = game_board.line_clear(); // clear full lines (rows)
							if (lines_cleared) // add points (game score)
							{
								update_score(lines_cleared, points, scoring);
							}
							block_bounds = game_board.get_block_bounds();// update block bounds (blocks on board)
							current_piece = std::move(next_piece); // change next_piece to current_piece
							next_piece = random_piece(); // new random piece
							next_piece_picture_obj = std::unique_ptr<Piece>{ new Piece{*next_piece} }; // copy of next piece (for displaying on right side)
							(*next_piece_picture_obj).setPosition(sf::Vector2f(420, 290)); // set position on right side of screen
							break; // break "for" at fist collision
						}
					}

					clock_move_Ver.restart();
				}
			}

			// vertical move - down (game)

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
					int lines_cleared = game_board.line_clear(); // clear full lines (rows)
					if (lines_cleared) // add points (game score)
					{
						update_score(lines_cleared, points, scoring);
					}
					block_bounds = game_board.get_block_bounds(); // update block bounds (blocks on board)
					current_piece = std::move(next_piece); // change next_piece to current_piece
					next_piece = random_piece(); // new random piece
					next_piece_picture_obj = std::unique_ptr<Piece>{ new Piece{*next_piece} }; // copy of next piece (for displaying on right side)
					(*next_piece_picture_obj).setPosition(sf::Vector2f(420, 290)); // set position on right side of screen
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
						int lines_cleared = game_board.line_clear(); // clear full lines (rows)
						if (lines_cleared) // add points (game score)
						{
							update_score(lines_cleared, points, scoring);
						}
						block_bounds = game_board.get_block_bounds(); // update block bounds (blocks on board)
						current_piece = std::move(next_piece); // change next_piece to current_piece
						next_piece = random_piece(); // new random piece
						next_piece_picture_obj = std::unique_ptr<Piece>{ new Piece{*next_piece} }; // copy of next piece (for displaying on right side)
						(*next_piece_picture_obj).setPosition(sf::Vector2f(420, 290)); // set position on right side of screen
						break; // break "for" at fist collision
					}
				}
				game_time.restart();
			}

			//speed up vertical move -----------------------------------------------------------------------------
			elapsed = game_speed.getElapsedTime();
			if (static_cast<int>(elapsed.asSeconds()) > SPEEDING_TIME)
			{
				vertical_move_time -= VERTICAL_MOVE_DIFF;
				game_speed.restart();
			}


			// --------------------------------------------------------------------------------------
		}
		else // if game is over
		{
			game_window.clear(sf::Color::Black);

			sf::Text game_over;
			game_over.setFont(font);
			game_over.setString("GAME OVER");
			game_over.setCharacterSize(50);
			game_over.setFillColor(sf::Color::Red);
			game_over.setPosition(80, 75);

			score_text.setPosition(230, 155);	// show final score
			scoring.setPosition(230, 180);		// show final score

			sf::RectangleShape background(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGTH));

			background.setFillColor(sf::Color(0,0,0,190)); // last number - background transparency

			game_window.draw(game_board);
			game_window.draw(background);
			game_window.draw(game_over);
			game_window.draw(scoring);
			game_window.draw(score_text);

		}

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

// --------------------------------------------------------------------------------

void removeConsoleWindow()
{
	ShowWindow(FindWindowA("ConsoleWindowClass", NULL), false);
	FreeConsole();
}

// --------------------------------------------------------------------------------

void update_score(int lines_cleared, int & points, sf::Text &scoring)
{
	points += lines_cleared * POINTS_FOR_CLEAR;
	scoring.setString(std::to_string(points));

}
