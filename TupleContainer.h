#ifndef __TUPLECONTAINER_H__
#define __TUPLECONTAINER_H__

#include "Tuple.h"
#include <vector>

using std::vector;

class TupleContainer : public Tuple
{
public:
	TupleContainer(double weight=1);
	~TupleContainer();
	// implement interface
	virtual double query_tuple_value(const GameBoard& game_board) const ;
	virtual void update_tuple_value(const GameBoard& game_board, const double& value);
	virtual void add_tuple(Tuple* tuple);
	virtual void save_tuple(ofstream& fout) const;
	virtual void load_tuple(ifstream& fin);
	
protected:
	mutable vector<Tuple*> tuples_;
};

#endif
