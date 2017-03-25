#ifndef __TDLEARNING_H__
#define __TDLEARNING_H__

#include "GameBoard.h"
#include "TupleNetwork/TupleNetwork.h"
#include "ExpectiMax.h"
#include <iostream>
#include <fstream> 
#include <vector>
#include <string>
#include <map>
#include <utility>
#include <sstream>

#define EXPECTIMAX_SEARCH_DEPTH 3

using namespace std;

class TdLearning
{
public:
	TdLearning(bool is_learning_mode=true, double learning_rate=0.0025);
	~TdLearning();
	void start_play_game(int total_rounds, string tuple_network_filename);
	MoveDirection find_best_move(GameBoard board);

private:
	int play_game(GameBoard start_board, int start_score);
	void show_detail(int rounds, double win_rate);
	void show_max_tile_count(int rounds);
	double evaluate(GameBoard board, MoveDirection direction);
	void learning(vector<BoardMoveRecord> move_records);
	void learn_evaluation(BoardMoveRecord current_record, BoardMoveRecord next_record, BoardMoveRecord last_record);
	void learn_evaluation_td_lamda(BoardMoveRecord current_record, double *nextFiveBoardScore);
	void load_initialized_board(string filename, vector< pair<GameBoard, int> >& initialized_board);

private:
	bool is_learning_mode_;
	double learning_rate_;
	int win_count_;
	int max_tile_;
	int max_score_;
	int max_score_normalize_;
	double average_score_;
	map<int, int> max_tile_count_;
	ExpectiMax expecti_max_;
};

#endif
