#ifndef __ZHASHTABLE_H__
#define __ZHASHTABLE_H__

#include "Random.h"
#include <cstring>

struct BoardStatus
{
	unsigned long long board_;
	double value_;
	int search_depth_;
	bool has_dead_board_;
};

class ZHashTable
{
public:
	static ZHashTable z_hash_table;

public:
	double get_value(unsigned long long board, int search_depth, bool& has_dead_board, bool& find_value);
	void set_value(unsigned long long board, int search_depth, double value, bool has_dead_board);
	void clear();

private:	
	ZHashTable(int size=0x100000);  // default 20 bits
	~ZHashTable();
	void init_hash_table();
	unsigned long long get_hash_value(unsigned long long board);

private:
	unsigned long long* fast_hash_table_;
	BoardStatus* transposition_table_;
	int index_mask_;
};

#endif

