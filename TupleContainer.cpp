#include "TupleContainer.h"

TupleContainer::TupleContainer(double weight):
Tuple(weight),
tuples_()
{
}

TupleContainer::~TupleContainer()
{
	for(vector<Tuple*>::iterator it = tuples_.begin(); it != tuples_.end(); ++it) {
		Tuple *each_tuple = *it;
		delete each_tuple;
	}
}

double TupleContainer::query_tuple_value(const GameBoard& game_board) const
{
	double sum_of_values = 0;
	for(vector<Tuple*>::iterator it = tuples_.begin(); it != tuples_.end(); ++it)
		sum_of_values += (*it)->query_tuple_value(game_board);
	return weight_ * sum_of_values;
}

void TupleContainer::update_tuple_value(const GameBoard& game_board, const double& value)
{
	for(vector<Tuple*>::iterator it = tuples_.begin(); it != tuples_.end(); ++it)
		(*it)->update_tuple_value(game_board, value);
}

void TupleContainer::add_tuple(Tuple* tuple)
{
	tuples_.push_back(tuple);
}

void TupleContainer::save_tuple(ofstream& fout) const
{
	for(vector<Tuple*>::iterator it = tuples_.begin(); it != tuples_.end(); ++it)
		(*it)->save_tuple(fout);
}

void TupleContainer::load_tuple(ifstream& fin)
{
	for(vector<Tuple*>::iterator it = tuples_.begin(); it != tuples_.end(); ++it)
		(*it)->load_tuple(fin);
}

