#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <vector>
#include <map>

class ChessPiece : public sf::Sprite
{
public:
	enum PieceType { King, Queen, Rook, Bishop, Knight, Pawn };
	PieceType type;
	int nb_moves;
	bool pawn_advance_move;
	bool killed;
	char color;
	int x;
	int y;
private:
	sf::Texture texture;
public:
	ChessPiece(int x, int y, PieceType type, char color);
	std::vector<std::pair<int, int>> get_moves(ChessPiece* board[8][8]);
	void undo_move(int x, int y);
	void move_to(int x, int y);
	void promote_to(PieceType type);
	void kill_piece();
	std::string to_string();
	int get_value();
	~ChessPiece();
};

