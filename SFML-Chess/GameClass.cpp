#include "pch.h"
#include "GameClass.h"

int nb_leafs = 0;

GameClass::GameClass()
{
	current_player = 'B';
	is_moving = false;
	grabbed_piece_x = -1;
	grabbed_piece_y = -1;
	check = false;
	for (int i = 0; i < 64; ++i) {
		this->board[i % 8][i / 8] = nullptr;
	}
	for (int i = 0; i < 8; ++i) {
		this->board[i][1] = new ChessPiece(i, 1, ChessPiece::Pawn, 'W');
		this->board[i][6] = new ChessPiece(i, 6, ChessPiece::Pawn, 'B');
	}
	this->board[0][7] = new ChessPiece(0, 7, ChessPiece::Rook, 'B');
	this->board[7][7] = new ChessPiece(7, 7, ChessPiece::Rook, 'B');
	this->board[1][7] = new ChessPiece(1, 7, ChessPiece::Knight, 'B');
	this->board[6][7] = new ChessPiece(6, 7, ChessPiece::Knight, 'B');
	this->board[2][7] = new ChessPiece(2, 7, ChessPiece::Bishop, 'B');
	this->board[5][7] = new ChessPiece(5, 7, ChessPiece::Bishop, 'B');
	this->board[3][7] = new ChessPiece(3, 7, ChessPiece::Queen, 'B');
	this->board[4][7] = new ChessPiece(4, 7, ChessPiece::King, 'B');

	this->board[0][0] = new ChessPiece(0, 0, ChessPiece::Rook, 'W');
	this->board[7][0] = new ChessPiece(7, 0, ChessPiece::Rook, 'W');
	this->board[1][0] = new ChessPiece(1, 0, ChessPiece::Knight, 'W');
	this->board[6][0] = new ChessPiece(6, 0, ChessPiece::Knight, 'W');
	this->board[2][0] = new ChessPiece(2, 0, ChessPiece::Bishop, 'W');
	this->board[5][0] = new ChessPiece(5, 0, ChessPiece::Bishop, 'W');
	this->board[3][0] = new ChessPiece(3, 0, ChessPiece::Queen, 'W');
	this->board[4][0] = new ChessPiece(4, 0, ChessPiece::King, 'W');

	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			if (this->board[i][j] == nullptr) continue;
			this->players_pieces[this->board[i][j]->color].push_back(this->board[i][j]);
			if (this->board[i][j]->type == ChessPiece::King) {
				this->players_kings[this->board[i][j]->color] = this->board[i][j];
			}
		}
	}
	for (int i = 0; i < 64; ++i) this->tiles_factors[i / 8][i % 8] = 5;
	for (int i = 1; i < 4; ++i) {
		for (int a = i; a < 8 - i; ++a) {
			for (int b = i; b < 8 - i; ++b) {
				this->tiles_factors[a][b]++;
			}
		}
	}
}

std::pair<int, std::pair<std::pair<int, int>, std::pair<int, int>>> GameClass::minimax(int alpha, int beta, int depth, int is_maximiser) {
	if (depth == 0) {
		//std::cout << "leaf reached" << std::endl;
		nb_leafs++;
		return std::make_pair(evaluate_game(), std::make_pair(std::make_pair(-1, -1), std::make_pair(-1, -1)));
	}
	std::pair<std::pair<int, int>, std::pair<int, int>> best_move(std::make_pair(-1, -1), std::make_pair(-1, -1));
	std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> possible_moves;
	for (ChessPiece* p : players_pieces[current_player]) {
		if (p->killed) continue;
		std::pair<int, int> from(p->x, p->y);
		for (std::pair<int, int> to : this->moves[p->x][p->y]) possible_moves.push_back(std::make_pair(from, to));
	}
	if (is_maximiser) {
		int best_score = -10000;
		for (std::pair<std::pair<int, int>, std::pair<int, int>> p : possible_moves) {
			std::pair<int, int>& from = p.first;
			std::pair<int, int>& to = p.second;
			ChessPiece* killed_piece = this->try_move(from, to);
			current_player = current_player == 'W' ? 'B' : 'W';
			this->update_game();
			std::pair<int, std::pair<std::pair<int, int>, std::pair<int, int>>> result = minimax(alpha, beta, depth - 1, false);
			this->undo_move(from, to, killed_piece);
			current_player = current_player == 'W' ? 'B' : 'W';
			this->update_game();

			int move_score = result.first;
			if (move_score > best_score) {
				best_score = result.first;
				best_move = std::make_pair(from, to);
			}
			if (move_score > alpha) alpha = move_score;
			if (alpha >= beta) break;
		}
		return std::make_pair(best_score, best_move);
	}
	else {
		int best_score = 10000;
		for (std::pair<std::pair<int, int>, std::pair<int, int>> p : possible_moves) {
			std::pair<int, int>& from = p.first;
			std::pair<int, int>& to = p.second;
			ChessPiece* killed_piece = this->try_move(from, to);
			current_player = current_player == 'W' ? 'B' : 'W';
			this->update_game();
			std::pair<int, std::pair<std::pair<int, int>, std::pair<int, int>>> result = minimax(alpha, beta, depth - 1, false);
			this->undo_move(from, to, killed_piece);
			current_player = current_player == 'W' ? 'B' : 'W';
			this->update_game();

			int move_score = result.first;
			if (move_score < best_score) {
				best_score = result.first;
				best_move = std::make_pair(from, to);
			}
			if (move_score < beta) beta = move_score;
			if (alpha >= beta) break;
		}
		return std::make_pair(best_score, best_move);
	}
}

void GameClass::draw_on(sf::RenderWindow& window, std::tuple<int, int, int> board_param) {
	int board_x = std::get<0>(board_param);
	int board_y = std::get<1>(board_param);
	int board_size = std::get<2>(board_param);
	int tile_size = board_size / 8;
	for (std::pair<std::pair<int, int>, sf::CircleShape> pr : this->move_indicators) {
		std::pair<int, int>& p = pr.first;
		sf::CircleShape& circle = pr.second;
		circle.setPosition(board_x + p.first * tile_size, board_y + p.second * tile_size);
		window.draw(circle);
	}
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			if (this->board[i][j] == nullptr) continue;
			this->board[i][j]->setPosition(board_x + tile_size * i, board_y + tile_size * j);
			window.draw(*this->board[i][j]);
		}
	}
}

std::vector<std::pair<int, int>> GameClass::get_castling_moves(char player) {
	std::vector<std::pair<int, int>> res;
	int y = -1;
	if (player == 'W') y = 0;
	else if (player == 'B') y = 7;
	if (this->board[4][y] != nullptr && this->board[4][y]->type == ChessPiece::King && this->board[4][y]->nb_moves == 0) {
		if (this->board[0][y] != nullptr && this->board[0][y]->type == ChessPiece::Rook && this->board[0][y]->nb_moves == 0) {
			if (this->board[1][y] == nullptr && this->board[2][y] == nullptr && this->board[3][y] == nullptr) {
				res.push_back(std::make_pair(2, y));
			}
		}
		if (this->board[7][y] != nullptr && this->board[7][y]->type == ChessPiece::Rook && this->board[7][y]->nb_moves == 0) {
			if (this->board[5][y] == nullptr && this->board[6][y] == nullptr) {
				res.push_back(std::make_pair(6, y));
			}
		}
	}
	return res;
}

// TODO : handle upgrade mecaanism
// TODO : add en passant special move
// this function handlse special moves along with normal ones
void GameClass::move_piece(std::pair<int, int> from, std::pair<int, int> to) {
	std::cout << "moving " << this->board[from.first][from.second]->to_string() << " from (" << from.first << ", " << from.second << ") to (" << to.first << ", " << to.second << ")" << std::endl;
	/// special moves
	// castling
	if (this->board[from.first][from.second]->type == ChessPiece::King && abs(from.first - to.first) > 1) {
		int k = (from.first - to.first) < 0 ? -1 : 1;
		this->move_piece(std::make_pair(k == 1 ? 0 : 7, from.second), std::make_pair(from.first - k, from.second));
	}
	ChessPiece* killed_piece = try_move(from, to);
	if (this->board[to.first][to.second]->type == ChessPiece::Pawn) {
		if (current_player == 'W' && to.second == 7 || current_player == 'B' && to.second == 0) {
			// upgrade mecanism
			this->board[to.first][to.second]->promote_to(ChessPiece::Queen);
		}
	}
}

void GameClass::select_piece_at(int x, int y) {
	std::vector<std::pair<int, int>>::iterator it1 = moves[this->grabbed_piece_x][this->grabbed_piece_y].begin();
	std::vector<std::pair<int, int>>::iterator it2 = moves[this->grabbed_piece_x][this->grabbed_piece_y].end();
	bool is_target = find(it1, it2, std::make_pair(x, y)) != it2;
	bool is_grabbed_piece = grabbed_piece_x == x && grabbed_piece_y == y;
	if (!is_target && !is_grabbed_piece && this->board[x][y] != nullptr && this->board[x][y]->color == this->current_player) {
		update_game();
		this->move_indicators.clear();
		this->grabbed_piece_x = x;
		this->grabbed_piece_y = y;
		this->is_moving = true;

		for (std::pair<int, int> p : this->moves[x][y]) {
			sf::CircleShape circle(25.f);
			circle.setFillColor(sf::Color(127, 86, 68));
			this->move_indicators[p] = circle;
		}
	}
	else {
		if (is_target || (x == this->grabbed_piece_x && y == this->grabbed_piece_y)) {
			this->move_indicators.clear();
			this->is_moving = false;
			if (grabbed_piece_x != x || grabbed_piece_y != y) {
				this->move_piece(std::make_pair(grabbed_piece_x, grabbed_piece_y), std::make_pair(x, y));
				current_player = current_player == 'W' ? 'B' : 'W';
				update_game();
				nb_leafs = 0;
				std::pair<int, std::pair<std::pair<int, int>, std::pair<int, int>>> res = minimax(-10000, 10000);
				std::pair<std::pair<int, int>, std::pair<int, int>>& p = res.second;
				std::cout << "minimax bot move : " << "(" << p.first.first << ", " << p.first.second << ") (" << p.second.first << ", " << p.second.second << ")" << std::endl;
				std::cout << "minimax score : " << res.first << std::endl;
				std::cout << "visited leafs count : " << nb_leafs << std::endl;
				this->move_piece(p.first, p.second);
				current_player = current_player == 'W' ? 'B' : 'W';
				update_game();
			}
			this->grabbed_piece_x = -1;
			this->grabbed_piece_y = -1;
		}
	}
}

void GameClass::update_game() {
	this->moves = this->get_all_pieces_moves();
	ChessPiece* king = this->players_kings[current_player];
	this->king_moves = &this->moves[king->x][king->y];
	this->check = !is_safe_situation();
}

std::vector<ChessPiece*> GameClass::get_player_killed_pieces(char color) {
	return this->players_killed_pieces[color];
}

std::map<int, std::map<int, std::vector<std::pair<int, int>>>> GameClass::get_all_pieces_moves(bool test_moves) {
	std::map<int, std::map<int, std::vector<std::pair<int, int>>>> m;
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			if (this->board[i][j] == nullptr) continue;
			if (test_moves) {
				for (std::pair<int, int> p : this->board[i][j]->get_moves(this->board)) {
					if (is_safe_move(std::make_pair(i, j), p)) m[i][j].push_back(p);
				}
			}
			else m[i][j] = this->board[i][j]->get_moves(this->board);
			// Castling moves handling
			if (this->board[i][j]->type == ChessPiece::King) {
				std::vector<std::pair<int, int>> castling = get_castling_moves(this->board[i][j]->color);
				if (test_moves) {
					for (std::pair<int, int> p : castling) {
						int k = (p.first - i) < 0 ? -1 : 1;
						std::pair<int, int> from(k < 0 ? 0 : 7, j);
						std::pair<int, int> to(i + k, j);
						ChessPiece* killed_piece = this->try_move(from, to);
						if (is_safe_move(std::make_pair(i, j), p)) m[i][j].push_back(p);
						this->undo_move(from, to, killed_piece);
					}
				}
				else {
					for (std::pair<int, int> p : castling) m[i][j].push_back(p);
				}
			}
		}
	}
	return m;
}

bool GameClass::is_safe_situation() {
	std::pair<int, int> king = std::make_pair(this->players_kings[this->current_player]->x, this->players_kings[this->current_player]->y);
	std::vector<std::pair<int, int>> m = this->moves[king.first][king.second];
	char op_color = this->current_player == 'W' ? 'B' : 'W';
	int tiles_danger_values[8][8];
	for (int i = 0; i < 64; ++i) tiles_danger_values[i / 8][i % 8] = 0;
	std::map<int, std::map<int, std::vector<std::pair<int, int>>>> moves = this->get_all_pieces_moves(false);
	for (ChessPiece* piece : this->players_pieces[op_color]) {
		if (piece->x == -1 && piece->y == -1 || piece->killed) continue;
		for (std::pair<int, int> p : moves[piece->x][piece->y]) {
			++tiles_danger_values[p.first][p.second];
		}
	}
	return tiles_danger_values[king.first][king.second] == 0;
}

ChessPiece* GameClass::try_move(std::pair<int, int> from, std::pair<int, int> to) {
	ChessPiece* killed_piece = this->board[to.first][to.second];
	if (killed_piece != nullptr) {
		this->players_killed_pieces[killed_piece->color].push_back(killed_piece);
		killed_piece->killed = true;
	}
	this->board[to.first][to.second] = this->board[from.first][from.second];
	this->board[to.first][to.second]->move_to(to.first, to.second);
	this->board[from.first][from.second] = nullptr;
	return killed_piece;
}

void GameClass::undo_move(std::pair<int, int> from, std::pair<int, int> to, ChessPiece* killed_piece) {
	if (killed_piece != nullptr) {
		this->players_killed_pieces[killed_piece->color].pop_back();
		killed_piece->killed = false;
	}
	this->board[from.first][from.second] = this->board[to.first][to.second];
	this->board[from.first][from.second]->undo_move(from.first, from.second);
	this->board[to.first][to.second] = killed_piece;
}

bool GameClass::is_safe_move(std::pair<int, int> from, std::pair<int, int> to) {
	ChessPiece* killed_piece = try_move(from, to);
	std::map<int, std::map<int, std::vector<std::pair<int, int>>>> moves = this->get_all_pieces_moves(false);
	bool is_check = is_safe_situation();
	undo_move(from, to, killed_piece);
	return is_check;
}

std::string GameClass::get_game_state() {
	if (check && this->king_moves->empty()) return "CHECKMATE";
	else if (check) return "CHECK";
	return "PLAYING";
}

std::string GameClass::get_current_player() {
	if (this->current_player == 'W') return "WHITE";
	return "BLACK";
}

int GameClass::evaluate_game() {
	int score = 0;
	for (ChessPiece* p : this->players_pieces['W']) score += p->get_value() * tiles_factors[p->x][p->y];
	for (ChessPiece* p : this->players_pieces['B']) score -= p->get_value() * tiles_factors[p->x][p->y];
	return score;
}

GameClass::~GameClass()
{
	for (ChessPiece* p : this->players_pieces['W']) delete p;
	for (ChessPiece* p : this->players_pieces['B']) delete p;
}
