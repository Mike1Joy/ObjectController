#pragma once
#include "DataStructs.h"

using namespace ObjCont;

struct speeds
{
	float speed_cost; // max speed scaled by this based on fitness of occupants. 1 = no scaling, near 0 = almost can't move

	float linear;
	float stair_dec;
	float stair_acc;
	float stair_side;
	float angular;

	float get_linear(unsigned char stair_dir, int stair_id) const
	{
		if (stair_id == -1) return linear * speed_cost;

		switch (stair_dir)
		{
		case NOT_STAIR_ARC:
			return linear * speed_cost;
		case STAIR_ARC_DOWN:
			return stair_dec * speed_cost;
		case STAIR_ARC_SIDEDOWN:
			return 0.5f*(stair_dec + stair_side)*speed_cost;
		case STAIR_ARC_SIDE:
			return stair_side * speed_cost;
		case STAIR_ARC_SIDEUP:
			return 0.5f*(stair_acc + stair_side)*speed_cost;
		case STAIR_ARC_UP:
			return stair_acc * speed_cost;
		default:
			return 0.0f;
		}
	}
	float get_angular() const { return angular * speed_cost; }

	void invert_stair_dirs()
	{
		float temp = stair_dec;
		stair_dec = stair_acc;
		stair_acc = temp;
	}

	speeds(float speed_cost, float linear, float stair_dec, float stair_acc, float stair_side, float angular)
		: speed_cost(speed_cost), linear(linear), stair_dec(stair_dec), stair_acc(stair_acc), stair_side(stair_side), angular(angular)
	{
		if (speed_cost > 1.0f) speed_cost = 1.0f;
	}
	speeds(const object_prefab& form)
		:
		speed_cost(1.0f),
		linear(form.max_linear_speed),
		stair_dec(form.stair_down ? form.max_stair_dec_speed : 0.0f),
		stair_acc(form.stair_up ? form.max_stair_acc_speed : 0.0f),
		stair_side(form.stair_side ? 0.5f*(stair_dec + stair_acc) : 0.0f),
		angular(form.max_angular_speed)
	{}
	speeds() : speed_cost(1.0f), linear(0.0f), stair_dec(0.0f), stair_acc(0.0f), stair_side(0.0f), angular(0.0f) {}
};

struct valid_stair_dirs
{
	bool up;
	bool down;
	bool side;

	bool valid(unsigned char stair_dir, int stair_id)
	{
		if (stair_id == -1) return true;

		switch (stair_dir)
		{
		case NOT_STAIR_ARC:
			return true;
		case STAIR_ARC_DOWN:
			return down;
		case STAIR_ARC_SIDEDOWN:
			return side && down;
		case STAIR_ARC_SIDE:
			return side;
		case STAIR_ARC_SIDEUP:
			return side && up;
		case STAIR_ARC_UP:
			return up;
		default:
			return true;
		}
	}

	void invert_stair_dirs()
	{
		bool temp = up;
		up = down;
		down = temp;
	}

	valid_stair_dirs(bool up, bool down, bool side)
		: up(up), down(down), side(side) {}
	valid_stair_dirs(const object_prefab& form)
		: up(form.stair_up), down(form.stair_down), side(form.stair_side) {}
	valid_stair_dirs()
		: up(true), down(true), side(true) {}

};

class ObjectPrefab
{
protected:
	int m_prefab_id;
	polygon m_polygon;
	speeds m_max_speeds;
	float m_max_acceleration;
	float m_holonomicity[iNUM_HOLO];
	bool m_turn_on_spot;
	valid_stair_dirs m_valid_stairs;
	std::vector<attachment_point> m_attachment_points;
	float m_landing_entrance;
	bool m_landing_inside;
	float m_width;
	float m_length;
	float m_max_width;

	ObjectPrefab();

public:
	float integral_drive;
	// getters
	int get_object_prefab_id() const;
	polygon get_polygon() const;
	std::vector<attachment_point> get_attachment_points();
	std::vector<attachment_point> get_extern_attachment_points();
	speeds get_max_speeds();
	float get_max_linear_speed();
	float get_max_linear_speed(unsigned char stair_dir, int stair_id);
	float get_max_angular_speed();
	float get_max_acceleration();
	float get_holonomicity(int);
	float get_holonomicity(std::pair<int,int>);
	bool can_turn_on_spot();
	bool can_take_stair(unsigned char stair_dir, int stair_id);
	valid_stair_dirs get_stair_valids();
	valid_stair_dirs get_inv_stair_valids();
	std::pair<float,bool> get_landing_entrance(); // distance, from_inside (false -> from outside)
	float get_width() const;
	float get_length() const;
	float get_max_width() const;

	void add_vertex(float x, float y);
	void add_attachment_point(int point_id, start_end<x_y<float>> line, int num_points, float radius, float attachment_time, int required_attendants, bool is_driver);
	void add_attachment_point(int point_id, start_end<x_y<float>> line, int num_points, float radius, int default_occ, float attachment_time, int required_attendants, bool is_driver);
	void add_attachment_point(int point_id, x_y<float> point, float radius, float attachment_time, int required_attendants, bool is_driver);

	ObjectPrefab(const object_prefab& structure);
	~ObjectPrefab();
};
