#pragma once

#include "Piece.h"

// Classes - classic tetris pieces - T-shape, L-shape, J-shape, S-shape, Z-Shape, O-shape and I-shape
// x,y - spawn coordinates
// texture, color - texture and color for single blocks
//--------------------------------------------------------------------------------------------------------------------------------------------------------

class PieceT : public Piece
{
public:
	PieceT(float x, float y, sf::Texture &texture, sf::Color color) : 
		Piece(4, { {0,BLOCK_SIZE},{BLOCK_SIZE,BLOCK_SIZE},{2* BLOCK_SIZE,BLOCK_SIZE},{BLOCK_SIZE,0} }, { BLOCK_SIZE,BLOCK_SIZE }, texture, color)
	{
		this->setPosition(x, y);
		name = "T-type";
	}
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------

class PieceL : public Piece
{
public:
	PieceL(float x, float y, sf::Texture &texture, sf::Color color) : 
		Piece(4, { {0,BLOCK_SIZE},{BLOCK_SIZE,BLOCK_SIZE},{2* BLOCK_SIZE,BLOCK_SIZE},{2* BLOCK_SIZE,0} }, { BLOCK_SIZE,BLOCK_SIZE }, texture, color)
	{
		this->setPosition(x, y);
		name = "L-type";
	}
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------

class PieceJ : public Piece
{
public:
	PieceJ(float x, float y, sf::Texture &texture, sf::Color color) : 
		Piece(4, { {0,BLOCK_SIZE},{BLOCK_SIZE,BLOCK_SIZE},{2* BLOCK_SIZE,BLOCK_SIZE},{0,0} }, { BLOCK_SIZE,BLOCK_SIZE }, texture, color)
	{
		this->setPosition(x, y);
		name = "J-type";
	}
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------

class PieceS : public Piece
{
public:
	PieceS(float x, float y, sf::Texture &texture, sf::Color color) : 
		Piece(4, { {0,BLOCK_SIZE},{BLOCK_SIZE,BLOCK_SIZE},{2* BLOCK_SIZE,0},{BLOCK_SIZE,0} }, { BLOCK_SIZE,BLOCK_SIZE }, texture, color)
	{
		this->setPosition(x, y);
		name = "S-type";
	}
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------

class PieceZ : public Piece
{
public:
	PieceZ(float x, float y, sf::Texture &texture, sf::Color color) : 
		Piece(4, { {0,0},{BLOCK_SIZE,BLOCK_SIZE},{2* BLOCK_SIZE,BLOCK_SIZE},{BLOCK_SIZE,0} }, { BLOCK_SIZE,BLOCK_SIZE }, texture, color)
	{
		this->setPosition(x, y);
		name = "Z-type";
	}
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------

class PieceO : public Piece
{
public:
	PieceO(float x, float y, sf::Texture &texture, sf::Color color) : 
		Piece(4, { {0,0},{BLOCK_SIZE,BLOCK_SIZE},{0,BLOCK_SIZE},{BLOCK_SIZE,0} }, { BLOCK_SIZE,BLOCK_SIZE }, texture, color)
	{
		this->setPosition(x, y);
		name = "O-type";
	}
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------

class PieceI : public Piece
{
public:
	PieceI(float x, float y, sf::Texture &texture, sf::Color color) : 
		Piece(4, { {0,0},{0,BLOCK_SIZE},{0,2* BLOCK_SIZE},{0,3* BLOCK_SIZE} }, { 0,2* BLOCK_SIZE }, texture, color)
	{
		this->setPosition(x, y);
		name = "I-type";
	}
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------