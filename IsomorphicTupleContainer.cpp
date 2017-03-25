#include "IsomorphicTupleContainer.h"

IsomorphicTupleContainer::IsomorphicTupleContainer(double weight):
TupleContainer(weight)
{
}

double IsomorphicTupleContainer::query_tuple_value(const GameBoard& game_board) const
{
	double sum_of_values = 0;
	GameBoard isomorphic_boards[8];
	game_board.get_isomorphic_boards(isomorphic_boards);
	for(int isomorphic_num = 0;isomorphic_num < 8;isomorphic_num++) {
		for(vector<Tuple*>::iterator it = tuples_.begin(); it != tuples_.end(); ++it) {
			sum_of_values += (*it)->query_tuple_value(isomorphic_boards[isomorphic_num]);
		}
	}
	return weight_ * sum_of_values;
}

void IsomorphicTupleContainer::update_tuple_value(const GameBoard& game_board, const double& value)
{
	GameBoard isomorphic_boards[8];
	game_board.get_isomorphic_boards(isomorphic_boards);
	for(int isomorphic_num = 0;isomorphic_num < 8;isomorphic_num++) {
		for(vector<Tuple*>::iterator it = tuples_.begin(); it != tuples_.end(); ++it) {
			(*it)->update_tuple_value(isomorphic_boards[isomorphic_num], value);
		}
	}
}
