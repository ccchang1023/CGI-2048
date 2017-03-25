#include "SixTuple.h"

SixTuple::SixTuple(double weight):
TileTuple(weight)
{
	score_table_ = new double[0x1000000];
	memset(score_table_, 0, sizeof(double) * 0x1000000);
}

SixTuple::~SixTuple()
{
	delete[] score_table_;
}

void SixTuple::save_tuple(ofstream& fout) const
{
	fout.write((char*)score_table_, sizeof(double) * 0x1000000);
}

void SixTuple::load_tuple(ifstream& fin)
{
	fin.read((char*)score_table_, sizeof(double) * 0x1000000);
}


