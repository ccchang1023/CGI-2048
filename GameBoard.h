#ifndef __GAMEBOARD_H__
#define __GAMEBOARD_H__

#include "MoveTable.h"
#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;

struct MoveStatus;

class GameBoard
{
public:
	GameBoard(void);
	GameBoard(board_t bitboard);
	~GameBoard(void);
	inline board_t get_board() const { return board_; }
	void initialize();
	int move(MoveDirection move_direction, bool& is_legal_move);
	void add_random_tile();
	void show_board();
	int get_successor(MoveDirection successors[]);
	int get_all_possible_board(GameBoard boards[]);
	MoveStatus ramdom_play_overall();
	bool is_finish();
	int get_max_tile();
	int get_min_tile();
	bool can_move(MoveDirection direction);
	int count_empty_tile();
	void get_isomorphic_boards(GameBoard isomorphic_boards[]) const;
	
	bool has_16_2k_or_more() const;
	
	bool has_16_8_4_2_1k_or_more() const;
	bool has_16_8_4_2k_or_more() const;
	bool has_16384_and_8192_and_4096_or_more() const;
	bool has_16384_and_8192() const;
	bool has_8192() const;
	bool has_16384() const;
	bool has_32768() const;
	int get_max_tile_greater_than_16384();
	bool is_possible_dead();

private:
	board_t get_row(int row);
	board_t get_column(int column);
	board_t restore_row(board_t row_bits, int row);
	board_t restore_column(board_t column_bits, int column);
	int get_tile(int row, int column);
	GameBoard get_flip_board() const;
	GameBoard get_mirror_board() const;
	GameBoard get_flip_diagonal_3_12_board() const;
	GameBoard get_flip_diagonal_0_15_board() const;

private:
	board_t board_;
};

struct MoveStatus
{
	MoveStatus():
	board_(0), score_(0), direction_(MOVE_UP) 
	{
	}
	MoveStatus(GameBoard board, int score, MoveDirection direction):
	board_(board), score_(score), direction_(direction)
	{
	}
	GameBoard board_;
	int score_;
	MoveDirection direction_;
};

struct BoardMoveRecord
{
	GameBoard board_;  // s
	int score_;
	bool operator==(const BoardMoveRecord& board_record) {
		return board_.get_board() == board_record.board_.get_board() && score_ == board_record.score_;
	} 
};

#endif

