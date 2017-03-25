#ifndef __EXPECTIMAX_H__
#define __EXPECTIMAX_H__

#include "GameBoard.h"
#include "TupleNetwork/TupleNetwork.h"
#include "ZHashTable.h"
#include <iostream>
#include <string>

using namespace std;

class ExpectiMax
{
public:
	ExpectiMax(int search_depth);
	MoveDirection find_best_move(GameBoard board);
	void set_search_depth(int search_depth) { search_depth_ = search_depth; }
	int get_search_depth() const { return search_depth_; }

private:
	double get_best_successor_and_score(GameBoard board, int depth, MoveDirection& best_successor, bool& has_dead_board);
	double expecti_max_ai_turn(GameBoard board, int depth, bool& has_dead_board);
	double expecti_max_random_turn(GameBoard board, int depth, bool& has_dead_board);
	double evaluate(GameBoard board);

private:
	int search_depth_;
};

#endif

