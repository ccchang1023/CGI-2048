#ifndef __TUPLE_H__
#define __TUPLE_H__

#include "../GameBoard.h"
#include <fstream>

using std::fstream;

class Tuple
{
public:
	Tuple(double weight=1) { weight_ = weight; }
	virtual ~Tuple() {}
	virtual double query_tuple_value(const GameBoard& game_board) const = 0;
	virtual void update_tuple_value(const GameBoard& game_board, const double& value) = 0;
	virtual void save_tuple(ofstream& fout) const = 0;
	virtual void load_tuple(ifstream& fin) = 0;

protected:
	double weight_;
};

#endif
