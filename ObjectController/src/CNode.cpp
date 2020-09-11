#include "CSNode.h"

// getters
cnode_pos CSNode::get_vec_pos() const { return m_vec_pos; }
bool CSNode::is_valid(unsigned char prefab_id)
{
	return _valid.at(prefab_id);
}
float CSNode::get_potential(char object_id)
{
	return _potentials.at(object_id);
}
const std::set<int>* CSNode::get_tnode_ids() { return &m_tnode_ids; }

x_y<float> CSNode::get_position() const { return m_position; }
float CSNode::get_theta()
{
	return fLAYER_GAP * (int)m_vec_pos.orientation;
}
int CSNode::get_floor_num() const { return m_floor; }

// modify id
void CSNode::change_orient(Orient o)
{
	int orient_change = (int)o - (int)m_vec_pos.orientation;

	m_vec_pos.orientation = o;

	for (std::vector<CSArc>::iterator it = _arcs.begin(); it != _arcs.end(); ++it)
	{
		it->cnode_to_vec_pos.orientation = o;
		it->orient_f = orient_to_angle(o);

		int holo = it->holo.first - orient_change;
		while (holo < 2 - iNUM_HOLO)
		{
			holo += 2 * (iNUM_HOLO - 1);
		}
		while (holo >= iNUM_HOLO)
		{
			holo -= 2 * (iNUM_HOLO - 1);
		}
		int holo2 = it->holo.second - orient_change;
		while (holo2 < 2 - iNUM_HOLO)
		{
			holo2 += 2 * (iNUM_HOLO - 1);
		}
		while (holo2 >= iNUM_HOLO)
		{
			holo2 -= 2 * (iNUM_HOLO - 1);
		}
		it->holo = std::pair<int,int>(holo, holo2);

		int inv_holo = it->inv_holo.first - orient_change;
		while (inv_holo < 2 - iNUM_HOLO)
		{
			inv_holo += 2 * (iNUM_HOLO - 1);
		}
		while (inv_holo >= iNUM_HOLO)
		{
			inv_holo -= 2 * (iNUM_HOLO - 1);
		}
		int inv_holo2 = it->inv_holo.second - orient_change;
		while (inv_holo2 < 2 - iNUM_HOLO)
		{
			inv_holo2 += 2 * (iNUM_HOLO - 1);
		}
		while (inv_holo2 >= iNUM_HOLO)
		{
			inv_holo2 -= 2 * (iNUM_HOLO - 1);
		}
		it->inv_holo = std::pair<int, int>(inv_holo, inv_holo2);
		


	}

	
}
void CSNode::connect_above_below()
{
	int size = _arcs.size();
	for (int i = 0; i < size; ++i)
	{
		_arcs.push_back(CSArc(&_arcs[i], true));
		_arcs.push_back(CSArc(&_arcs[i], false));
	}
	_arcs.push_back(CSArc(m_vec_pos, m_position, m_floor, true));
	_arcs.push_back(CSArc(m_vec_pos, m_position, m_floor, false));
}

CSNode::CSNode(cnode_pos vector_position, node tnode) :
	m_vec_pos(vector_position),
	m_position(tnode.position),
	m_floor(tnode.floor_num),
	fuse(false),
	divider(false),
	_stair_id(tnode.stair_id),
	landing_id(tnode.landing_id)
{
	m_tnode_ids.insert(tnode.id);
	_arcs.reserve(51);
}

CSNode::CSNode(cnode_pos vector_position, std::set<int>& tnode_ids, x_y<float> pos, int floor, bool fuse, bool divider, int stair_id, int landing_id) :
	m_vec_pos(vector_position),
	m_tnode_ids(tnode_ids),
	m_position(pos),
	m_floor(floor),
	fuse(fuse),
	divider(divider),
	_stair_id(stair_id),
	landing_id(landing_id)
{
	_arcs.reserve(51);
}
CSNode::CSNode()
{
	_arcs.reserve(51);
}

CSNode::~CSNode(){}