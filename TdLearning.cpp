#include "TdLearning.h"
#define SAVETUPLEROUND 1000000

TdLearning::TdLearning(bool is_learning_mode, double learning_rate):
is_learning_mode_(is_learning_mode),
	learning_rate_(learning_rate),
	win_count_(0),
	max_tile_(0),
	max_score_(0),
	max_score_normalize_(0),
	average_score_(0.0),
	max_tile_count_(),
	expecti_max_(EXPECTIMAX_SEARCH_DEPTH)
{
}

TdLearning::~TdLearning()
{
}


void TdLearning::start_play_game(int total_rounds, string tuple_network_filename)
{
	max_tile_ = 0;
	max_score_ = 0;
	max_score_normalize_ = 0;
	average_score_ = 0;
	int count = 0;

	
	stringstream ss;
	ss << total_rounds;
	
	TupleNetwork::tuple_network_.load_tuple_network( (tuple_network_filename + ss.str() + ".tuple") );
	
	//-------chuan mark, split training set start board-------
	// GameBoard* Start_board= new GameBoard[110000];
	// double* Start_score= new double[110000];
 	// ifstream fin1("16_8k.board");
 	// fin1.read((char*)Start_board, sizeof(GameBoard) * 110000);
	// ifstream fin2("16_8k.score");
 	// fin2.read((char*)Start_score, sizeof(double) * 110000);	
	//-------chuan mark, split training set start board end-------
	
	
	//-------chuan mark, split training collect board-------
	GameBoard* Collection_board= new GameBoard[110000];
	double* Collection_score= new double[110000];
	int board_counts_collection = 0;
	//-------chuan mark, split training collect board end-------
		
	
	double previous_average_score = 0;
	int average_collection_counting = 0;

	for(int i = 1;i <= total_rounds;i++) {

		//int score = play_game(GameBoard(0), 0);
		
		
		//-------chuan mark, first stage training-------
		GameBoard game_board(0);
		int game_score = 0;	
		game_board.initialize();
		//-------chuan mark, first stage training end-------
		

		//-------chuan mark, split training set start board-------
		// int board_index = i%100000;
		// game_board = Start_board[board_index];
		// game_score = Start_score[board_index];
		//-------chuan mark, split training set start board end-------
		
				
		vector<BoardMoveRecord> move_records;
		BoardMoveRecord board_move_record;
		
		while(game_board.is_finish() == false) {
			MoveDirection best_move = find_best_move(game_board);
			bool is_legal_move = true;
			game_score += game_board.move(best_move, is_legal_move);
			board_move_record.board_ = game_board;
			board_move_record.score_ = game_score;
			move_records.push_back(board_move_record);
			game_board.add_random_tile();

			
			//-------chuan mark, split training collect board-------
			// if(game_board.has_16384_and_8192() && !game_board.is_finish())
			// {
				// Collection_board[board_counts_collection] = game_board;
				// Collection_score[board_counts_collection++] = game_score;		
				// break;
			// }
			//-------chuan mark, split training collect board end-------
			
		}

		// previous_average_score += game_score;
		// average_collection_counting++;
		// if(average_collection_counting>=10000)
		// {
			// cout << "threshold of 16+8k" << endl;
			// cout<<"average score in "<<average_collection_counting<<" is "<<previous_average_score/average_collection_counting<<endl;
			// break;
		// }
		
		
		if(is_learning_mode_)
			learning(move_records);
		int max_tile = game_board.get_max_tile();
		if(max_tile >= 2048)
			win_count_++;
		if(max_tile == 16384)
			max_tile = game_board.get_max_tile_greater_than_16384();
		if(max_tile > max_tile_)
			max_tile_ = max_tile;
		if(max_tile_count_.find(max_tile) == max_tile_count_.end())
			max_tile_count_[max_tile] = 0;
		max_tile_count_[max_tile]++;
		int score = game_score;

		//----------------------play_game() end------------------------------------


		if(score > max_score_)
			max_score_ = score;
		
		int normalize_round = 138;
		if(i%1000 < normalize_round && score > max_score_normalize_)
			max_score_normalize_ = score;

		count++;
		average_score_ = (average_score_ * (count - 1) + score) / (double)count;

		if(i % 1000 == 0 || i == total_rounds) {
			double win_rate = 100 * win_count_ / (double)count;
			show_detail(i, win_rate);
			show_max_tile_count(count);
			
			// cout<<"current collection number is "<<average_collection_counting<<endl;
			// cout<<"16+8k boards count is "<<board_counts_collection<<endl;

			cout << "------------------------------\n";
			max_score_ = 0;
			max_score_normalize_ = 0;
			max_tile_ = 0;
		}
		if(i % 1000 == 0 || i == total_rounds) {
			win_count_ = 0;
			count = 0;
			max_tile_count_.clear();
		}
		
		
		if( (i!=0) && (i % SAVETUPLEROUND ==0) && is_learning_mode_) {
			stringstream ss;
			ss << i;
			TupleNetwork::tuple_network_.save_tuple_network(tuple_network_filename + ss.str() + ".tuple" );
		}		
		
		// if((board_counts_collection>=100000) || (i == total_rounds) )
		// {
			// ofstream fout1("16_8k.board");
			// fout1.write((char*)Collection_board, sizeof(GameBoard) * 110000);	
			// ofstream fout2("16_8k.score");
			// fout2.write((char*)Collection_score, sizeof(double) * 110000);
			// break;
		// }
	}	
}

void TdLearning::show_detail(int rounds, double win_rate)
{
	  //-------chuan mark, training log name-------
	  ofstream outputFile("detail_TD0_merge_dropLR_1000w.txt",ios::out|ios::app);
	  //-------chuan mark, training date log name end-------
	  
	  outputFile<<win_rate<<"\t"<<average_score_<<"\t"<<max_score_<<"\t"<<max_score_normalize_<<endl;

	cout << "Rounds : " << rounds << "\tWin rate : " << win_rate << "%\tAvg. score : " << average_score_;
	cout << "\tMax score : " << max_score_ << "\tMax_score_normalize : " << max_score_normalize_ << "\tMax tile : ";
	
	
	if(max_tile_ >= 16384 && max_tile_ < 32768) {
		cout << "16384";
		int each_tiles = max_tile_ - 16384;
		for(int i = 0;i < 4;i++) {
			if(each_tiles == 0)
				break;
			cout << "+" << ((0x1 << (13 - i)) / 1000) << "k";
			each_tiles -= (0x1 << (13 - i));
		}
		cout << endl;
	}
	else
		cout << max_tile_ << endl;
}

void TdLearning::show_max_tile_count(int rounds)
{	
	//-------chuan mark, training rate log name-------
	ofstream outputFile_tile_rate("rate_TD0_merge_dropLR_1000w.txt",ios::out|ios::app);
	//-------chuan mark, training rate log name end-------
	
	
	// ofstream outputFile_tile_rate("garbage.txt",ios::out|ios::app);
	
	int accumulated_count = 0;
	int win_count = 0;
	for(map<int, int>::reverse_iterator rit = max_tile_count_.rbegin(); rit != max_tile_count_.rend(); ++rit) {
		accumulated_count += rit->second;
		double accumulated_rate = 100 * accumulated_count / static_cast<double>(rounds);
		double max_tile_rate = 100 *  rit->second / static_cast<double>(rounds);
		if(rit->first >= 16384 && rit->first < 32768) {
			cout << "16384";
			outputFile_tile_rate << "16384";
			
			int each_tiles = rit->first - 16384;
			for(int i = 0;i < 4;i++) {
				if(each_tiles == 0)
					break;
				cout << "+" << ((0x1 << (13 - i)) / 1000) << "k";
				outputFile_tile_rate << "+" << ((0x1 << (13 - i)) / 1000) << "k";
				each_tiles -= (0x1 << (13 - i));
			}
			cout << ": ";
			outputFile_tile_rate << ": ";
		}
		else
		{
			cout << rit->first << ": ";
			outputFile_tile_rate << rit->first << ": ";
		}
		cout << rit->second << " (" << max_tile_rate << "%)\taccumulate: " << accumulated_rate << "%\n";
		outputFile_tile_rate << rit->second << " (" << max_tile_rate << "%)\taccumulate: " << accumulated_rate << "%\t";
		if(rit->first >= 2048)
			win_count += rit->second;
	}
	double win_rate = 100 * win_count / static_cast<double>(rounds);
	cout << "Win Rate: " << win_rate << "%\n";
	outputFile_tile_rate << "Win Rate: " << win_rate << "%\n";
}

MoveDirection TdLearning::find_best_move(GameBoard board)
{
	MoveDirection successors[4];
	int successor_count = board.get_successor(successors);
	MoveDirection best_direction;
	double best_td_value = 0.0;
	bool is_first_direction = true;
	MoveDirection best_direction_with_dead;
	double best_td_value_with_dead = 0.0;
	bool is_first_direction_with_dead = true;
	for(int i = 0;i < successor_count;i++) {
		double td_value = evaluate(board, successors[i]);
		GameBoard after_move_board = board;
		bool is_legal_move;
		after_move_board.move(successors[i], is_legal_move);
		bool possible_dead = after_move_board.is_possible_dead();
		if(possible_dead) {
			if(is_first_direction_with_dead || td_value > best_td_value_with_dead) {
				best_direction_with_dead = successors[i];
				best_td_value_with_dead = td_value;
				is_first_direction_with_dead = false;
			}
			continue;
		}
		if(is_first_direction || td_value > best_td_value) {
			best_direction = successors[i];
			best_td_value = td_value;
			is_first_direction = false;
		}
	}
	if(is_first_direction) {
		best_direction = best_direction_with_dead;
	}
	return best_direction;
}

double TdLearning::evaluate(GameBoard board, MoveDirection direction)
{
	bool is_legal_move = false;
	GameBoard after_move_board = board;
	int reward = after_move_board.move(direction, is_legal_move);
	return reward + TupleNetwork::tuple_network_.get_board_value(after_move_board);
}

void TdLearning::learning(vector<BoardMoveRecord> move_records)
{
	BoardMoveRecord current_record;
	BoardMoveRecord next_record;
	BoardMoveRecord last_record;
	current_record = next_record = last_record = move_records.back();
	
	
	//-------chuan mark, switch 1-------
	//normal td(0)
	while(move_records.empty() != true) {
		current_record = move_records.back();
		move_records.pop_back();
		learn_evaluation(current_record, next_record, last_record);		
		last_record = next_record;
		next_record = current_record;
	}	
		
	//td lamda
	// double *nextFiveStateScore = new double[5];
	// double *nextFiveR = new double[5];	
	// int backwardMoveCount = 0;
	// int latestIndex = move_records.size() - 1;
	// for(int i=0;i<5;i++)
		// nextFiveStateScore[i] = 0;
	// while(latestIndex>=0) {
		// current_record = move_records[latestIndex];
		// if(backwardMoveCount>0)
		// {	
		    // nextFiveR[0] = move_records[latestIndex+1].score_ - current_record.score_;
			// nextFiveStateScore[0] = TupleNetwork::tuple_network_.get_board_value(move_records[latestIndex+1].board_) + nextFiveR[0];
		// }
		// if(backwardMoveCount>1)
		// {
			// nextFiveR[1] = move_records[latestIndex+2].score_ - move_records[latestIndex+1].score_;
			// nextFiveStateScore[1] = TupleNetwork::tuple_network_.get_board_value(move_records[latestIndex+2].board_) + nextFiveR[0] + nextFiveR[1];	
		// }			
		// if(backwardMoveCount>2)			
		// {
			// nextFiveR[2] = move_records[latestIndex+3].score_ - move_records[latestIndex+2].score_;
			// nextFiveStateScore[2] = TupleNetwork::tuple_network_.get_board_value(move_records[latestIndex+3].board_) + nextFiveR[0] + nextFiveR[1] + nextFiveR[2];
		// }			
		// if(backwardMoveCount>3)
		// {
			// nextFiveR[3] = move_records[latestIndex+4].score_ - move_records[latestIndex+3].score_;
			// nextFiveStateScore[3] = TupleNetwork::tuple_network_.get_board_value(move_records[latestIndex+4].board_) + nextFiveR[0] + nextFiveR[1] + nextFiveR[2] + nextFiveR[3];
		// }			
		// if(backwardMoveCount>4)
		// {
			// nextFiveR[4] = move_records[latestIndex+5].score_ - move_records[latestIndex+4].score_;
			// nextFiveStateScore[4] = TupleNetwork::tuple_network_.get_board_value(move_records[latestIndex+5].board_) + nextFiveR[0] + nextFiveR[1] + nextFiveR[2] + nextFiveR[3] + nextFiveR[4];
		// }					
		// learn_evaluation_td_lamda(current_record,nextFiveStateScore);		
		// backwardMoveCount++;
		// latestIndex--;
	// }
	// move_records.clear();
	// delete [] nextFiveStateScore;
	// delete [] nextFiveR;
	
	//-------chuan mark, switch 1-------
}	

void TdLearning::learn_evaluation(BoardMoveRecord current_record, BoardMoveRecord next_record, BoardMoveRecord last_record)
{ 	
	double current_board_value = TupleNetwork::tuple_network_.get_board_value(current_record.board_);
	double next_board_value = TupleNetwork::tuple_network_.get_board_value(next_record.board_);
	//double last_board_value = TupleNetwork::tuple_network_.get_board_value(last_record.board_);

	double next_reward = static_cast<double>(next_record.score_ - current_record.score_); 
	//double last_reward = static_cast<double>(last_record.score_ - current_record.score_); 

	double expected_reward;
	if(current_record == next_record)
		expected_reward = 0;
	else
		expected_reward = next_reward + next_board_value;
	//  	else if(next_record == last_record)
	//  		expected_reward = next_reward + next_board_value;
	//  	else
	// 		expected_reward = 0.8 * (next_reward + next_board_value) + 0.2 * (last_reward + last_board_value);

	double difference = learning_rate_ * (expected_reward - current_board_value);
	TupleNetwork::tuple_network_.update_board_value(current_record.board_, difference);
}

void TdLearning::learn_evaluation_td_lamda(BoardMoveRecord current_record, double *nextFiveBoardScore)
{ 	
	double current_board_value = TupleNetwork::tuple_network_.get_board_value(current_record.board_);
	//double next_board_value = TupleNetwork::tuple_network_.get_board_value(next_record.board_);
	//double last_board_value = TupleNetwork::tuple_network_.get_board_value(last_record.board_);

	double expected_reward = 0.5 * nextFiveBoardScore[0] + 0.25 * nextFiveBoardScore[1] + 0.125 * nextFiveBoardScore[2] +
							 0.0625 * nextFiveBoardScore[3] + 0.0625 * nextFiveBoardScore[4];	

	double difference = learning_rate_ * (expected_reward - current_board_value);
	TupleNetwork::tuple_network_.update_board_value(current_record.board_, difference);
}


void TdLearning::load_initialized_board(string filename, vector< pair<GameBoard, int> >& initialized_board)
{
	ifstream fin(filename.c_str(), ios::in | ios::binary);
	initialized_board.clear();
	int start_score = 0;
	board_t start_board = 0;
	while(!fin.eof()) {
		fin >> start_score >> start_board;
		initialized_board.push_back(pair<GameBoard, int>(start_board, start_score));	
	}
	fin.close();
}
