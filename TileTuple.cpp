#include "TileTuple.h"

TileTuple::TileTuple(double weight):
Tuple(weight),
score_table_(NULL)
{
}

TileTuple::~TileTuple()
{
}

double TileTuple::query_tuple_value(const GameBoard& game_board) const
{
	return weight_ * score_table_[get_index(game_board.get_board())];
}

void TileTuple::update_tuple_value(const GameBoard& game_board, const double& value)
{
	score_table_[get_index(game_board.get_board())] += value;
}

