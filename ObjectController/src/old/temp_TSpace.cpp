#include "temp_TSpace.h"
#include "temp_Node.h"
#include "temp_Arc1.h"
#include "temp_BoundaryLine.h"
#include <iostream>

//Node
void TSpace::addNode(float x, float y)
{
	int j = 0;
	while (j < nodeList.size())
	{
		if (nodeList.at(j).isDeleted())
		{
			nodeList.at(j) = Node(x, y, j, false);
			return;
		}
		j++;
	}

	int i = nodeList.size();
	nodeList.push_back(Node(x, y, i, false));
}
void TSpace::addNode(float x, float y, int i, bool del)
{
	nodeList.push_back(Node(x, y, i, del));
}
bool TSpace::removeNode(int pos_in_vector)
{
	//test if valid removal
	if (pos_in_vector < 0 || pos_in_vector >= nodeList.size()) return false;

	//erase all arcs joint to node
	while (nodeList.at(pos_in_vector).get_num_arcs() > 0)
	{
		removeArc1(nodeList.at(pos_in_vector).get_arc(0));
	}

	//erase node
	nodeList.at(pos_in_vector).erase();

	return true;
}
Node* TSpace::getNode_fromVecPos(int pos_in_vector)
{
	if (pos_in_vector < 0 || pos_in_vector >= nodeList.size())
	{
		return nullptr;
	}
	else
	{
		return &(nodeList.at(pos_in_vector));
	}
}
int TSpace::getNode_fromPos(float x, float y, float tolarence, int ignore)
{
	for (int i = nodeList.size() - 1; i >= 0; i--)
	{
		if (i != ignore && !nodeList.at(i).isDeleted()) {
			if (getNode_fromVecPos(i)->getX() > x - tolarence &&
				getNode_fromVecPos(i)->getX() < x + tolarence &&
				getNode_fromVecPos(i)->getY() > y - tolarence &&
				getNode_fromVecPos(i)->getY() < y + tolarence)
			{
				return i;
			}
		}
	}
	return -1;
}
int TSpace::getNode_VecSize(void)
{//
	return nodeList.size();
}
std::vector<Node>::iterator TSpace::get_nodes_start()
{
	return nodeList.begin();
}
std::vector<Node>::iterator TSpace::get_nodes_end()
{
	return nodeList.end();
}

//Arc1
void TSpace::addArc1(Node* start_node, Node* end_node)
{
	int j = 0;
	while (j < Arc1List.size())
	{
		if (Arc1List.at(j).isDeleted())
		{
			Arc1List.at(j) = Arc1(start_node, end_node, j, false);

			start_node->add_arc(j);
			end_node->add_arc(j);
			return;
		}
		j++;
	}

	int i = Arc1List.size();
	Arc1List.push_back(Arc1(start_node, end_node, i, false));

	start_node->add_arc(i);
	end_node->add_arc(i);
}
void TSpace::addArc1(int start_node, int end_node, int pos, bool del)
{
	Node* node1 = &(nodeList.at(start_node));
	Node* node2 = &(nodeList.at(end_node));

	Arc1List.push_back(Arc1(node1, node2, pos, del));

	node1->add_arc(pos);
	node2->add_arc(pos);
}
bool TSpace::removeArc1(int pos_in_vector)
{
	//test if valid removal
	if (pos_in_vector < 0 || pos_in_vector >= Arc1List.size()) { return false; }

	// remove arc from nodes
	getNode_fromVecPos(Arc1List.at(pos_in_vector).getNode(0))->remove_arc(pos_in_vector);
	getNode_fromVecPos(Arc1List.at(pos_in_vector).getNode(2))->remove_arc(pos_in_vector);

	// erase arc
	Arc1List.at(pos_in_vector).erase();

	return true;
}
Arc1* TSpace::getArc1_fromVecPos(int pos_in_vector)
{
	if (pos_in_vector < 0 || pos_in_vector >= Arc1List.size()) return nullptr;

	return &(Arc1List.at(pos_in_vector));
}
int TSpace::getArc_fromPos(float x, float y, float tolarence, int ignore)
{
	for (int i = Arc1List.size() - 1; i >= 0; i--)
	{
		if (i != ignore && !Arc1List.at(i).isDeleted()) {
			float arcx1 = getNode_fromVecPos(getArc1_fromVecPos(i)->getNode(0))->getX();
			float arcx2 = getNode_fromVecPos(getArc1_fromVecPos(i)->getNode(1))->getX();
			float arcy1 = getNode_fromVecPos(getArc1_fromVecPos(i)->getNode(0))->getY();
			float arcy2 = getNode_fromVecPos(getArc1_fromVecPos(i)->getNode(1))->getY();

			bool end1 = arcx1 > x - tolarence &&
				arcx1 < x + tolarence &&
				arcy1 > y - tolarence &&
				arcy1 < y + tolarence;

			bool end2 = arcx2 > x - tolarence &&
				arcx2 < x + tolarence &&
				arcy2 > y - tolarence &&
				arcy2 < y + tolarence;

			bool middle = (arcx1 + arcx2) / 2 > x - tolarence &&
				(arcx1 + arcx2) / 2 < x + tolarence &&
				(arcy1 + arcy2) / 2 > y - tolarence &&
				(arcy1 + arcy2) / 2 < y + tolarence;

			if (middle) return i;
		}
	}
	return -1;
}
int TSpace::getArc1_VecSize(void)
{
	return Arc1List.size();
}
std::vector<Arc1>::iterator TSpace::get_arcs_start()
{
	return Arc1List.begin();
}
std::vector<Arc1>::iterator TSpace::get_arcs_end()
{
	return Arc1List.end();
}

//Other
void TSpace::makeNodeGrid(int counterx, int countery, float startx, float starty, int w, int h, float spacing, bool northbound, bool eastbound, bool southbound, bool westbound)
{
	if (counterx == w)
	{
		if (countery + 1 == h) {
			float top = starty + (h - 1)*(spacing)+0.2f;
			float right = startx + (w - 1)*(spacing)+0.2f;
			float bottom = starty - 0.2f;
			float left = startx - 0.2f;

			if (northbound) addBound(left, top, right, top);
			if (eastbound) addBound(right, top, right, bottom);
			if (southbound) addBound(right, bottom, left, bottom);
			if (westbound) addBound(left, bottom, left, top);

			return;
		}
		counterx = 0;
		countery++;
	}

	addNode(startx + counterx * spacing, starty + countery * spacing);

	if (counterx > 0) { addArc1(&(nodeList.back()), &(nodeList.at(nodeList.size() - 2))); }
	if (countery > 0) { addArc1(&(nodeList.back()), &(nodeList.at(nodeList.size() - (w + 1)))); }
	if (counterx > 0 && countery > 0) { addArc1(&(nodeList.back()), &(nodeList.at(nodeList.size() - (w + 2)))); }
	if (counterx < (w - 1) && countery>0) { addArc1(&(nodeList.back()), &(nodeList.at(nodeList.size() - w))); }

	counterx++;
	makeNodeGrid(counterx, countery, startx, starty, w, h, spacing, northbound, eastbound, southbound, westbound);
}

//Boundary
void TSpace::addBound(float startX, float startY, float endX, float endY)
{
	for (int i = 0; i < BoundList.size(); i++)
	{
		if (BoundList.at(i).isDeleted())
		{
			BoundList.at(i) = BoundaryLine(startX, startY, endX, endY, false);
			return;
		}
	}

	BoundList.push_back(BoundaryLine(startX, startY, endX, endY, false));
}
void TSpace::addBound(float startX, float startY, float endX, float endY, bool del)
{
	BoundList.push_back(BoundaryLine(startX, startY, endX, endY, del));
}
bool TSpace::removeBound(int pos_in_vector)
{
	if (pos_in_vector < 0 || pos_in_vector >= BoundList.size()) return false;
	BoundList.erase(BoundList.begin() + pos_in_vector);
	return true;
}
int TSpace::getBoundPos(int seqNo)
{
	for (int i = 1; i < BoundList.size(); i++)
	{
		if (BoundList.at(i).getSeqNo() == seqNo) return i;
	}

	return -1;
}
BoundaryLine* TSpace::getBound_fromVecPos(int pos_in_vector)
{
	if (pos_in_vector < 1) return &(BoundList.front());
	if (pos_in_vector >= BoundList.size()) return &(BoundList.back());

	return &(BoundList.at(pos_in_vector));
}
int TSpace::getBoundVecSize(void)
{
	return BoundList.size();
}
int TSpace::getBoundVecPos_fromPos(float x, float y, float tolarence, int ignore)
{
	for (int i = BoundList.size() - 1; i >= 0; i--)
	{
		if (i != ignore) {
			float x1 = BoundList.at(i).getStartX();
			float x2 = BoundList.at(i).getEndX();
			float y1 = BoundList.at(i).getStartY();
			float y2 = BoundList.at(i).getEndY();

			bool end1 = x1 > x - tolarence &&
				x1 < x + tolarence &&
				y1 > y - tolarence &&
				y1 < y + tolarence;

			bool end2 = x2 > x - tolarence &&
				x2 < x + tolarence &&
				y2 > y - tolarence &&
				y2 < y + tolarence;

			bool middle = (x1 + x2) / 2 > x - tolarence &&
				(x1 + x2) / 2 < x + tolarence &&
				(y1 + y2) / 2 > y - tolarence &&
				(y1 + y2) / 2 < y + tolarence;

			if (middle) return i;
		}
	}
	return -1;
}
std::vector<BoundaryLine>::iterator TSpace::get_bounds_start()
{
	return BoundList.begin();
}
std::vector<BoundaryLine>::iterator TSpace::get_bounds_end()
{
	return BoundList.end();
}

//clear
void TSpace::clear()
{
	BoundList.clear();
	Arc1List.clear();
	nodeList.clear();
}

//Construct / destr
TSpace::TSpace()
{
	printf("TSpace const\n");
	printf("Max num nodes = %i\n", nodeList.max_size());
	printf("Max num arcs = %i\n", Arc1List.max_size());
	printf("Max num bounds = %i\n", BoundList.max_size());
}
TSpace::~TSpace(void) { printf("TSpace dest\n"); }
