#pragma once
#include <vector>
#include "temp_Node.h"
#include "temp_Arc1.h"
#include "temp_BoundaryLine.h"

class TSpace
{
	std::vector<Node> nodeList;
	std::vector<Arc1> Arc1List;
	std::vector<BoundaryLine> BoundList;
public:
	

	//Nodes
	void addNode(float, float);
	void addNode(float, float, int, bool);
	bool removeNode(int);

	Node* getNode_fromVecPos(int);
	int getNode_fromPos(float, float, float, int);
	int getNode_VecSize(void);
	std::vector<Node>::iterator get_nodes_start();
	std::vector<Node>::iterator get_nodes_end();

	//Arc1s
	void addArc1(Node*, Node*);
	void addArc1(int, int, int, bool);
	bool removeArc1(int);

	Arc1* getArc1_fromVecPos(int);
	int getArc_fromPos(float, float, float, int);
	int getArc1_VecSize(void);
	std::vector<Arc1>::iterator get_arcs_start();
	std::vector<Arc1>::iterator get_arcs_end();

	//other
	void makeNodeGrid(int, int, float, float, int, int, float, bool, bool, bool, bool);

	//Boundary
	void addBound(float, float, float, float);
	void addBound(float, float, float, float, bool);
	bool removeBound(int);

	int getBoundPos(int);
	BoundaryLine* getBound_fromVecPos(int);
	int getBoundVecPos_fromPos(float, float, float, int);
	int getBoundVecSize(void);
	std::vector<BoundaryLine>::iterator get_bounds_start();
	std::vector<BoundaryLine>::iterator get_bounds_end();

	//clear
	void clear();

	TSpace();
	~TSpace(void);
};

