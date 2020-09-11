#include "temp_Node.h"
#include "temp_Cell.h"
#include <iostream>
#include <vector>

Node::Node(float xPos, float yPos, int pos_in_vec, bool isDeleted)
{
	static int id = 0; _seqNo = id++;

	_xPos = xPos;
	_yPos = yPos;
	_pos_in_vec = pos_in_vec;

	_isDeleted = isDeleted;

	//printf("node const %i\n", _seqNo);
}

Node::~Node(void) {
	//printf("node dest %i\n", _seqNo);
}

int Node::getSeqNo(void) { return _seqNo; }
float Node::getX(void) { return _xPos; }
float Node::getY(void) { return _yPos; }
int Node::get_pos_in_vec(void) { return _pos_in_vec; }
int Node::get_arc(int i) { return _connected_arcs.at(i); }
int Node::get_num_arcs() { return _connected_arcs.size(); }

std::pair<int, int> Node::get_cell(void)
{
	return _cell;
}
void Node::set_cell(int x, int y)
{
	_cell.first = x;
	_cell.second = y;
}

bool Node::isDeleted(void) { return _isDeleted; }
void Node::erase(void) { _isDeleted = true; }

void Node::add_arc(int arc_pos) { _connected_arcs.push_back(arc_pos); }
void Node::remove_arc(int arc_pos)
{
	for (int i = 0; i < _connected_arcs.size(); i++) {
		if (_connected_arcs.at(i) == arc_pos)
		{
			_connected_arcs.erase(_connected_arcs.begin() + i);
			i--;
		}
	}
}