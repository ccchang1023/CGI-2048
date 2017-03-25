#ifndef __TUPLENETWORK_H__
#define __TUPLENETWORK_H__

#include <fstream>
#include <string>
#include "../GameBoard.h"
#include "Tuple.h"
#include "TupleContainer.h"
#include "IsomorphicTupleContainer.h"
#include "TupleCollection.h"
#include "LargeTileFeature.h"
#include "MergeFeature.h"

using std::fstream;
using std::string;

class TupleNetwork
{
public:
	static TupleNetwork tuple_network_;
	// static TupleNetwork tuple_network_2;
	// static TupleNetwork tuple_network_3;
	// static TupleNetwork tuple_network_4;
	// static TupleNetwork tuple_network_5;
public:
	TupleNetwork();
	~TupleNetwork();
	double get_board_value(const GameBoard& board) const;
	void update_board_value(const GameBoard& board, const double& value);
	void save_tuple_network(const string& filename) const;
	void load_tuple_network(const string& filename);
	GameBoard get_masquerade_board(const GameBoard& board) const;
	static int compare_tile (const void *a, const void *b);

private:
	Tuple* create_isomorphic_tuple_collection() const;

private:
	Tuple* tuples;
};

#endif
