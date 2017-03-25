#ifndef __MOVETABLE_H__
#define __MOVETABLE_H__

#include <cstring>

typedef unsigned long long board_t;

enum MoveDirection
{
	MOVE_UP, MOVE_RIGHT, MOVE_DOWN, MOVE_LEFT
};

class MoveTable
{
public:
	static MoveTable move_table;

private:
	MoveTable();
	~MoveTable();
	void init_move_table();
	int move_row(board_t& row, int start_pos, int end_pos, int direction, bool& movable);

public:
	board_t** row_move_table_;
	int** row_move_score_table_;
	bool** row_can_move_;
};

#endif

