#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>
#include "GameBoard.h"
#include "TdLearning.h"

using namespace std;

int main()
{
// 	if(argc < 2) {
// 		cerr << "Usage: ./learning <rounds> <tuple network filename>\n";
// 		return -1;
// 	}

	unsigned int random_seed = static_cast<unsigned int>(time(NULL));
	//unsigned int random_seed = 0;
	ofstream seedFp("TD0_merge_dropLR_1000w.txt",ios::out|ios::app);
	seedFp << random_seed << endl;
	seedFp.close();
	
	srand(random_seed);

	int test;	
	
	//-------chuan mark, switch 2-------
	int rounds = 5000000;
	string tuple_network_file = "";
	tuple_network_file = "TD0_merge_dropLR_1000w";
	TdLearning td_learning(true, 0.0025);
	//-------chuan mark, switch 2 end-------
	
	td_learning.start_play_game(rounds, tuple_network_file);
	cout<<"VER : lambda.tuple"<<endl;
	// cout << "count average 16+8k threshold" << endl;
	// cout<<"VER : collection: 16+8k_board"<<endl;

	cin >> test;

	return 0;
}
