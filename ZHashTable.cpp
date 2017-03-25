#include "ZHashTable.h"

ZHashTable ZHashTable::z_hash_table(0x1000000);

ZHashTable::ZHashTable(int size)
{
	init_hash_table();
	transposition_table_ = new BoardStatus[size];
	memset(transposition_table_, 0, sizeof(BoardStatus) * size);
	index_mask_ = size - 1;
}

ZHashTable::~ZHashTable()
{
	delete[] fast_hash_table_;
	delete[] transposition_table_;
}

void ZHashTable::clear()
{
	memset(transposition_table_, 0, sizeof(BoardStatus) * (index_mask_ + 1));
}

void ZHashTable::init_hash_table()
{
	Random my_random(0);
	unsigned long long normal_hash_table[16][16]; // [tile][location]
	for(int i = 0;i < 16;i++)
		for(int j = 0;j < 16;j++)
			normal_hash_table[i][j] = my_random.get_rand_num();
	fast_hash_table_ = new unsigned long long[0x40000]; // 4 row
	for(int i = 0;i < 65536;i++) {
		for(int j = 0;j < 4;j++) {
			fast_hash_table_[i * 4 + j] = normal_hash_table[(i & 0xf000) >> 12][j * 4] ^
										  normal_hash_table[(i & 0x0f00) >> 8][j * 4 + 1] ^
										  normal_hash_table[(i & 0x00f0) >> 4][j * 4 + 2] ^
										  normal_hash_table[(i & 0x000f)][j * 4 + 3];
		}
	}
}

unsigned long long ZHashTable::get_hash_value(unsigned long long board)
{
	return fast_hash_table_[((board & 0xffff000000000000) >> 46)] ^
		   fast_hash_table_[((board & 0x0000ffff00000000) >> 30) + 1] ^
		   fast_hash_table_[((board & 0x00000000ffff0000) >> 14) + 2] ^
		   fast_hash_table_[((board & 0x000000000000ffff) << 2) + 3];
}

double ZHashTable::get_value(unsigned long long board, int search_depth, bool& has_dead_board, bool& find_value)
{
	unsigned long long hash_value = get_hash_value(board);
	find_value = false;
	BoardStatus board_status = transposition_table_[hash_value & index_mask_];
	if(board_status.board_ == board && board_status.search_depth_ >= search_depth) {
		find_value = true;
		has_dead_board = board_status.has_dead_board_;
		return board_status.value_;
	}
	return 0.0;
}

void ZHashTable::set_value(unsigned long long board, int search_depth, double value, bool has_dead_board)
{
	unsigned long long hash_value = get_hash_value(board);
	BoardStatus& board_status = transposition_table_[hash_value & index_mask_];
	if(board_status.board_ != board) {
		board_status.board_ = board;
		board_status.has_dead_board_ = has_dead_board;
		board_status.value_ = value;
		board_status.search_depth_ = search_depth;
	}
	else if(board_status.search_depth_ < search_depth) {		
		board_status.has_dead_board_ = has_dead_board;
		board_status.value_ = value;
		board_status.search_depth_ = search_depth;
	}
}

