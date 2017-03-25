#ifndef __TILETUPLE_H__
#define __TILETUPLE_H__

#include "Tuple.h"

class TileTuple : public Tuple
{
public:
	TileTuple(double weight=1);
	virtual ~TileTuple();
	// implement interface
	virtual double query_tuple_value(const GameBoard& game_board) const;
	virtual void update_tuple_value(const GameBoard& game_board, const double& value);
	virtual void save_tuple(ofstream& fout) const = 0;
	virtual void load_tuple(ifstream& fin) = 0;

protected:
	virtual board_t get_index(const board_t& game_board) const = 0;

protected:
	double* score_table_;
};

#endif
