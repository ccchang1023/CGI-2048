#include "MoveTable.h"

MoveTable MoveTable::move_table;

MoveTable::MoveTable()
{
	row_move_table_ = new board_t*[2];
	row_move_score_table_ = new int*[2];
	row_can_move_ = new bool*[2];
	for(int i = 0;i < 2;i++) {
		row_move_table_[i] = new board_t[65536];
		row_move_score_table_[i] = new int[65536];
		row_can_move_[i] = new bool[65536];
	}
	init_move_table();
}

MoveTable::~MoveTable()
{
	for(int i = 0;i < 2;i++) {
		delete[] row_move_table_[i];
		delete[] row_move_score_table_[i];
		delete[] row_can_move_[i];
	}
	delete[] row_move_table_;
	delete[] row_move_score_table_;
	delete[] row_can_move_;
}

void MoveTable::init_move_table()
{
	for(int i = 0;i < 65536;i++) {
		for(int direction = 0;direction <= 1;direction++) {
			board_t row = i;
			bool movable = false;
			int score = move_row(row, 3 - direction * 3, 0 + direction * 3, direction, movable);
			row_move_table_[direction][i] = row;
			row_move_score_table_[direction][i] = score;
			row_can_move_[direction][i] = movable;
		}
	}
}

int MoveTable::move_row(board_t& row, int start_pos, int end_pos, int direction, bool& movable)
{
	int score = 0;
	bool has_moved = false;
	board_t tile_mask = 0xf << (start_pos * 4);
	for(int i = start_pos;i != end_pos;i += direction?1:-1) {
		board_t find_mask;
		int shift_count;
		if((row & tile_mask) == 0) {
			find_mask = direction?(tile_mask << 4):(tile_mask >> 4);
			shift_count = 1;
			while((row & find_mask) == 0) {
				find_mask = direction?((find_mask << 4) & 0xffff):(find_mask >> 4);
				if(find_mask == 0)
					break;
				shift_count++;
			}
			if(find_mask != 0) {
				board_t move_mask = direction?((row & find_mask) >> (4 * shift_count)):((row & find_mask) << (4 * shift_count));
				row &= ~find_mask;
				row |= move_mask;
				has_moved = true;
			}
			else
		break;
		}
		find_mask = direction?(tile_mask << 4):(tile_mask >> 4);
		board_t find_tile_mask = direction?((row & tile_mask) << 4):((row & tile_mask) >> 4);
		shift_count = 1;
		while((row & find_mask) == 0) {
			find_mask = direction?((find_mask << 4) & 0xffff):(find_mask >> 4);
			find_tile_mask = direction?((find_tile_mask << 4) & 0xffff):(find_tile_mask >> 4);
			if(find_mask == 0)
				break;
			shift_count++;
		}
		if(find_mask == 0)
			break;
		if(((row & find_mask) ^ find_tile_mask) == 0) {
			int restore_tile_count = 0;
			while((find_tile_mask & 0xf) == 0) {
				find_tile_mask >>= 4;
				restore_tile_count++;
			}
			find_tile_mask += 1;
			score += 1 << find_tile_mask;
			find_tile_mask <<= restore_tile_count * 4;
			board_t add_tile_mask = direction?(find_tile_mask >> (4 * shift_count)):(find_tile_mask << (4 * shift_count));
			row &= ~(find_mask | tile_mask);
			row |= add_tile_mask;
			has_moved = true;
		}
		tile_mask = direction?(tile_mask << 4):(tile_mask >> 4);
	}
	movable = has_moved;
	return score;
}


