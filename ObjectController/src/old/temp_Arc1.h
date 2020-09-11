#pragma once
#include "temp_Node.h"
#include "temp_Cell.h"
#include <math.h>
#include <vector>

class Arc1
{
	int _seqNo;

	int _node1pos;
	int _node2pos;

	float _length;
	float _angle;

	float _x1;
	float _x2;
	float _y1;
	float _y2;
	float _midx;
	float _midy;

	std::pair<int, int> _cell;

	int _pos_in_vector;

	bool _isDeleted;

	Arc1(void);

public:
	int getSeqNo(void);
	int getNode(int);
	int get_pos_in_vector(void);
	float get_x1(void);
	float get_x2(void);
	float get_y1(void);
	float get_y2(void);
	float get_midx(void);
	float get_midy(void);

	std::pair<int, int> get_cell(void);
	void set_cell(int, int);

	bool isDeleted(void);
	void erase(void);

	Arc1(Node*, Node*, int, bool);
	~Arc1(void);
};

