#pragma once
#include "ChessPiece.h"
#include <map>
#include <tuple>
#include <algorithm>

class GameClass
{
private:
	ChessPiece* board[8][8];
	int tiles_factors[8][8];
	std::map<char, std::vector<ChessPiece*>> players_pieces;
	std::map<char, ChessPiece*> players_kings;
	std::map<int, std::map<int, std::vector<std::pair<int, int>>>> moves;
	std::vector<std::pair<int, int>>* king_moves;
	bool check;
	char current_player;
	int grabbed_piece_x;
	int grabbed_piece_y;
	bool is_moving;
	std::map<std::pair<int, int>, sf::CircleShape> move_indicators;
	std::map<char, std::vector<ChessPiece*>> players_killed_pieces;
	std::vector<std::pair<int, int>> castling_v;
private:
	std::map<int, std::map<int, std::vector<std::pair<int, int>>>> get_all_pieces_moves(bool test_moves = true);
	ChessPiece* try_move(std::pair<int, int> from, std::pair<int, int> to);
	void undo_move(std::pair<int, int> from, std::pair<int, int> to, ChessPiece* killed_piece);
	bool is_safe_move(std::pair<int, int> from, std::pair<int, int> to);
	bool is_safe_situation();
	void move_piece(std::pair<int, int> from, std::pair<int, int> to);
	std::vector<std::pair<int, int>> get_castling_moves(char player);
	int evaluate_game();
	std::pair<int, std::pair<std::pair<int, int>, std::pair<int, int>>> minimax(int alpha, int beta, int depth = 2, int is_maximiser = true);
public:
	GameClass();
	void draw_on(sf::RenderWindow& window, std::tuple<int, int, int> board_param);
	std::vector<ChessPiece*> get_player_killed_pieces(char color);
	// handle a click on the board
	void select_piece_at(int x, int y);
	std::string get_game_state();
	void update_game();
	std::string get_current_player();
	~GameClass();
};

