#include "temp_Arc1.h"
#include "temp_Node.h"
#include "temp_Cell.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>


Arc1::Arc1(Node* start_node, Node* end_node, int pos_in_vector, bool isDeleted)
{
	static int id = 0; _seqNo = id++;

	_pos_in_vector = pos_in_vector;

	_node1pos = start_node->get_pos_in_vec();
	_node2pos = end_node->get_pos_in_vec();

	_x1 = start_node->getX();
	_x2 = end_node->getX();
	_y1 = start_node->getY();
	_y2 = end_node->getY();

	_midx = (_x1 + _x2) / 2.0f;
	_midy = (_y1 + _y2) / 2.0f;

	float xdiff = _x1 - _x2;
	float ydiff = _y1 - _y2;

	_length = sqrt((xdiff*xdiff) + (ydiff*ydiff));

	_angle = ydiff == 0.0f ? 90.0f : atan(xdiff / ydiff)*(180 / M_PI);
	if (_angle < -180) { _angle += 360; }
	else if (_angle < 0) { _angle += 180; }
	else if (_angle >= 180) { _angle -= 180; }
	_angle = abs(_angle);

	_isDeleted = isDeleted;

	//printf("arc const %i\n", _seqNo);
}

Arc1::~Arc1(void)
{
	//printf("arc dest %i\n", _seqNo);
}

int Arc1::getSeqNo(void) { return _seqNo; }
int Arc1::getNode(int i) {
	if (i == 0) return _node1pos;
	return _node2pos;
}
int Arc1::get_pos_in_vector() { return _pos_in_vector; }
float Arc1::get_x1(void) { return _x1; }
float Arc1::get_x2(void) { return _x2; }
float Arc1::get_y1(void) { return _y1; }
float Arc1::get_y2(void) { return _y2; }
float Arc1::get_midx(void) { return _midx; }
float Arc1::get_midy(void) { return _midy; }

std::pair<int, int> Arc1::get_cell(void)
{
	return _cell;
}
void Arc1::set_cell(int x, int y)
{
	_cell.first = x;
	_cell.second = y;
}

bool Arc1::isDeleted(void) { return _isDeleted; }
void Arc1::erase(void) { _isDeleted = true; }
