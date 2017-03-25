#include "MergeFeature.h"

//      0000    +   00000    +   00000     , totally 14 bits
//   blankCount   mergeCount   layerCount  
//  

MergeFeature::MergeFeature(double weight):
TileTuple(weight)
{
	score_table_ = new double[16384];
	memset(score_table_, 0, sizeof(double) * 16384);
}

MergeFeature::~MergeFeature()
{
	delete[] score_table_;
}

unsigned long long MergeFeature::get_index(const unsigned long long& game_board) const
{
	int tileArray[16];
	int shiftNum;
	unsigned long long mask = 0xf000000000000000;
	int count = 0;
	for(shiftNum = 60 ;shiftNum >= 0;shiftNum-=4,mask=mask>>4)
		tileArray[count++] = static_cast<int>((game_board&mask)>>shiftNum);
	int featureIndex;
	int layerDifferent;
	int blankTileCount = 0;
	int mergeCount = 0;
	int layerMergeCount = 0;

	for(int i=0;i<16;i++)
	{
		if(tileArray[i]==0)
		{
			blankTileCount++;
			continue;
		}
		
		if((tileArray[i] == tileArray[i+1]) && (i%4!=3))
			mergeCount++;
		if((tileArray[i] == tileArray[i+4]) && (i<12))
			mergeCount++;

		layerDifferent = abs(tileArray[i]-tileArray[i+1]);
		if((layerDifferent==1)&& (i%4!=3) && (tileArray[i+1]!=0))
			layerMergeCount++;
		layerDifferent = abs(tileArray[i]-tileArray[i+4]);
		if((layerDifferent==1)&& (i<12) && (tileArray[i+4]!=0) )
			layerMergeCount++;
	}
	featureIndex = (blankTileCount<<10)| (mergeCount<<5)|layerMergeCount;
	return featureIndex;
}

/*int LargeTileFeature::get_isomorphic_indexes(const board_t& original_index, board_t* isomorphic_indexes) const
{
	isomorphic_indexes[0] = original_index;
	return 1;
}*/

void MergeFeature::save_tuple(ofstream& fout) const
{
	fout.write((char*)score_table_, sizeof(double) * 16384);
}

void MergeFeature::load_tuple(ifstream& fin)
{
	fin.read((char*)score_table_, sizeof(double) * 16384);
}


