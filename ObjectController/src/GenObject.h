#pragma once
#include "DataStructs.h"
#include "ObjectPrefab.h"

class GenObject : public ObjectPrefab
{
	int m_prefab_id;
	polygon m_polygon;
	speeds m_max_speeds;
	float m_max_acceleration;
	float m_holonomicity[iNUM_HOLO];
	bool m_turn_on_spot;
	valid_stair_dirs m_valid_stairs;

	float m_drive; // for VOs
	float m_integral_drive;
	float m_width;
	float m_length;
	float m_max_width;
	float m_blocking_width;

	int m_object_id;
	cnode_pos m_current_cnode;
	cnode_pos m_last_cnode;

	int m_tnode_id;
	int m_floor_num;

	std::vector<int> m_occupied_tnodes;
	bool m_connected_halo;

	std::vector<cnode_pos> m_current_targets;

	std::vector<velocity_obstacle> m_VOs;

public:
	std::vector<attachment_point> _attachment_points;

	int get_object_prefab_id() const;
	polygon get_polygon() const;
	std::vector<x_y<float>> get_verticies_relative() const;
	speeds get_max_speeds() const;
	float get_max_linear_speed(unsigned char stair_dir, int stair_id) const;
	float get_max_angular_speed() const;
	float get_max_acceleration() const;
	float get_holonomicity(std::pair<int, int>);
	bool can_turn_on_spot() const;
	bool can_take_stair(unsigned char stair_dir, int stair_id);
	valid_stair_dirs get_stair_valids() const;
	valid_stair_dirs get_inv_stair_valids() const;
	int get_floor() const;
	float get_EX_angle() const;

	float wait;
	std::pair<vector2, float> velocity_current; // vector2 linear vel, float angular vel
	bool active;
	vector2 _position;
	vector2 _next_position;
	Angle _orientation;
	Angle _next_orientation;
	int stair_id;
	bool moved;
	bool stopped;
	bool just_stopped;
	bool moved_backwards;
	float not_move_cost;
	float time_blocking_cor;
	float time_not_blocking_cor;

	//itinerary
	std::set<task> itinerary;
	task current_task;

	///////////////////////////////////////////

	// action
	bool can_move();
	void move(cnode_pos, int, float, vector2, vector2, std::map<unsigned char, std::set<unsigned char>> valid_attach, bool, int floor, int stair_id, bool backwards, float seconds);
	void dont_move(float seconds);
	void idle();
	void match_cnodes();
	std::pair<float, vector2> calc_wait_and_vel(vector2 from, vector2 to, float holo, bool rotated, bool translated, unsigned char stair_dir, bool& oscillate, cnode_pos new_pos);
	void set_position_orient(vector2, Angle);

	//potential
	void update_target_nodes(std::vector<cnode_pos>);
	std::vector<cnode_pos> get_current_targets();

	// getters
	int get_object_id() const;
	cnode_pos get_cnode_id() const;
	float get_drive() const;
	float get_width() const;
	float get_length() const;
	float get_max_width() const;
	float get_blocking_width() const;

	// attach / detach
	bool attach_person(person&, int, float& add_wait);
	bool remove_person(int);
	void update_drive_fit();

	// Occupation
	void set_occupation_tnodes(std::vector<int>& tnodes);
	std::vector<int> get_occupation_tnodes() const;
	void set_connected_halo(bool connected, float seconds);
	void clear_occ();
	bool halo_is_connected();
	occupation_seed generate_occupation_seed();
	std::vector<attachment_point> get_occupying_attachment_points() const;
	std::vector<x_y<float>> get_attachment_point_pos();
	int get_tnode_id();
	void set_blocking_width(float w);

	// VO
	void add_vo(velocity_obstacle vo);
	void clear_vos();
	void remove_vo(int id, bool object);
	std::vector<velocity_obstacle>::iterator find_vo(int id, bool object);
	const std::vector<velocity_obstacle>& get_vos() const;

	// OpenGL draw
	bool just_moved;

	GenObject(std::vector<ObjectPrefab>::iterator, object_instance, int);
	GenObject();
	~GenObject();
};
