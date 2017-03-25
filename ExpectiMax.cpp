#include "ExpectiMax.h"

ExpectiMax::ExpectiMax(int search_depth):
search_depth_(search_depth)
{
}


MoveDirection ExpectiMax::find_best_move( GameBoard board)
{
	MoveDirection best_successor;
	bool has_dead_board = false;
	get_best_successor_and_score(board, search_depth_, best_successor, has_dead_board);
	return best_successor;
}

double ExpectiMax::get_best_successor_and_score( GameBoard board, int depth, MoveDirection& best_successor, bool& has_dead_board )
{
	MoveDirection successors[4];
	int successor_count = board.get_successor(successors);
	double best_evaluation_score = 0.0;
	bool is_first_direction = true;
	// handle child node has dead board
	MoveDirection best_successor_with_dead;
	double best_evaluation_score_with_dead = 0.0;
	bool is_first_direction_with_dead = true;

	for(int i = 0;i < successor_count;i++) {
		GameBoard next_board = board;
		bool is_legal_move;
		int reward = next_board.move(successors[i], is_legal_move);
		bool next_tern_has_dead_board = false;
		double evaluation_score = reward + expecti_max_random_turn(next_board, depth - 1, next_tern_has_dead_board);
		if(next_tern_has_dead_board == true) {    
			if(is_first_direction_with_dead || evaluation_score > best_evaluation_score_with_dead) {
				best_evaluation_score_with_dead = evaluation_score;
				best_successor_with_dead = successors[i];
				is_first_direction_with_dead = false;
			}
			continue;
		}
		if(is_first_direction || evaluation_score > best_evaluation_score) {
		  best_evaluation_score = evaluation_score;
		  best_successor = successors[i];
		  is_first_direction = false;
		}
	}
	if(is_first_direction == true) { // all child has dead board
		best_evaluation_score = best_evaluation_score_with_dead;
		best_successor = best_successor_with_dead;
		has_dead_board = true;
	} 
	return best_evaluation_score;
}

double ExpectiMax::expecti_max_ai_turn( GameBoard board, int depth, bool& has_dead_board )
{
	if(board.is_finish()) {
		has_dead_board = true;
		return 0;
	}
	MoveDirection best_successor;
	return get_best_successor_and_score(board, depth, best_successor, has_dead_board);
}

double ExpectiMax::expecti_max_random_turn( GameBoard board, int depth, bool& has_dead_board )
{
	bool find_in_hash_table = false;
	double evaluate_value = ZHashTable::z_hash_table.get_value(board.get_board(), depth, has_dead_board, find_in_hash_table);
	if(find_in_hash_table)
		return evaluate_value;

	if(depth <= 0) {
		evaluate_value = evaluate(board);
		ZHashTable::z_hash_table.set_value(board.get_board(), depth, evaluate_value, has_dead_board);
		return evaluate_value;
	}
	GameBoard possible_boards[32];
	int possible_boards_count = board.get_all_possible_board(possible_boards);
	double total_evaluation_score = 0.0;
	double worst_score = 0;
	for(int i = 0;i < possible_boards_count;i++) {
		bool next_tern_has_dead_board = false;
		double evaluation_score = expecti_max_ai_turn(possible_boards[i], depth - 1, next_tern_has_dead_board);
		if(next_tern_has_dead_board == true)
			has_dead_board = true;
		if(total_evaluation_score == 0 || evaluation_score < worst_score)
			worst_score = evaluation_score;
		if(i < possible_boards_count / 2) // 4 tile
			total_evaluation_score += 0.2 * (evaluation_score / static_cast<double>(possible_boards_count));
		else
			total_evaluation_score += 1.8 * (evaluation_score / static_cast<double>(possible_boards_count));
		
		//total_evaluation_score += evaluation_score / static_cast<double>(possible_boards_count);
	}
	static double alpha = 0.95;
	total_evaluation_score = alpha * total_evaluation_score + (1.0 - alpha) * worst_score;

	ZHashTable::z_hash_table.set_value(board.get_board(), depth, total_evaluation_score, has_dead_board);
	return total_evaluation_score;
}

double ExpectiMax::evaluate( GameBoard board )
{
// 	if(board.has_16384_and_8192())
// 		return TupleNetwork::tuple_network_16k_8k_.get_board_value(board);
// 	else
		return TupleNetwork::tuple_network_.get_board_value(board);
}

