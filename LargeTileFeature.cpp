#include "LargeTileFeature.h"

LargeTileFeature::LargeTileFeature(double weight):
TileTuple(weight)
{
	score_table_ = new double[1024];  // 32k ~ 2k, 0~3 tiles
	memset(score_table_, 0, sizeof(double) * 1024);
}

LargeTileFeature::~LargeTileFeature()
{
	delete[] score_table_;
}

board_t LargeTileFeature::get_index(const board_t& game_board) const
{
	static int shift_pos[5] = {0, 2, 4, 6, 8};
	int index = 0;
	int count = 0;
	int tile = 0;
	for(board_t mask = 0xf;mask != 0;mask <<= 4) {
		if((tile = (game_board & mask) >> count) >= 11)
			index += 0x1ull << shift_pos[tile - 11];
		count += 4;
	}
	return index;
}

int LargeTileFeature::get_isomorphic_indexes(const board_t& original_index, board_t* isomorphic_indexes) const
{
	isomorphic_indexes[0] = original_index;
	return 1;
}

void LargeTileFeature::save_tuple(ofstream& fout) const
{
	fout.write((char*)score_table_, sizeof(double) * 1024);
}

void LargeTileFeature::load_tuple(ifstream& fin)
{
	fin.read((char*)score_table_, sizeof(double) * 1024);
}


