#include "GameBoard.h"

GameBoard::GameBoard(void):
board_(0)
{
}

GameBoard::GameBoard( board_t bitboard ):
board_(bitboard)
{
}

GameBoard::~GameBoard(void)
{
}

void GameBoard::initialize()
{
	board_ = 0;
	add_random_tile();
	add_random_tile();
}

int GameBoard::move(MoveDirection move_direction, bool& is_legal_move)
{
	board_t new_board = 0;
	is_legal_move = true;
	int score = 0;
	if(move_direction == MOVE_UP || move_direction == MOVE_DOWN) {
		for(int i = 0;i < 4;i++) {
			board_t temp_column = 0;
			temp_column = get_column(i);
			new_board |= restore_column(MoveTable::move_table.row_move_table_[move_direction == MOVE_DOWN][temp_column], i);
			score += MoveTable::move_table.row_move_score_table_[move_direction == MOVE_DOWN][temp_column];
		}
	}
	else {
		for(int i = 0;i < 4;i++) {
			board_t temp_row = 0;
			temp_row = get_row(i);
			new_board |= restore_row(MoveTable::move_table.row_move_table_[move_direction == MOVE_RIGHT][temp_row], i);
			score += MoveTable::move_table.row_move_score_table_[move_direction == MOVE_RIGHT][temp_row];
		}
	}
	if(board_ == new_board)
		is_legal_move = false;
	board_ = new_board;
	return score;
}

int GameBoard::get_successor(MoveDirection successors[])
{
	int successor_count = 0;
	for(int direction = 0;direction < 4;direction++) {
		if(can_move(static_cast<MoveDirection>(direction)))
			successors[successor_count++] = static_cast<MoveDirection>(direction);
	}
	return successor_count;
}

int GameBoard::get_all_possible_board( GameBoard boards[] )
{
	board_t tile_2_mask = 0x1;
	board_t tile_4_mask = 0x2;
	int boards_count = 0;
	for(board_t tile_mask = 0xf;tile_mask != 0;tile_mask <<= 4, tile_4_mask <<= 4) {
		if(board_ & tile_mask)
			continue;
		boards[boards_count++] = board_ | tile_4_mask;
	}
	for(board_t tile_mask = 0xf;tile_mask != 0;tile_mask <<= 4, tile_2_mask <<= 4) {
		if(board_ & tile_mask)
			continue;
		boards[boards_count++] = board_ | tile_2_mask;
	}
	return boards_count;
}

board_t GameBoard::get_row( int row )
{
	return board_ >> (row << 4) & 0xffff;
}

board_t GameBoard::get_column( int column )
{
	board_t temp_board = board_ >> (column << 2) & 0xf000f000f000f;
	return (temp_board & 0xf000000000000) >> 36 |
      	   (temp_board & 0x0000f00000000) >> 24 |
      	   (temp_board & 0x00000000f0000) >> 12 |
       	   (temp_board & 0x000000000000f);
}

board_t GameBoard::restore_row( board_t row_bits, int row )
{
  	return row_bits << (row << 4);
}

board_t GameBoard::restore_column( board_t column_bits, int column )
{
  	return ((column_bits & 0xf000) << 36 |
            (column_bits & 0x0f00) << 24 |
            (column_bits & 0x00f0) << 12 |
            (column_bits & 0x000f)        ) << (column << 2);
}

void GameBoard::show_board()
{
  	for(int row = 3;row >= 0;row--) {
   		for(int column = 3;column >= 0;column--)
      		cout << get_tile(row, column) << "\t";
    	cout << "\n";
  	}
}

int GameBoard::get_tile( int row, int column )
{
	board_t power = (board_ >> (row << 4) & 0xffff) >> (column << 2) & 0xf;
	if(power == 0)
		return 0;
	return 1 << power;
}

void GameBoard::add_random_tile()
{
	int two_tile_rate = 9;
	int empty_tile_num = count_empty_tile();
	int random_tile_location = rand() % empty_tile_num;
	board_t random_tile = (rand() % 10 < two_tile_rate)?0x1:0x2;
	int count = 0;
	for(board_t tile_mask = 0xf;tile_mask != 0;tile_mask <<= 4, random_tile <<= 4) {
		if((board_ & tile_mask) != 0)
			continue;
		if(count == random_tile_location) {
			board_ |= random_tile;
			break;
		}
		count++;
	}
}

int GameBoard::count_empty_tile()
{
	int count = 0;
	board_t tile_mask = 0xf;
	for(;tile_mask & 0xffffffffffffffff;tile_mask <<= 4) {
		if((board_ & tile_mask) == 0)
			count++;
	}
	return count;
}

MoveStatus GameBoard::ramdom_play_overall()
{
	bool assign_first = true;
	MoveStatus first_move;
	MoveDirection successors[4];
	GameBoard simulated_board = *this;
	int successor_count = simulated_board.get_successor(successors);
	while(successor_count != 0) {
		int random_choose_index = rand() % successor_count;
		bool is_legal_move;
		int score = simulated_board.move(successors[random_choose_index], is_legal_move);
		simulated_board.add_random_tile();
		if(assign_first) {
			assign_first = false;
			first_move.board_ = simulated_board;
			first_move.score_ = 0;
			first_move.direction_ = successors[random_choose_index];
		}
		first_move.score_ += score;
		successor_count = simulated_board.get_successor(successors);
	}
	return first_move;
}

bool GameBoard::is_finish()
{
	bool movable = false;
	board_t temp_column;
	board_t temp_row;
	for(int i = 0;i < 4;i++) {
		temp_column = get_column(i);
		movable |= MoveTable::move_table.row_can_move_[0][temp_column];
		movable |= MoveTable::move_table.row_can_move_[1][temp_column];
		temp_row = get_row(i);
		movable |= MoveTable::move_table.row_can_move_[0][temp_row];
		movable |= MoveTable::move_table.row_can_move_[1][temp_row];
		if(movable)
			break;
	}
	return !movable;
}

int GameBoard::get_max_tile()
{
	board_t count_board = board_;
	int max_tile = 0;
	for(int i = 0;i < 16;i++) {
		if((count_board & 0xf) > max_tile)
			max_tile = count_board & 0xf;
		count_board >>= 4;
	}
	return 1 << max_tile;
}

int GameBoard::get_min_tile()
{
	board_t count_board = board_;
	int min_tile = 0xf;
	for(int i = 0;i < 16;i++) {
		if((count_board & 0xf) < min_tile)
			min_tile = count_board & 0xf;
		count_board >>= 4;
	}
	return min_tile;
}

bool GameBoard::can_move( MoveDirection direction )
{
	bool movable = false;
	if(direction == MOVE_UP || direction == MOVE_DOWN) {
		for(int i = 0;i < 4;i++) {
			board_t temp_column = get_column(i);
			movable |= MoveTable::move_table.row_can_move_[direction == MOVE_DOWN][temp_column];
		}
	}
	else {
		for(int i = 0;i < 4;i++) {
			board_t temp_row = get_row(i);
			movable |= MoveTable::move_table.row_can_move_[direction == MOVE_RIGHT][temp_row];
		}
	}
	return movable;
}

bool GameBoard::has_8192() const
{
	for(board_t temp_board = board_;temp_board > 0;temp_board >>= 4) {
		if((temp_board & 0xf) == 13)
			return true;
	}
	return false;
}

bool GameBoard::has_16384() const
{
	for(board_t temp_board = board_;temp_board > 0;temp_board >>= 4) {
		if((temp_board & 0xf) == 14)
			return true;
	}
	return false;
}

bool GameBoard::has_32768() const
{
	for(board_t temp_board = board_;temp_board > 0;temp_board >>= 4) {
		if((temp_board & 0xf) == 15)
			return true;
	}
	return false;
}

bool GameBoard::has_16_2k_or_more() const
{
		int more_than_16k = 0;
        int more_than_2k = 0;
        for(board_t temp_board = board_;temp_board > 0;temp_board >>= 4) {
                if((temp_board & 0xf) == 15)    // 32768
                        return true;
				else if((temp_board & 0xf) >= 14)
                        more_than_16k++;
                else if((temp_board & 0xf) >= 11)	// more than 2048
                        more_than_2k++;
				if(more_than_16k > 0 && more_than_2k > 0)
					return true;
				if(more_than_16k > 1)
					return true;
        }
        return false;
}

bool GameBoard::has_16384_and_8192() const
{
	bool has_16384 = false;
	bool has_8192 = false;
	for(board_t temp_board = board_;temp_board > 0;temp_board >>= 4) {
		if((temp_board & 0xf) == 13)
			has_8192 = true;
		else if((temp_board & 0xf) == 14)
			has_16384 = true;
		else if((temp_board & 0xf) == 15)
			return true;
		if(has_8192 && has_16384)
			return true;
	}
	return false;
}


bool GameBoard::has_16_8_4_2k_or_more() const
{
        int count_16k = 0;
        int count_8k = 0;
        int count_4k = 0;
        int count_2k = 0;

        for(board_t temp_board = board_;temp_board > 0;temp_board >>= 4) {
                if((temp_board & 0xf) == 15)    // 32768
                        return true;
                else if((temp_board & 0xf) == 11)
                        count_2k++;
                else if((temp_board & 0xf) == 12)
                        count_4k++;
                else if((temp_board & 0xf) == 13)
                        count_8k++;
                else if((temp_board & 0xf) == 14)
                        count_16k++;
                if(count_16k>0 && count_8k>0 && count_4k>0 && count_2k>0)
                        return true;
                if(count_16k>0 && count_8k>0 && count_4k>1)
                        return true;
                if(count_16k>0 && count_8k>1)
                        return true;
                if(count_16k>1)
                        return true;
        }
        return false;
}


bool GameBoard::has_16384_and_8192_and_4096_or_more() const
{
	bool has_16384 = false;
	bool has_8192 = false;
	bool has_4096 = false;
	for(board_t temp_board = board_;temp_board > 0;temp_board >>= 4) {
		if((temp_board & 0xf) == 13)
			has_8192 = true;
		else if((temp_board & 0xf) == 14)
			has_16384 = true;
		else if((temp_board & 0xf) == 12)
			has_4096 = true;
		else if((temp_board & 0xf) == 15)	// 32768
			return true;
		if(has_8192 && has_16384 && has_4096)
			return true;
	}
	return false;
}


bool GameBoard::has_16_8_4_2_1k_or_more() const
{
	bool has_16384 = false;
	bool has_8192 = false;
	bool has_4096 = false;
	bool has_2048 = false;
	bool has_1024 = false;
	for(board_t temp_board = board_;temp_board > 0;temp_board >>= 4) {
		if((temp_board & 0xf) == 15)	// 32768
			return true;
		if((temp_board & 0xf) == 14)
			has_16384 = true;
		else if((temp_board & 0xf) == 13)
			has_8192 = true;
		else if((temp_board & 0xf) == 12)
			has_4096 = true;
		else if((temp_board & 0xf) == 11)
			has_2048 = true;
		else if((temp_board & 0xf) == 10)
			has_1024 = true;	
		if(has_16384 && has_8192 && has_4096 && has_2048 && has_1024)
			return true;
	}
	return false;
}

// bool GameBoard::has_16_8_4_2k_or_more() const
// {
	// bool has_16384 = false;
	// bool has_8192 = false;
	// bool has_4096 = false;
	// bool has_2048 = false;
	// for(board_t temp_board = board_;temp_board > 0;temp_board >>= 4) {
		// if((temp_board & 0xf) == 15)	// 32768
			// return true;
		// if((temp_board & 0xf) == 14)
			// has_16384 = true;
		// else if((temp_board & 0xf) == 13)
			// has_8192 = true;
		// else if((temp_board & 0xf) == 12)
			// has_4096 = true;
		// else if((temp_board & 0xf) == 11)
			// has_2048 = true;	
		// if(has_16384 && has_8192 && has_4096 && has_2048)
			// return true;
	// }
	// return false;
// }


int GameBoard::get_max_tile_greater_than_16384()
{
	// 16384, 8192, 4096, 2048, 1024
	bool each_tiles[5] = {false, false, false, false, false};
	for(board_t temp_board = board_;temp_board > 0;temp_board >>= 4) {
		for(int i = 0;i < 5;i++) {
			if((temp_board & 0xf) == 14 - i) {
				each_tiles[i] = true;
				break;
			}
		}
	}
	int max_tile = 0;
	for(int i = 0;i < 5;i++) {
		if(each_tiles[i] == false)
			break;
		max_tile += (0x1 << (14 - i));
	}
	return max_tile;
}

bool GameBoard::is_possible_dead()
{
	GameBoard possible_boards[32];
	int possible_boards_count = get_all_possible_board(possible_boards);
	for(int i = 0;i < possible_boards_count;i++) {
		if(possible_boards[i].is_finish())
			return true;
	}
	return false;
}

void GameBoard::get_isomorphic_boards(GameBoard isomorphic_boards[]) const
{
	GameBoard flip_board = get_flip_board();
	GameBoard mirror_board = get_mirror_board();
	GameBoard flip_diagonal_3_12_board = get_flip_diagonal_3_12_board();
	GameBoard flip_diagonal_0_15_board = get_flip_diagonal_0_15_board();
	isomorphic_boards[0] = *this;
	isomorphic_boards[1] = flip_board;
	isomorphic_boards[2] = mirror_board;
	isomorphic_boards[3] = flip_board.get_mirror_board();
	isomorphic_boards[4] = flip_diagonal_3_12_board;
	isomorphic_boards[5] = flip_diagonal_0_15_board;
	isomorphic_boards[6] = flip_diagonal_3_12_board.get_flip_board();
	isomorphic_boards[7] = flip_diagonal_0_15_board.get_flip_board();
}

GameBoard GameBoard::get_flip_board() const
{
  	return (board_ & 0xffff000000000000) >> 48 |
           (board_ & 0x0000ffff00000000) >> 16 |
           (board_ & 0x00000000ffff0000) << 16 |
           (board_ & 0x000000000000ffff) << 48;
}

GameBoard GameBoard::get_mirror_board() const
{
  	return (board_ & 0xf000f000f000f000) >> 12 |
           (board_ & 0x0f000f000f000f00) >> 4 |
           (board_ & 0x00f000f000f000f0) << 4 |
           (board_ & 0x000f000f000f000f) << 12;
}

GameBoard GameBoard::get_flip_diagonal_3_12_board() const
{
  	return (board_ & 0x000f00f00f00f000) |
           (board_ & 0x00f00f00f0000000) >> 20 |
           (board_ & 0x0000000f00f00f00) << 20 |
           (board_ & 0x0f00f00000000000) >> 40 |
           (board_ & 0x00000000000f00f0) << 40 |
           (board_ & 0xf000000000000000) >> 60 |
           (board_ & 0x000000000000000f) << 60;
}

GameBoard GameBoard::get_flip_diagonal_0_15_board() const
{
  	return (board_ & 0xf0000f0000f0000f) |
           (board_ & 0x0f0000f0000f0000) >> 12 |
           (board_ & 0x0000f0000f0000f0) << 12 |
           (board_ & 0x00f0000f00000000) >> 24 |
           (board_ & 0x00000000f0000f00) << 24 |
           (board_ & 0x000f000000000000) >> 36 |
           (board_ & 0x000000000000f000) << 36;
}

