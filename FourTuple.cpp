#include "FourTuple.h"

FourTuple::FourTuple(double weight):
TileTuple(weight)
{
	score_table_ = new double[0x10000];
	memset(score_table_, 0, sizeof(double) * 0x10000);
}

FourTuple::~FourTuple()
{
	delete[] score_table_;
}

void FourTuple::save_tuple(ofstream& fout) const
{
	fout.write((char*)score_table_, sizeof(double) * 0x10000);
}

void FourTuple::load_tuple(ifstream& fin)
{
	fin.read((char*)score_table_, sizeof(double) * 0x10000);
}

