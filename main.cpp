#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include "GameBoard.h"
#include "ExpectiMax.h"
#include "TupleNetwork/TupleNetwork.h"

using namespace std;

int play_game(GameBoard& game_board, ExpectiMax& expecti_max_search);
void show_average_result(int rounds, int total_score, map<int, int> max_tile_count);

int main_temp()
{
// 	if(argc < 5) {
// 		cerr << "Usage: ./main <rounds> <search depth> <tuple network less than 16+8> <tuple network larger than 16+8> \n";
// 		return -1;
// 	}

	int rounds = 1000;
	int search_depth = 5;
	string tuple_network_filename = "tuple_saw_merge_feature_400w";
	TupleNetwork::tuple_network_.load_tuple_network(tuple_network_filename);
	//tuple_network_filename = argv[4];
	//TupleNetwork::tuple_network_16k_8k_.load_tuple_network(tuple_network_filename);
	ExpectiMax expecti_max_search(search_depth);

	//srand(static_cast<unsigned int>(time(NULL)));
	srand(0);

	bool use_random_seeds = true;
	unsigned int random_seed = 0;
	// set random seed of each round
	vector<unsigned int> random_seeds;
	if(use_random_seeds) {
		for(int i = 0;i < rounds;i++)
			random_seeds.push_back(rand());
	}

	int total_score = 0;
	int max_score = 0;
	map<int, int> max_tile_count;

	for(int i = 1;i <= rounds;i++) {
		// set random seed of each game
		if(use_random_seeds) {
			srand(random_seeds[i - 1]);
			//cout << "first random num: " << rand() << endl;
			cout << "Random seed: " << random_seeds[i - 1] << endl;
		}
		else {
			srand(random_seed);
			//cout << "first random num: " << rand() << endl;
			cout << "Random seed: " << random_seed << endl;
		}

		// initialize game board
		GameBoard game_board(0);
		game_board.initialize();
		ZHashTable::z_hash_table.clear();
		//game_board.show_board();

		int score = play_game(game_board, expecti_max_search);
		game_board.show_board();

		int max_tile = game_board.get_max_tile();
		cout << "Rounds: " << i << "\tScore: " << score << "\tMax-tile: " << max_tile << endl;

		total_score += score;
		if(score > max_score)
			max_score = score;
		if(max_tile == 16384)
			max_tile = game_board.get_max_tile_greater_than_16384();
		if(max_tile_count.find(max_tile) == max_tile_count.end())
			max_tile_count[max_tile] = 0;
		max_tile_count[max_tile]++;

		if(i % 100 == 0 && i != rounds) {
			cout << "============================\n";
			cout << "Accumulated rounds: " << i << endl;
			cout << "Max score: " << max_score << endl;
			show_average_result(i, total_score, max_tile_count);
		}
		cout << "----------------------------\n";
	}
	cout << "============================\n";
	cout << "Total rounds: " << rounds << endl;
	cout << "Max score: " << max_score << endl;
	show_average_result(rounds, total_score, max_tile_count);

	return 0;
}

int play_game(GameBoard& game_board, ExpectiMax& expecti_max_search)
{
	clock_t run_time = -clock();
	int move_count = 0;
	int score = 0;
	while(game_board.is_finish() == false) {
		// ExpectiMax algorithm
		MoveDirection best_move = expecti_max_search.find_best_move(game_board);

		bool is_legal_move = true;
		score += game_board.move(best_move, is_legal_move);
		move_count++;
		if(!is_legal_move)
			cout << "Not legal move!\n";
		else
			game_board.add_random_tile();
		//cout << "----------------------------\n";
		//game_board.show_board();
	}
	cout << "Moves: " << move_count << endl;
	run_time += clock();
	cout << "Speed: " << move_count / (double)run_time * CLOCKS_PER_SEC << " (moves/sec)\n";
	return score;
}

void show_average_result(int rounds, int total_score, map<int, int> max_tile_count)
{
	double average_score = total_score / static_cast<double>(rounds);
	cout << "Average score: " << average_score << endl;

	int accumulated_count = 0;
	int win_count = 0;
	for(map<int, int>::reverse_iterator rit = max_tile_count.rbegin(); rit != max_tile_count.rend(); ++rit) {
		accumulated_count += rit->second;
		double accumulated_rate = 100 * accumulated_count / static_cast<double>(rounds);
		double max_tile_rate = 100 *  rit->second / static_cast<double>(rounds);
		if(rit->first >= 16384 && rit->first < 32768) {
			cout << "16384";
			int each_tiles = rit->first - 16384;
			for(int i = 0;i < 4;i++) {
				if(each_tiles == 0)
					break;
				cout << "+" << ((0x1 << (13 - i)) / 1000) << "k";
				each_tiles -= (0x1 << (13 - i));
			}
			cout << ": ";
		}
		else
			cout << rit->first << ": ";
		cout << rit->second << " (" << max_tile_rate << "%)\taccumulate: " << accumulated_rate << "%\n";
		if(rit->first >= 2048)
			win_count += rit->second;
	}
	double win_rate = 100 * win_count / static_cast<double>(rounds);
	cout << "Win Rate: " << win_rate << "%\n";
}
