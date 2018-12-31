#include "pch.h"
#include "ChessPiece.h"


ChessPiece::ChessPiece(int x, int y, ChessPiece::PieceType type, char color)
{
	sf::Sprite();
	this->type = type;
	this->color = color;
	this->killed = false;
	this->nb_moves = 0;
	pawn_advance_move = false;
	this->x = x;
	this->y = y;
	std::string texture_path = std::string() + "Sprites/" + (color == 'B' ? "white" : "black");
	switch (type) {
	case King:
		texture_path += "King.png";
		break;
	case Queen:
		texture_path += "Queen.png";
		break;
	case Rook:
		texture_path += "Rook.png";
		break;
	case Bishop:
		texture_path += "Bishop.png";
		break;
	case Knight:
		texture_path += "Knight.png";
		break;
	case Pawn:
		texture_path += "Pawn.png";
		break;
	}
	if (!texture.loadFromFile(texture_path))std::cerr << "error loading sprite texture";
	this->setTexture(texture);
}

std::vector<std::pair<int, int>> ChessPiece::get_moves(ChessPiece* board[8][8]) {
	std::vector<std::pair<int, int>> res;
	char piece_color = this->color;
	char op_color = piece_color == 'W' ? 'B' : 'W';
	int i;
	switch (this->type) {
	case ChessPiece::King:
		for (int i = x - 1; i < x + 2; ++i) {
			if (i < 0 || i > 7) continue;
			for (int j = y - 1; j < y + 2; ++j) {
				if (j < 0 || j > 7) continue;
				if (board[i][j] == nullptr || board[i][j]->color == op_color) {
					res.push_back(std::make_pair(i, j));
				}
			}
		}
		break;
	case ChessPiece::Queen:
		i = y - 1;
		while (i > 0 && board[x][i] == nullptr) {
			res.push_back(std::make_pair(x, i));
			--i;
		}
		if (i >= 0 && (board[x][i] == nullptr || board[x][i]->color == op_color)) res.push_back(std::make_pair(x, i));
		//
		i = y + 1;
		while (i < 7 && board[x][i] == nullptr) {
			res.push_back(std::make_pair(x, i));
			++i;
		}
		if (i <= 7 && (board[x][i] == nullptr || board[x][i]->color == op_color)) res.push_back(std::make_pair(x, i));
		//
		i = x - 1;
		while (i > 0 && board[i][y] == nullptr) {
			res.push_back(std::make_pair(i, y));
			--i;
		}
		if (i >= 0 && (board[i][y] == nullptr || board[i][y]->color == op_color)) res.push_back(std::make_pair(i, y));
		//
		i = x + 1;
		while (i < 7 && board[i][y] == nullptr) {
			res.push_back(std::make_pair(i, y));
			++i;
		}
		if (i <= 7 && (board[i][y] == nullptr || board[i][y]->color == op_color)) res.push_back(std::make_pair(i, y));
		i = 1;
		while ((x - i) > 0 && (y - i) > 0 && board[x - i][y - i] == nullptr) {
			res.push_back(std::make_pair(x - i, y - i));
			++i;
		}
		if ((x - i) >= 0 && (y - i) >= 0 && (board[x - i][y - i] == nullptr || board[x - i][y - i]->color == op_color))
			res.push_back(std::make_pair(x - i, y - i));
		// 
		i = 1;
		while ((x + i) < 7 && (y + i) < 7 && board[x + i][y + i] == nullptr) {
			res.push_back(std::make_pair(x + i, y + i));
			++i;
		}
		if ((x + i) <= 7 && (y + i) <= 7 && (board[x + i][y + i] == nullptr || board[x + i][y + i]->color == op_color))
			res.push_back(std::make_pair(x + i, y + i));
		//
		i = 1;
		while ((x - i) > 0 && (y + i) < 7 && board[x - i][y + i] == nullptr) {
			res.push_back(std::make_pair(x - i, y + i));
			++i;
		}
		if ((x - i) >= 0 && (y + i) <= 7 && (board[x - i][y + i] == nullptr || board[x - i][y + i]->color == op_color))
			res.push_back(std::make_pair(x - i, y + i));
		//
		i = 1;
		while ((x + i) < 7 && (y - i) > 0 && board[x + i][y - i] == nullptr) {
			res.push_back(std::make_pair(x + i, y - i));
			++i;
		}
		if ((x + i) <= 7 && (y - i) >= 0 && (board[x + i][y - i] == nullptr || board[x + i][y - i]->color == op_color))
			res.push_back(std::make_pair(x + i, y - i));
		break;

	case ChessPiece::Rook:
		i = y - 1;
		while (i > 0 && board[x][i] == nullptr) {
			res.push_back(std::make_pair(x, i));
			--i;
		}
		if (i >= 0 && (board[x][i] == nullptr || board[x][i]->color == op_color)) res.push_back(std::make_pair(x, i));
		//
		i = y + 1;
		while (i < 7 && board[x][i] == nullptr) {
			res.push_back(std::make_pair(x, i));
			++i;
		}
		if (i <= 7 && (board[x][i] == nullptr || board[x][i]->color == op_color)) res.push_back(std::make_pair(x, i));
		//
		i = x - 1;
		while (i > 0 && board[i][y] == nullptr) {
			res.push_back(std::make_pair(i, y));
			--i;
		}
		if (i >= 0 && (board[i][y] == nullptr || board[i][y]->color == op_color)) res.push_back(std::make_pair(i, y));
		//
		i = x + 1;
		while (i < 7 && board[i][y] == nullptr) {
			res.push_back(std::make_pair(i, y));
			++i;
		}
		if (i <= 7 && (board[i][y] == nullptr || board[i][y]->color == op_color)) res.push_back(std::make_pair(i, y));
		break;
	case ChessPiece::Bishop:
		i = 1;
		while ((x - i) > 0 && (y - i) > 0 && board[x - i][y - i] == nullptr) {
			res.push_back(std::make_pair(x - i, y - i));
			++i;
		}
		if ((x - i) >= 0 && (y - i) >= 0 && (board[x - i][y - i] == nullptr || board[x - i][y - i]->color == op_color))
			res.push_back(std::make_pair(x - i, y - i));
		// 
		i = 1;
		while ((x + i) < 7 && (y + i) < 7 && board[x + i][y + i] == nullptr) {
			res.push_back(std::make_pair(x + i, y + i));
			++i;
		}
		if ((x + i) <= 7 && (y + i) <= 7 && (board[x + i][y + i] == nullptr || board[x + i][y + i]->color == op_color))
			res.push_back(std::make_pair(x + i, y + i));
		//
		i = 1;
		while ((x - i) > 0 && (y + i) < 7 && board[x - i][y + i] == nullptr) {
			res.push_back(std::make_pair(x - i, y + i));
			++i;
		}
		if ((x - i) >= 0 && (y + i) <= 7 && (board[x - i][y + i] == nullptr || board[x - i][y + i]->color == op_color))
			res.push_back(std::make_pair(x - i, y + i));
		//
		i = 1;
		while ((x + i) < 7 && (y - i) > 0 && board[x + i][y - i] == nullptr) {
			res.push_back(std::make_pair(x + i, y - i));
			++i;
		}
		if ((x + i) <= 7 && (y - i) >= 0 && (board[x + i][y - i] == nullptr || board[x + i][y - i]->color == op_color))
			res.push_back(std::make_pair(x + i, y - i));
		break;
	case ChessPiece::Knight:
		if (x < 7 && y < 6 && (board[x + 1][y + 2] == nullptr || board[x + 1][y + 2]->color == op_color)) res.push_back(std::make_pair(x + 1, y + 2));
		if (x < 7 && y > 1 && (board[x + 1][y - 2] == nullptr || board[x + 1][y - 2]->color == op_color)) res.push_back(std::make_pair(x + 1, y - 2));
		if (x > 0 && y < 6 && (board[x - 1][y + 2] == nullptr || board[x - 1][y + 2]->color == op_color)) res.push_back(std::make_pair(x - 1, y + 2));
		if (x > 0 && y > 1 && (board[x - 1][y - 2] == nullptr || board[x - 1][y - 2]->color == op_color)) res.push_back(std::make_pair(x - 1, y - 2));
		if (x < 6 && y < 7 && (board[x + 2][y + 1] == nullptr || board[x + 2][y + 1]->color == op_color)) res.push_back(std::make_pair(x + 2, y + 1));
		if (x < 6 && y > 0 && (board[x + 2][y - 1] == nullptr || board[x + 2][y - 1]->color == op_color)) res.push_back(std::make_pair(x + 2, y - 1));
		if (x > 1 && y < 7 && (board[x - 2][y + 1] == nullptr || board[x - 2][y + 1]->color == op_color)) res.push_back(std::make_pair(x - 2, y + 1));
		if (x > 1 && y > 0 && (board[x - 2][y - 1] == nullptr || board[x - 2][y - 1]->color == op_color)) res.push_back(std::make_pair(x - 2, y - 1));
		break;
	case ChessPiece::Pawn:
		if (piece_color == 'W') {
			if (y < 7 && (board[x][y + 1] == nullptr)) {
				res.push_back(std::make_pair(x, y + 1));
				if (nb_moves == 0 && y < 6 && board[x][y + 2] == nullptr) {
					res.push_back(std::make_pair(x, y + 2));
				}
			}
			if (x < 7 && y < 7 && (board[x + 1][y + 1] != nullptr && board[x + 1][y + 1]->color == 'B')) {
				res.push_back(std::make_pair(x + 1, y + 1));
			}
			if (x > 0 && y < 7 && (board[x - 1][y + 1] != nullptr && board[x - 1][y + 1]->color == 'B')) {
				res.push_back(std::make_pair(x - 1, y + 1));
			}
		}
		else {
			if (y > 0 && (board[x][y - 1] == nullptr)) {
				res.push_back(std::make_pair(x, y - 1));
				if (nb_moves == 0 && y > 1 && board[x][y - 2] == nullptr) {
					res.push_back(std::make_pair(x, y - 2));
				}
			}
			if (x < 7 && y > 0 && (board[x + 1][y - 1] != nullptr && board[x + 1][y - 1]->color == 'W')) {
				res.push_back(std::make_pair(x + 1, y - 1));
			}
			if (x > 0 && y > 0 && (board[x - 1][y - 1] != nullptr && board[x - 1][y - 1]->color == 'W')) {
				res.push_back(std::make_pair(x - 1, y - 1));
			}
		}
		break;
	}
	return res;
}

void ChessPiece::promote_to(PieceType type) {
	this->type = type;
	std::string texture_path = std::string() + "Sprites/" + (color == 'W' ? "white" : "black");
	switch (type) {
	case King:
		texture_path += "King.png";
		break;
	case Queen:
		texture_path += "Queen.png";
		break;
	case Rook:
		texture_path += "Rook.png";
		break;
	case Bishop:
		texture_path += "Bishop.png";
		break;
	case Knight:
		texture_path += "Knight.png";
		break;
	case Pawn:
		texture_path += "Pawn.png";
		break;
	}
	if (!texture.loadFromFile(texture_path))std::cerr << "error loading sprite texture";
	this->setTexture(texture);
}

void ChessPiece::move_to(int x, int y) {
	if (this->type == Pawn && nb_moves == 0 && abs(this->y - y) == 2) pawn_advance_move = true;
	this->x = x;
	this->y = y;
	++this->nb_moves;
}

void ChessPiece::undo_move(int x, int y) {
	--this->nb_moves;
	if (this->type == Pawn && this->nb_moves == 0) pawn_advance_move = false;
	this->x = x;
	this->y = y;
}

void ChessPiece::kill_piece() {
	this->killed = true;
	std::cerr << "piece at " << x << " " << y << " killed" << std::endl;
	this->x = -1;
	this->y = -1;
}

std::string ChessPiece::to_string() {
	switch (type) {
	case King: return "King";
	case Queen: return "Queen";
	case Rook: return "Rook";
	case Bishop: return "Bishop";
	case Knight: return "Knight";
	case Pawn: return "Pawn";
	}
	return "";
}

int ChessPiece::get_value() {
	if (this->killed) return 0;
	switch (type) {
	case King: return 20000 + ChessPiece::get_position_factor(this->type, x, y);
	case Queen: return 900 + ChessPiece::get_position_factor(this->type, x, y);;
	case Rook: return 500 + ChessPiece::get_position_factor(this->type, x, y);;
	case Bishop: return 330 + ChessPiece::get_position_factor(this->type, x, y);;
	case Knight: return 320 + ChessPiece::get_position_factor(this->type, x, y);;
	case Pawn: return 100 + ChessPiece::get_position_factor(this->type, x, y);;
	}
	return 0;
}

int ChessPiece::get_position_factor(PieceType type, int x, int y) {
	int pawn_factor[8][8] = {
		{ 0,  0,  0,  0,  0,  0,  0,  0},
		{50, 50, 50, 50, 50, 50, 50, 50},
		{10, 10, 20, 30, 30, 20, 10, 10},
		{ 5,  5, 10, 25, 25, 10,  5,  5},
		{ 0,  0,  0, 20, 20,  0,  0,  0},
		{ 5, -5,-10,  0,  0,-10, -5,  5},
		{ 5, 10, 10,-20,-20, 10, 10,  5},
		{ 0,  0,  0,  0,  0,  0,  0,  0}
	};
	int bishop_factor[8][8] = {
		{-20,-10,-10,-10,-10,-10,-10,-20},
		{-10,  0,  0,  0,  0,  0,  0,-10},
		{-10,  0,  5, 10, 10,  5,  0,-10},
		{-10,  5,  5, 10, 10,  5,  5,-10},
		{-10,  0, 10, 10, 10, 10,  0,-10},
		{-10, 10, 10, 10, 10, 10, 10,-10},
		{-10,  5,  0,  0,  0,  0,  5,-10},
		{-20,-10,-10,-10,-10,-10,-10,-20}
	};
	int knight_factor[8][8] = {
		{-50,-40,-30,-30,-30,-30,-40,-50},
		{-40,-20,  0,  0,  0,  0,-20,-40},
		{-30,  0, 10, 15, 15, 10,  0,-30},
		{-30,  5, 15, 20, 20, 15,  5,-30},
		{-30,  0, 15, 20, 20, 15,  0,-30},
		{-30,  5, 10, 15, 15, 10,  5,-30},
		{-40,-20,  0,  5,  5,  0,-20,-40},
		{-50,-40,-30,-30,-30,-30,-40,-50}
	};

	int rook_factor[8][8] = {
		{ 0,  0,  0,  0,  0,  0,  0,  0},
		{ 5, 10, 10, 10, 10, 10, 10,  5},
		{-5,  0,  0,  0,  0,  0,  0, -5},
		{-5,  0,  0,  0,  0,  0,  0, -5},
		{-5,  0,  0,  0,  0,  0,  0, -5},
		{-5,  0,  0,  0,  0,  0,  0, -5},
		{-5,  0,  0,  0,  0,  0,  0, -5},
		{ 0,  0,  0,  0,  0,  0,  0,  0}
	};

	int queen_factor[8][8] = {
		{-20,-10,-10, -5, -5,-10,-10,-20},
		{-10,  0,  0,  0,  0,  0,  0,-10},
		{-10,  0,  5,  5,  5,  5,  0,-10},
		{ -5,  0,  5,  5,  5,  5,  0, -5},
		{  0,  0,  5,  5,  5,  5,  0, -5},
		{-10,  5,  5,  5,  5,  5,  0,-10},
		{-10,  0,  5,  0,  0,  0,  0,-10},
		{-20,-10,-10, -5, -5,-10,-10,-20}
	};
	int king_factor[8][8] = {
		{-30,-40,-40,-50,-50,-40,-40,-30},
		{-30,-40,-40,-50,-50,-40,-40,-30},
		{-30,-40,-40,-50,-50,-40,-40,-30,},
		{-30,-40,-40,-50,-50,-40,-40,-30},
		{-20,-30,-30,-40,-40,-30,-30,-20},
		{-10,-20,-20,-20,-20,-20,-20,-10},
		{ 20, 20,  0,  0,  0,  0, 20, 20},
		{ 20, 30, 10,  0,  0, 10, 30, 20}
	};

	switch (type) {
	case King: return king_factor[y][x];
	case Queen: return queen_factor[y][x];
	case Rook: return rook_factor[y][x];
	case Bishop: return bishop_factor[y][x];
	case Knight: return knight_factor[y][x];
	case Pawn: return pawn_factor[y][x];
	}
	return 0;
};

ChessPiece::~ChessPiece()
{
}
