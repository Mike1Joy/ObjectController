#pragma once
#include "temp_Cell.h"
#include <vector>

class Node
{
	int _seqNo;
	int _pos_in_vec;
	float _xPos;
	float _yPos;

	std::pair<int, int> _cell;

	bool _isDeleted;

	std::vector<int> _connected_arcs;

	Node(void);
public:
	int getSeqNo(void);
	float getX(void);
	float getY(void);
	int get_pos_in_vec(void);
	int get_arc(int);
	int get_num_arcs(void);

	std::pair<int, int> get_cell(void);
	void set_cell(int, int);

	bool isDeleted(void);
	void erase(void);

	void add_arc(int);
	void remove_arc(int);

	Node(float, float, int, bool);
	~Node(void);
};
