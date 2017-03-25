#ifndef __FOURTUPLE_H__
#define __FOURTUPLE_H__

#include "TileTuple.h"

class FourTuple : public TileTuple
{
public:
	FourTuple(double weight=1);
	~FourTuple();
	// implement interface
	virtual void save_tuple(ofstream& fout) const;
	virtual void load_tuple(ifstream& fin);

protected:
	virtual board_t get_index(const board_t& game_board) const = 0;
};

#endif
