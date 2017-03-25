#ifndef __ISOMORPHICTUPLECONTAINER_H__
#define __ISOMORPHICTUPLECONTAINER_H__

#include "TupleContainer.h"

class IsomorphicTupleContainer : public TupleContainer
{
public:
	IsomorphicTupleContainer(double weight=1);
	// implement interface
	virtual double query_tuple_value(const GameBoard& game_board) const;
	virtual void update_tuple_value(const GameBoard& game_board, const double& value);
};

#endif

