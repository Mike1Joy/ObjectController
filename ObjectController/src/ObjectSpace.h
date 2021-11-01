#pragma once
#include "DataStructs.h"
#include "CSNode.h"
#include "GenObject.h"
#include <vector>
#include <array>
#include <math.h>
#include <random>
#include "time.h"
#include "Log.h"

using namespace ObjCont;

struct cspace
{
	// cspace
	std::vector<CSNode> _cspace[iNUM_ORIENT]; //_cspace[o][n] returns node at position n at orientation o

	void clear()
	{
		for (int i = 0; i < iNUM_ORIENT; ++i)
		{
			_cspace[i].clear();
		}
	}

	void reserve(int num_tnodes)
	{
		for (int i = iNUM_ORIENT - 1; i >= 0; --i)
		{
			_cspace[i].reserve(num_tnodes*4*iNUM_ORIENT);
		}
	}

	void shrink_to_fit()
	{
		for (int i = iNUM_ORIENT-1; i >= 0; --i)
		{
			_cspace[i].shrink_to_fit();
		}
	}

	int size(Orient o)
	{
		return _cspace[(int)o].size();
	}

	void push_back(cnode_pos pos, node n)
	{
		_cspace[pos.orientation].emplace_back(pos,n);
	}

	void push_back(cnode_pos vector_position, std::set<int>& tnode_ids, x_y<float> pos, int floor, bool fuse, bool divider, int stair_id, int landing_id)
	{
		_cspace[vector_position.orientation].emplace_back(vector_position, tnode_ids, pos, floor, fuse, divider, stair_id, landing_id);
	}

	CSNode* get_cnode_pt(cnode_pos pos)
	{
		if ( pos.vec_pos < size(pos.orientation) )
		{
			return &_cspace[pos.orientation][pos.vec_pos];
		}
		else
		{
			return nullptr;
		}
	}

	CSNode* get_cnode_pt(Orient orientation, int position)
	{
		if (position < size(orientation))
		{
			return &_cspace[orientation][position];
		}
		else
		{
			return nullptr;
		}
	}

	CSNode* get_cnode_pt(float x, float y, Orient o, int floor_num)
	{
		if (_cspace[o].empty())
		{
			return 0;
		}

		vector2 xy(x, y);

		float distsq_close = 0.0f;
		int pos_close = -1;
		for (int pos_cur = 0; pos_cur < _cspace[o].size(); ++pos_cur)
		{
			if (_cspace[o][pos_cur].get_floor_num() == floor_num)
			{
				float distsq_cur = xy.distsq(_cspace[o][pos_cur].get_position());
				if (pos_close == -1 || distsq_cur < distsq_close)
				{
					distsq_close = distsq_cur;
					pos_close = pos_cur;
				}
			}
		}
		if (pos_close == -1)
			return 0;

		return &_cspace[o][pos_close];
	}

	void duplicate_layer(Orient layer_from)
	{
		for (Orient o = layer_from + 1; o != layer_from; o+=1)
		{
			_cspace[o] = _cspace[layer_from];
		
			for (std::vector<CSNode>::iterator it = _cspace[o].begin(); it != _cspace[o].end(); ++it)
			{
				it->change_orient(o);
			}
		}
		log_main.print("duplicate_layer");
	}

	void connect_to_neigh(Orient o)
	{
		for (int i = 0; i < _cspace[o].size(); ++i)
		{
			_cspace[o][i].connect_above_below();
		}
		log_main.print("connect_to_neigh");
	}

	// Low Dimension (LD) cspace for heuristics
	std::vector<CSNode> _LD_cspace;

	void LD_construct()
	{
		_LD_cspace = _cspace[0];
	}

	bool LD_pos_in_vec(int pos)
	{
		if (pos < _LD_cspace.size() && pos >= 0)
		{
			return true;
		}
		return false;
	}
	bool LD_pos_in_vec(cnode_pos pos)
	{
		if (pos.vec_pos < _LD_cspace.size() && pos.vec_pos >= 0)
		{
			return true;
		}
		return false;
	}

	CSNode* LD_get_cnode_pt(int pos)
	{
		if (pos < _LD_cspace.size())
		{
			return &_LD_cspace[pos];
		}
		else
		{
			return nullptr;
		}
	}
	CSNode* LD_get_cnode_pt(cnode_pos pos)
	{
		if (pos.vec_pos < _LD_cspace.size())
		{
			return &_LD_cspace[pos.vec_pos];
		}
		else
		{
			return nullptr;
		}
	}

	void LD_set_to_valid(int pos, int prefab_id)
	{
		if (pos < _LD_cspace.size())
		{
			_LD_cspace[pos]._valid[prefab_id] = true;
		}
	}
	void LD_set_to_valid(cnode_pos pos, int prefab_id)
	{
		if (pos.vec_pos < _LD_cspace.size())
		{
			_LD_cspace[pos.vec_pos]._valid[prefab_id] = true;
		}
	}

	bool LD_node_is_valid(int pos, int prefab_id)
	{
		return _LD_cspace[pos]._valid[prefab_id];
	}

	void LD_init_heuristics(int object_id)
	{
		for (std::vector<CSNode>::iterator it = _LD_cspace.begin(); it != _LD_cspace.end(); ++it)
		{
			it->_potentials[object_id] = -1.0f;
		}
	}

	void LD_reset_heuristics(int object_id)
	{
		for (std::vector<CSNode>::iterator it = _LD_cspace.begin(); it != _LD_cspace.end(); ++it)
		{
			if (it->_valid[object_id])
			{
				it->_potentials[object_id] = -1.0f;
			}
		}
	}

	void LD_null_heuristics(int object_id)
	{
		{
			for (std::vector<CSNode>::iterator it = _LD_cspace.begin(); it != _LD_cspace.end(); ++it)
			{
				if (it->_valid[object_id])
				{
					it->_potentials[object_id] = 0.0f;
				}
			}
		}
	}

	void LD_set_heuristic(cnode_pos pos, int object_id, float value)
	{
		_LD_cspace[pos.vec_pos]._potentials[object_id] = value;
	}
	void LD_set_heuristic(int pos, int object_id, float value)
	{
		_LD_cspace[pos]._potentials[object_id] = value;
	}

	float LD_get_heuristic(cnode_pos pos, int object_id)
	{
		return _LD_cspace[pos.vec_pos]._potentials[object_id];
	}
	float LD_get_heuristic(int pos, int object_id)
	{
		return _LD_cspace[pos]._potentials[object_id];
	}

	std::vector<CSArc>* LD_get_arcs(cnode_pos pos)
	{
		return &_LD_cspace[pos.vec_pos]._arcs;
	}
	std::vector<CSArc>* LD_get_arcs(int pos)
	{
		return &_LD_cspace[pos]._arcs;
	}

	int LD_get_stair_id(int pos)
	{
		return _LD_cspace[pos]._stair_id;
	}

	// cspace and LD cspace
	void init_validity(int prefab_id, bool state)
	{
		for (int i = 0; i < iNUM_ORIENT; ++i)
		{
			for (std::vector<CSNode>::iterator it = _cspace[i].begin(); it != _cspace[i].end(); ++it)
			{
				it->_valid[prefab_id] = state;
			}
		}
		for (std::vector<CSNode>::iterator it = _LD_cspace.begin(); it != _LD_cspace.end(); ++it)
		{
			it->_valid[prefab_id] = state;
		}
	}

	// stairs
	const std::map<int, std::map<int, std::vector<cnode_pos>>> init_stairs(std::vector<int>& prefab_ids) // set all stair_ids = stair_id for all prefabs
	{
		std::map<int, std::map<int, std::vector<cnode_pos>>> output;

		std::map<unsigned char, int> base_stair;

		for (int i : prefab_ids)
		{
			base_stair[i] = -1;
		}

		// init LD (actual stair ids and stair arcs will be added when setting stair arcs)
		for (int n = 0; n < _LD_cspace.size(); ++n)
		{
			_LD_cspace[n]._stair_ids = base_stair;
			for (int a = 0; a < _LD_cspace[n]._arcs.size(); ++a)
			{
				_LD_cspace[n]._arcs[a].stair_dir = NOT_STAIR_ARC;
			}
		}

		for (int o = 0; o < iNUM_ORIENT; ++o)
		{
			for (int n = 0; n< _cspace[o].size(); ++n)
			{
				CSNode& node = _cspace[o][n];
				int s_id = node._stair_id;
				if (s_id == -1)
				{
					node._stair_ids = base_stair;
				}
				else
				{
					std::map<unsigned char, int> temp_stair = base_stair;
					for (std::pair<const unsigned char, int>& p : temp_stair)
					{
						p.second = s_id;
						output[p.first][s_id].push_back(node.get_vec_pos());
					}
					node._stair_ids = temp_stair;
				}
			}
		}

		return output;
	}

};

struct corridor_edge
{
private: 
	struct element
	{
		float potential_heuristic;
		float potential;
		cnode_pos position;

		element(float pot_heu, float pot, cnode_pos pos) : potential_heuristic(pot_heu), potential(pot), position(pos) {};

		bool operator== (element& other) const
		{
			if (potential_heuristic == other.potential_heuristic && potential == other.potential && position == other.position) return true;
			return false;
		}
		bool operator== (const element& other) const
		{
			if (potential_heuristic == other.potential_heuristic && potential == other.potential && position == other.position) return true;
			return false;
		}

		bool operator< (element& other) const
		{
			if (potential_heuristic < other.potential_heuristic)
			{
				return true;
			}
			else if (potential_heuristic == other.potential_heuristic)
			{
				if (potential < other.potential)
				{
					return true;
				}
				else if (potential == other.potential)
				{
					if (position < other.position)
					{
						return true;
					}
				}
			}

			return false;
		}
		bool operator< (const element& other) const
		{
			if (potential_heuristic < other.potential_heuristic)
			{
				return true;
			}
			else if (potential_heuristic == other.potential_heuristic)
			{
				if (potential < other.potential)
				{
					return true;
				}
				else if (potential == other.potential)
				{
					if (position < other.position)
					{
						return true;
					}
				}
			}

			return false;
		}

		bool operator> (element& other) const
		{
			if (potential_heuristic > other.potential_heuristic)
			{
				return true;
			}
			else if (potential_heuristic == other.potential_heuristic)
			{
				if (potential > other.potential)
				{
					return true;
				}
				else if (potential == other.potential)
				{
					if (position > other.position)
					{
						return true;
					}
				}
			}

			return false;
		}
		bool operator> (const element& other) const
		{
			if (potential_heuristic > other.potential_heuristic)
			{
				return true;
			}
			else if (potential_heuristic == other.potential_heuristic)
			{
				if (potential > other.potential)
				{
					return true;
				}
				else if (potential == other.potential)
				{
					if (position > other.position)
					{
						return true;
					}
				}
			}

			return false;
		}

		bool less_than(float& pot, float& heu) const
		{
			if (potential < pot)
			{
				return true;
			}
			else if (potential == pot)
			{
				if (potential_heuristic < pot + heu)
				{
					return true;
				}
			}
			return false;
		}

		bool greater_than(float& pot, float& heu) const
		{
			if (potential > pot)
			{
				return true;
			}
			else if (potential == pot)
			{
				if (potential_heuristic > pot + heu)
				{
					return true;
				}
			}
			return false;
		}

	};

	std::set<element> st;
	std::set<element>::iterator find(cnode_pos node)
	{
		clock_t t1 = clock();
		for (std::set<element>::iterator it = st.begin(); it != st.end(); ++it)
		{
			if (node == it->position)
			{
				timer_find += (clock() - t1) / (float)CLOCKS_PER_SEC;
				return it;
			}
		}
		timer_find += (clock() - t1) / (float)CLOCKS_PER_SEC;
		return st.end();
	}
public:
	//debug
	float timer_find = 0.0f;
	int count = 0;
	int count2 = 0;

	// add new cnode
	void add_new_cnode(cnode_pos node, float potential, float heuristic) //(if already exists, update potential)
	{
		st.insert(element(potential + heuristic, potential, node));
	}

	// update heuristics
	std::vector<cnode_pos> get_ordered_cnode_pos()
	{
		std::vector<cnode_pos> vec;
		for (std::set<element>::iterator it = st.begin(); it != st.end(); ++it)
		{
			vec.push_back(it->position);
		}
		return vec;
	}
	void update_heuristics(std::vector<float> ordered_heuristics) // heuristics in same order as elements of st
	{
		if (ordered_heuristics.size() != st.size()) return;

		std::set<element> temp;

		std::vector<float>::iterator vec_it = ordered_heuristics.begin();
		for (std::set<element>::iterator set_it = st.begin(); set_it != st.end(); ++set_it, ++vec_it)
		{
			temp.insert(element(set_it->potential + *vec_it, set_it->potential, set_it->position));
		}
		st.swap(temp);
	}

	// return pair(cnode position, potential) and remove element
	std::pair<cnode_pos,float> pop_first() 
	{
		std::pair<cnode_pos, float> temp = std::pair<cnode_pos, float>(st.begin()->position, st.begin()->potential);
		st.erase(st.begin());
		return temp;
	}

	// clear edges (if new path is needed)
	void clear_edges()
	{
		st.clear();
	}

	int num_edges()
	{
		return st.size();
	}
	bool empty()
	{
		return st.empty();
	}
};

struct heuristic_edge
{
private: 
	struct element
	{
		float heuristic;
		int position;

		element(float heu, int pos) : heuristic(heu), position(pos) {};

		bool operator== (element& other) const
		{
			if (heuristic == other.heuristic && position == other.position) return true;
			return false;
		}
		bool operator== (const element& other) const
		{
			if (heuristic == other.heuristic && position == other.position) return true;
			return false;
		}

		bool operator< (element& other) const
		{
			if (heuristic < other.heuristic)
			{
				return true;
			}
			else if (heuristic == other.heuristic)
			{
				if (position < other.position)
				{
					return true;
				}
			}
			return false;
		}
		bool operator< (const element& other) const
		{
			if (heuristic < other.heuristic)
			{
				return true;
			}
			else if (heuristic == other.heuristic)
			{
				if (position < other.position)
				{
					return true;
				}
			}
			return false;
		}

		bool operator> (element& other) const
		{
			if (heuristic > other.heuristic)
			{
				return true;
			}
			else if (heuristic == other.heuristic)
			{
				if (position > other.position)
				{
					return true;
				}
			}
			return false;
		}
		bool operator> (const element& other) const
		{
			if (heuristic > other.heuristic)
			{
				return true;
			}
			else if (heuristic == other.heuristic)
			{
				if (position > other.position)
				{
					return true;
				}
			}
			return false;
		}

		bool less_than(float& heu) const
		{
			if (heuristic < heu)
			{
				return true;
			}
			return false;
		}

		bool greater_than(float& heu) const
		{
			if (heuristic > heu)
			{
				return true;
			}
			return false;
		}
	};
	std::set<element> st;

	std::set<element>::iterator find(int node)
		 {
			 for (std::set<element>::iterator it = st.begin(); it != st.end(); ++it)
			 {
				 if (node == it->position)
				 {
					 return it;
				 }
			 }
			 return st.end();
		 }

public:
	// add new cnode / update existing heuristic
	void add_node(int node, float heuristic) //(if already exists, update potential)
	{
		std::set<element>::iterator it = find(node);
		if (it != st.end()) // in set
		{
			if (it->greater_than(heuristic))
			{
				st.erase(it);
				st.insert(element(heuristic, node));
			}
		}
		else // not in set
		{
			st.insert(element(heuristic, node));
		}
	}
	void add_new_node(int node, float heuristic) //(if already exists, update potential)
	{
		st.insert(element(heuristic, node));
	}

	// return pair(cnode position, potential) and remove element
	std::pair<int, float> pop_first()
	{
		std::pair<int, float> temp = std::pair<int, float>(st.begin()->position, st.begin()->heuristic);
		st.erase(st.begin());
		return temp;
	}

	// clear edges (if new path is needed)
	void clear_edges()
	{
		st.clear();
	}

	int num_edges()
	{
		return st.size();
	}
	bool empty()
	{
		return st.empty();
	}
};

struct ObjectSpace_state
{
	bool saved;
	std::vector<GenObject> objects;
	std::map<int, person> people;
	std::vector<node> nodes;

	ObjectSpace_state() : saved(false) {}
};

class ObjectSpace // singleton!
{
	cspace m_cspace;

	std::map<int, std::vector<cnode_pos>> m_tnode_to_cnode; // tnode_id, cnode_ids
	std::set<int> m_all_tnode_ids;
	std::vector<boundary_line> m_bounds;

	std::vector<ObjectPrefab> m_prefabs;
	std::vector<GenObject> m_objects;
	std::map<int, person> m_people;
	ObjectSpace_state m_state;

	std::map<int, corridor_edge> m_corridor_edges; // object_id, set of edges
	std::map<int, heuristic_edge> m_heuristic_edges; // object_id, set of LD_node_ids
	std::default_random_engine m_generator;

	// TCP tspace construction
	std::vector<node> m_EX_nodes;
	std::vector<arc> m_EX_arcs;
	std::vector<stair> m_EX_stairs;
	std::vector<landing> m_EX_landings;
	std::vector<boundary_line> m_EX_bounds;
	int m_next_node_id;
	int m_next_arc_id;
	int m_next_stair_id;
	int m_next_landing_id;
	int m_next_bound_id;

	// TCP object construction
	int m_next_prefab_id;
	int m_next_object_id;

	// TSBS
	void add_arcs(cnode_pos, cnode_pos, bool);
	void add_arcs_angle_check(cnode_pos, cnode_pos, bool, Angle);
	void generate_TSBS(std::vector<arc>&);
	void print_arcs(int);

	//arc intersection
	int intersect_test2_orient(const x_y<float>&, const x_y<float>&, const x_y<float>&);
	bool intersect_test2(const arc&, const arc&);
	x_y<float> arc_intersection(const arc&, const arc&);

	// heuristics
	void reset_heuristic(int, cnode_pos);
	std::pair<float, bool> get_heuristic(int, int, speeds, cnode_pos, float);

	// potential corridor
	float calc_weight(bool trans, bool rot, float xy_displacement, float max_acel, float max_speed, float max_ang_speed);
	bool generate_potentials(int, std::vector<cnode_pos>, std::vector<cnode_pos>, float);
	void reset_potentials(int);
	void reset_all_potentials();
	void add_new_cnode_to_edges(cnode_pos, float, float, int);
	void set_potential(cnode_pos, float, int);
	void reset_edge_heuristics(int, int, speeds, cnode_pos, float);
	bool landing_valid_pot_gen(CSNode* from, CSNode* to, unsigned char stair_dir, int prefab_id, float& additional_cost);

	// Freespace
	bool set_validity(cnode_pos, ObjectPrefab*, polygon, std::vector<attachment_point>, int floor, int stair);
	bool is_valid(cnode_pos, int);
	unsigned char get_stair_type(float stair_direction, float arc_angle, float tolarence);
	void set_all_stair_nodes(); // find cnodes at top and bottom of stairs and mark them as part of the stair if object intersects with stair
	std::vector<CSArc*> get_opposed_arcs(CSNode* node); // in same order. If vector is empty, then at least one opposite arc was not found
	std::vector<CSArc*> get_opposed_LD_arcs(CSNode* node);
	void set_landing_distance(CSNode* node_pt, ObjectPrefab* prefab_pt);

	// simulation

	// action
	bool manage_tasks(int);
	std::pair<bool, bool> action_task(int, float);
	void move_obj(int, float, float, bool interpolate);
	void delay_obj(int);
	void idle_obj(int);
	std::pair<std::map<int, bool>, std::map<int, bool>> action_all_tasks(float); // first = task completed, second = object deleted
	std::vector<int> rnd_get_task_order();
	float calc_blocking_dist(vector2 velocity, std::vector<x_y<float>> verts, int floor, int stair_id);
	vector2 get_desired_vel(GenObject* obj, CSNode* new_node);

	// Constructors
	ObjectSpace();
	~ObjectSpace();

public:
	// singleton
	static ObjectSpace& Get()
	{
		static ObjectSpace instance;
		return instance;
	}
	ObjectSpace(const ObjectSpace&) = delete;
	ObjectSpace& operator=(const ObjectSpace&) = delete;

	// TCP construction
	static bool TCP_arc_id_check;
	static bool TCP_only_floor_1;
	void TCP_add_node(node);
	int GL_autoconnect_nodes(float spacing);
	void TCP_add_arc(arc);
	void TCP_add_stair(stair, bool);
	void TCP_add_landing(landing);
	void TCP_add_bound(boundary_line);
	bool TCP_node_exist(int id);
	bool TCP_arc_exist(int id);
	bool TCP_stair_exist(int id);
	bool TCP_landing_exist(int id);
	bool TCP_bound_exist(int id);
	node* TCP_get_node_from_id(int id);
	void TCP_remove_node(const int* id);
	void TCP_remove_arc(const int* id);
	void TCP_remove_stair(const int* id);
	void TCP_remove_bound(const int* id);
	void TCP_add_prefab(object_prefab);
	void TCP_add_prefab_vertex(int prefab_id, float x, float y);
	void TCP_add_prefab_attach_point(int prefab_id, int point_id, start_end<x_y<float>> line, int num_points, float radius, float attachment_time, int required_attendants, bool is_driver);
	void TCP_add_prefab_attach_point(int prefab_id, int point_id, x_y<float> point, float radius, float attachment_time, int required_attendants, bool is_driver);
	void TCP_add_prefab_attach_point(int prefab_id, int point_id, start_end<x_y<float>> line, int num_points, float radius, int default_occ, float attachment_time, int required_attendants, bool is_driver);
	void TCP_add_object(object_instance);
	void TCP_add_object_EX(int iObjectId, int iType, int iLevel, float fAngle, float sX, float sZ);
	bool TCP_add_person_to_object(int, int, int, bool during_sim);
	int TCP_pickup_person(int object_id, int person_id, float max_radius);
	void TCP_remove_person_from_object(int object_id, int person_id, int node_id);
	void TCP_remove_object(int object_id);
	void TCP_add_person(int, person);
	void TCP_add_person(int);
	void TCP_add_person(int id, int node_id, float drive, float fitness, int obj_id, int pnt_id, bool during_sim);
	void TCP_remove_person(int id);
	void GL_fill_objects_people(bool during_sim);
	void TCP_clear_geometry_vec();
	void TCP_clear_object_vec();
	std::vector<int> TCP_get_all_object_ids();

	// Landings
	void auto_gen_landings();
	std::vector<node*> get_landing_nodes(const stair& this_stair, bool top, int& other_stair);

	// TCP simulation
	void TCP_move_person(int person_id, int new_node_id, float speed, float drive, float fitness, int object_id, int point_id);

	// Generate
	void generate_cspace(bool auto_landings);
	void generate_freespace();
	bool cspace_ready;
	bool freespace_ready;

	std::vector<CSNode>::iterator nodes_begin(Orient);
	std::vector<CSNode>::iterator nodes_end(Orient);
	std::vector<CSNode>::iterator nodes_begin();
	std::vector<CSNode>::iterator nodes_end();

	// get stuff
	CSNode* get_node(cnode_pos);
	CSNode* get_LD_node(int);
	CSNode* get_nearest_cnode(float x, float y, Orient o, int floor);
	bool is_ready();
	int layer_size(Orient);
	int get_num_bounds();
	boundary_line get_bound_line(int);
	int get_num_tnodes();
	node get_tnode(int);
	node* get_tnode_pnt(int);
	int get_num_tarcs();
	arc get_tarc(int);
	int get_num_stairs();
	stair get_stair(int pos_in_vec);
	int get_num_landings();
	landing get_landing(int pos_in_vec);
	landing* get_landing_pt(int landing_id);
	stair* get_stairpt_from_id(int id);
	stair* get_stairpt(float x, float y, float floor_num);
	int get_num_obj();
	int get_num_people();
	const node* get_tnode(float x, float y, float tolarence, int floor);
	const node* get_tnode(float x, float y, float tolarence, int floor, int start_search_pos);
	const int* select_tnode(float x, float y, float tolarence, const int* ignore, int floor);
	const int* select_tarc(float x, float y, float tolarence, const int* ignore, int floor);
	const int* select_bound(float x, float y, float tolarence, const int* ignore, int floor);
	int get_next_node_id();
	int get_next_arc_id();
	int get_next_bound_id();
	int get_next_stair_id();
	int get_next_landing_id();
	int get_next_prefab_id();
	int get_next_object_id();
	GenObject get_object_from_vec(int vec_pos);
	std::vector<GenObject> get_objects();
	std::vector<ObjectPrefab> get_prefabs();
	std::map<int, person> get_people();

	// stair / landing
	void auto_landing_nodes(int landing_id);
	void auto_all_landing_nodes();
	bool is_node_in_landing_area(int node_id, int landing_id);
	void set_landing_id(node& n, int land_id);
	void set_all_arc_landing_ids();

	// file IO
	void save(const char* filename, float zoom, float camerax, float cameray);
	void load(const char* filename, float* zoom, float* camerax, float* cameray);
	void reset_all_arc_land_stair_ids();
	void output_setup_data();
	void output_tnode_adj_matrix(const char* filename);
	void output_cnode_adj_matrix(const char* filename);
	void output_tnode_adj_list(const char* filename);
	void output_cnode_adj_list(const char* filename);
	void output_cnode_carc_info(const char* filename);
	void output_tnode_tarc_info(const char* filename);
	void output_setup_object_info(const char * filename);
	void output_sim_data(const char * filename, int step, int sub_step, int comp_time_ms, float time, bool first);

	// objects / people
	GenObject* get_object(int);
	person* get_person(int);
	person* get_closest_person(vector2 pos, int floor, bool has_to_be_available);

	// occupation + halo
	std::pair<potential_move, occ_nodes> get_best_move_occ(std::vector<potential_move>& moves, GenObject* obj);
	occ_nodes try_to_occupy(GenObject& obj, CSNode& node, float radius);
	bool collision(const polygon& this_poly, const std::vector<attachment_point>& this_attach, char other_object_id);
	bool collision(const polygon& this_poly, const std::vector<attachment_point>& this_attach, const GenObject& other_object);
	void remove_occupation_obj(int object_id);
	void set_occupation_halo_obj(int object_id, float radius, float seconds);
	void set_occupation_halo_obj(occ_nodes& o_h, GenObject& obj, float seconds);
	bool test_intersect(node* n, polygon& poly, float radius, std::vector<attachment_point>& points);
	void add_occ_to_nodes_from_obj(int object_id);
	void update_occupation_halo(int object_id, float seconds);
	void update_person_position(int person_id, int node_id);
	void move_occupation(int old_id, int new_id, int person_id);
	void init_all_occupations();
	bool find_occupation_person(int person_id);
	node* get_nearest_tnode(float x, float y, int floor);

	// vos
	x_y<float> get_tnode_xypos(int id);
	bool intersection(start_end<x_y<float>> l1, start_end<x_y<float>> l2);
	bool intersection(start_end<x_y<float>> l1, start_end<x_y<float>> l2, x_y<float>& intersect);
	velocity_obstacle generate_pvo(GenObject* obj, person* per, bool generalised, bool hybrid, float time_to_collision, float dist_to_collision, float radius, float VO_dist_cost);
	void generate_ovo(GenObject* this_obj, GenObject* other_obj, bool generalised, bool hybrid, float time_to_collision, float dist_to_collision, float radius, float VO_dist_cost);
	void update_all_vos(bool first, bool generalised, bool hybrid, float time_to_collision, float dist_to_collision, float person_radius, float VO_dist_cost);
	bool check_visable(node* n1, node* n2);
	bool check_visable(std::vector<int> n1s, int n2);
	bool check_visable(std::vector<int> n1s, std::vector<int> n2s);

	// tasks
	void go_to_target(int, int, int);
	void go_to_target(int, int);
	void wait_for_seconds(int, int, float);
	void wait_for_seconds(int, float);
	void remove_object_task(int id);
	void pick_up_person(int o_id, int p_id);

	// simulation
	std::vector<occupation_seed> get_occupation_seeds();
	std::vector<data_for_TCP::object_config> main_sim_init_1();
	std::vector<data_for_TCP::occupied_nodes> main_sim_init_2();
	std::vector<data_for_TCP::stair_node> main_sim_init_stair_node();
	std::vector<data_for_TCP::object_config> main_sim_step_1(float time_sec, bool first);
	std::vector<data_for_TCP::occupied_nodes> main_sim_step_2();
	std::vector<data_for_TCP::pvo> main_sim_step_3(bool first);
	data_for_TCP::all_sim_data main_sim_step_ALL(float time_sec, bool first);
	float sim_time;

	// additional TCP data to send
	std::vector<data_for_TCP::attach_people> attached_people_store; // used for pick up person

	// save state
	void save_state();
	void retrieve_state();

	// test / debug
	std::map<int, int> test_duplicate_ids_tnodes();
	std::map<int, int> test_duplicate_ids_tarcs();
	std::map<cnode_pos, int> test_duplicate_ids_cnodes();
	void print_details_tnode(int id);
	void print_details_tarc(int id);
	void print_details_bound(int id);
	void correct_tarc_pos();
	void print_move(potential_move& m);
	std::vector<data_for_TCP::pvo> pvos_store;

	// options
	bool vo_hybrid;
	bool vo_generalised;
	float vo_time_to_collision; // Objects do not try to avoid each other objects until a collision would occur in less than this time (moving at current velocities)
	float vo_dist_to_collision;
	float vo_add_dist_cost;
	float vo_l_r_priority;
	float min_time_scale_stopped_ped;
	int num_runs_per_sim;
	int tcp_PortID;
	bool data_sim;
	bool data_adj_matrix;
	bool data_adj_list;
	bool data_cnode_arc_info;
	bool data_tnode_arc_info;
	bool data_object_info;
	float vo_des_vel_f;
};

// singleton
static ObjectSpace& s_object_space = ObjectSpace::Get();