#ifndef __TUPLECOLLECTION_H__
#define __TUPLECOLLECTION_H__

#include "FourTuple.h"
#include "SixTuple.h"

////////////////////////
// line-tuple outside
// O O O O	
// O O O O
// O O O O
// X X X X
class LineTupleOutside : public FourTuple
{
public:
	LineTupleOutside(double weight=1):FourTuple(weight) { }

protected:
	board_t get_index(const board_t& game_board) const 
	{
		return game_board & 0xffff;
	}
};

////////////////////////
// line-tuple inside
// O O O O	
// O O O O
// X X X X
// O O O O
class LineTupleInside : public FourTuple
{
public:
	LineTupleInside(double weight=1):FourTuple(weight) { }

protected:
	board_t get_index(const board_t& game_board) const 
	{
		return (game_board & 0xffff0000) >> 16;
	}
};

////////////////////////
// square-tuple outside
// O O O O	
// O O O O
// O X X X
// O X X X
class SquareTupleOutside : public SixTuple
{
public:
	SquareTupleOutside(double weight=1):SixTuple(weight) { }

protected:
	board_t get_index(const board_t& game_board) const 
	{
		return (game_board & 0xfff0000) >> 4 | (game_board & 0xfff);
	}
};

////////////////////////
// square-tuple inside
// O O O O	
// O X X X
// O X X X
// O O O O
class SquareTupleInside : public SixTuple
{
public:
	SquareTupleInside(double weight=1):SixTuple(weight) { }

protected:
	board_t get_index(const board_t& game_board) const 
	{
		return (game_board & 0xfff00000000) >> 20 | (game_board & 0xfff0000) >> 16;
	}
};

////////////////////////
// small axe-tuple outside
// O O O O	
// O O O O
// O O X X
// X X X X
class SmallAxeTupleOutside : public SixTuple
{
public:
	SmallAxeTupleOutside(double weight=1):SixTuple(weight) { }

protected:
	board_t get_index(const board_t& game_board) const 
	{
		return game_board & 0xffffff;
	}
};

////////////////////////
// small axe-tuple inside
// O O O O	
// O O X X
// X X X X
// O O O O
class SmallAxeTupleInside : public SixTuple
{
public:
	SmallAxeTupleInside(double weight=1):SixTuple(weight) { }

protected:
	board_t get_index(const board_t& game_board) const 
	{
		return (game_board & 0xffffff0000) >> 16;
	}
};

////////////////////////
// small saw-tuple outside
// O O O O	
// O O O O
// X X X X
// O O X X
class SmallSawTupleOutside : public SixTuple
{
public:
	SmallSawTupleOutside(double weight=1):SixTuple(weight) { }

protected:
	board_t get_index(const board_t& game_board) const 
	{
		return (game_board & 0xff) | (game_board & 0xffff0000) >> 8;
	}
};

////////////////////////
// small saw-tuple inside
// O O O O	
// X X X X
// O O X X
// O O O O
class SmallSawTupleInside : public SixTuple
{
public:
	SmallSawTupleInside(double weight=1):SixTuple(weight) { }

protected:
	board_t get_index(const board_t& game_board) const 
	{
		return (game_board & 0xff0000) >> 16 | (game_board & 0xffff00000000) >> 24;
	}
};


#endif
