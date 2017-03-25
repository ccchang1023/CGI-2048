#ifndef __LARGETILEFEATURE_H__
#define __LARGETILEFEATURE_H__

#include "TileTuple.h"

class LargeTileFeature : public TileTuple
{
public:
	LargeTileFeature(double weight=1);
	~LargeTileFeature();
	// implement interface
	virtual void save_tuple(ofstream& fout) const;
	virtual void load_tuple(ifstream& fin);

protected:
	board_t get_index(const board_t& game_board) const;
	int get_isomorphic_indexes(const board_t& original_index, board_t* isomorphic_indexes) const;
};

#endif
