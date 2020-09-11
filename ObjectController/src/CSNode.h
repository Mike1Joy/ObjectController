#pragma once

#include <set>
#include <algorithm>
#include "DataStructs.h"

using namespace ObjCont;

struct CSArc
{
	cnode_pos cnode_to_vec_pos;
	x_y<float> pos_to;
	unsigned char floor_num_to;


	Angle angle;
	Angle orient_f;
	std::pair<char, char> holo;
	std::pair<char, char> inv_holo;
	float length;
	
	unsigned char stair_dir; // use enum stair_arc_direction

	bool squared;

	bool turn_on_spot;

	bool same_layer;

	CSArc(cnode_pos cnode_to_vec_pos_, x_y<float> _pos_to_, int floor_num_to_, Angle angle_, float length_, bool squared_, int holo_, int inv_holo_) // same layer
		:
		cnode_to_vec_pos(cnode_to_vec_pos_),
		pos_to(_pos_to_),
		floor_num_to(floor_num_to_),
		angle(angle_),
		length(length_),
		squared(squared_),
		turn_on_spot(false),
		same_layer(true),
		holo(std::pair<int,int>(holo_,holo_)),
		inv_holo(std::pair<int, int>(inv_holo_, inv_holo_))
	{
		stair_dir = NOT_STAIR_ARC;

		orient_f = orient_to_angle(cnode_to_vec_pos_.orientation);
	}

	CSArc(const CSArc *arc, bool below) // neigh layer
		: turn_on_spot(false), same_layer(false)
	{
		pos_to = arc->pos_to;
		floor_num_to = arc->floor_num_to;
		angle = arc->angle;
		orient_f = arc->orient_f;
		length = arc->length;
		squared = arc->squared;
		stair_dir = NOT_STAIR_ARC;
		
		cnode_to_vec_pos = arc->cnode_to_vec_pos;
		Angle temp_a = angle + 180.0f;

		if (below)
		{
			cnode_to_vec_pos.orientation -= 1;

			int h1 = arc->inv_holo.first;
			int h2 = h1 + 1;
			if (h2 >= iNUM_HOLO) h2 = 2 - iNUM_HOLO;
			inv_holo = std::pair<int, int>(h1, h2);

			h1 = arc->holo.first;
			h2 = h1 + 1;
			if (h2 >= iNUM_HOLO) h2 = 2 - iNUM_HOLO;
			holo = std::pair<int, int>(h1, h2);

			orient_f -= 0.5f * fLAYER_GAP;
		}
		else
		{
			cnode_to_vec_pos.orientation += 1;

			int h1 = arc->inv_holo.first;
			int h2 = h1 - 1;
			if (h2 <= 1 - iNUM_HOLO) h2 = iNUM_HOLO - 1;
			inv_holo = std::pair<int, int>(h1, h2);

			h1 = arc->holo.first;
			h2 = h1 - 1;
			if (h2 <= 1 - iNUM_HOLO) h2 = iNUM_HOLO - 1;
			holo = std::pair<int, int>(h1, h2);

			orient_f += 0.5f * fLAYER_GAP;
		}

	}

	CSArc(cnode_pos cnode_vec_pos, x_y<float> cnode_pos, int cnode_floor_num, bool below) // turn on spot
		: turn_on_spot(true), same_layer(false)
	{
		angle = 0.0f;
		orient_f = orient_to_angle(cnode_vec_pos.orientation);
		length = 0.0f;
		holo = std::pair<int,int>(0,0);
		inv_holo = std::pair<int, int>(0,0);
		squared = false;
		stair_dir = NOT_STAIR_ARC;

		pos_to = cnode_pos;
		floor_num_to = cnode_floor_num;

		cnode_to_vec_pos = cnode_vec_pos;

		if (below)
		{
			cnode_to_vec_pos.orientation -= 1;
			orient_f -= 0.5f*fLAYER_GAP;
		}
		else
		{
			cnode_to_vec_pos.orientation += 1;
			orient_f += 0.5f*fLAYER_GAP;
		}
	}
};

class CSNode
{
	cnode_pos m_vec_pos;
	
	std::set<int> m_tnode_ids;

	x_y<float> m_position;
	unsigned char m_floor;

public:
	// public vars
	bool fuse;
	bool divider;

	// stair ids
	int _stair_id;
	std::map<unsigned char, int> _stair_ids; // prefab_id, stair_id
	int landing_id;

	std::vector<CSArc> _arcs;

	std::map<unsigned char, bool> _valid; // int prefab_id, bool is_valid

	std::map<unsigned char, std::map<unsigned char, std::set<unsigned char>>> _attachment_point_validity; // map<prefab_id, map<attachment_point_id, set<invalid_circles>>>

	std::map<char, float> _potentials; // int object_id, float potential
									  // -1 = empty
									  // -2 = in stack
									  // -3 = broken
									  // else = potential is fixed
	std::map<unsigned char, bool> _potential_fixed; // int obj id, bool pot fixed

	std::map<unsigned char, float> _landing_dist_curve;

	// getters
	cnode_pos get_vec_pos() const;
	bool is_valid(unsigned char prefab_id);
	float get_potential(char object_id);
	const std::set<int>* get_tnode_ids();


	x_y<float> get_position() const;
	float get_theta();

	int get_floor_num() const;

	// copy to other layers
	void change_orient(Orient);
	void connect_above_below();

	// constructors
	CSNode(cnode_pos, node);
	CSNode();

	CSNode(cnode_pos, std::set<int>&, x_y<float>, int, bool, bool, int, int);

	~CSNode();
};

