#include "TupleNetwork.h"

TupleNetwork TupleNetwork::tuple_network_;
// TupleNetwork TupleNetwork::tuple_network_2;
// TupleNetwork TupleNetwork::tuple_network_3;
// TupleNetwork TupleNetwork::tuple_network_4;
// TupleNetwork TupleNetwork::tuple_network_5;

TupleNetwork::TupleNetwork():
tuples(NULL)
{
	TupleContainer *tuple_collection = new TupleContainer();
	tuple_collection->add_tuple(create_isomorphic_tuple_collection());
	tuple_collection->add_tuple(new LargeTileFeature());
	tuple_collection->add_tuple(new MergeFeature());
	tuples = tuple_collection;
}

TupleNetwork::~TupleNetwork()
{
	delete tuples;
}

Tuple* TupleNetwork::create_isomorphic_tuple_collection() const
{
	IsomorphicTupleContainer *isomorphic_tuple_collection = new IsomorphicTupleContainer();
	isomorphic_tuple_collection->add_tuple(new SquareTupleOutside());
	isomorphic_tuple_collection->add_tuple(new SquareTupleInside());
	//isomorphic_tuple_collection->add_tuple(new LineTupleOutside());
	//isomorphic_tuple_collection->add_tuple(new LineTupleInside());
	isomorphic_tuple_collection->add_tuple(new SmallAxeTupleOutside());
	isomorphic_tuple_collection->add_tuple(new SmallAxeTupleInside());
	//isomorphic_tuple_collection->add_tuple(new SmallSawTupleOutside());
	//isomorphic_tuple_collection->add_tuple(new SmallSawTupleInside());
	return isomorphic_tuple_collection;
}

double TupleNetwork::get_board_value(const GameBoard& game_board) const
{
	//GameBoard masquerade_board = get_masquerade_board(game_board);
	//return tuples->query_tuple_value(masquerade_board);
	return tuples->query_tuple_value(game_board);
}

void TupleNetwork::update_board_value(const GameBoard& game_board, const double& value)
{
	//GameBoard masquerade_board = get_masquerade_board(game_board);
	//tuples->update_tuple_value(masquerade_board, value);
	tuples->update_tuple_value(game_board, value);
}

void TupleNetwork::save_tuple_network(const string& filename) const
{
	ofstream fout(filename.c_str(), ios::out | ios::binary);
	if(!fout) return;
	tuples->save_tuple(fout);
	fout.close();
}

void TupleNetwork::load_tuple_network(const string& filename)
{
	ifstream fin(filename.c_str(), ios::in | ios::binary);
	if(!fin) return;
	tuples->load_tuple(fin);
	fin.close();
}

GameBoard TupleNetwork::get_masquerade_board(const GameBoard& board) const
{
	int board_array[16];
	int sort_array[16];
	board_t temp_board = board.get_board();
	for(int i = 0;i < 16;i++) {
		board_array[i] = sort_array[i] = temp_board & 0xf;
		temp_board >>= 4;
	}

	qsort(sort_array, 16, sizeof(int), compare_tile);
	int first_bound_tile = sort_array[0];
	int second_bound_tile = first_bound_tile;
	bool is_more_second_bound_tile = false;
	for(int i = 1;i < 16;i++) {
		if(sort_array[i] == 0)
			break;
		if(sort_array[i] == first_bound_tile)
			break;
		if(sort_array[i] == first_bound_tile - 1) {
			second_bound_tile = first_bound_tile = sort_array[i];
			continue;
		}
		second_bound_tile = sort_array[i];
		if(i != 15 && sort_array[i + 1] == second_bound_tile)
			is_more_second_bound_tile = true;
		break;
	}
	// difference between first and second bound will larger equal than 2
	int delta = first_bound_tile - second_bound_tile - 1;
	if(is_more_second_bound_tile)
		delta -= 1;
	if(delta > 0) {
		board_t new_board = 0;
		for(int i = 15;i >= 0;i--) {
			new_board <<= 4;
			if(board_array[i] >= first_bound_tile)
				board_array[i] -= delta;
			new_board |= board_array[i];
		}
		return GameBoard(new_board);
	}
	return board;
}

int TupleNetwork::compare_tile (const void *a, const void *b)
{
  return -1 * (*(int*)a - *(int*)b);
}

