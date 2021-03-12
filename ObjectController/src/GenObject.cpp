#include "GenObject.h"
#include "Log.h"

// getters
int GenObject::get_object_id() const { return m_object_id; }
cnode_pos GenObject::get_cnode_id()const { return m_current_cnode; }
int GenObject::get_object_prefab_id() const { return m_prefab_id; }
polygon GenObject::get_polygon() const { return m_polygon; }
std::vector<x_y<float>> GenObject::get_verticies_relative() const
{
	polygon poly = m_polygon;
	poly.rotate(_orientation);
	poly.translate(_position);
	return poly.verticies;
}
speeds GenObject::get_max_speeds() const { return m_max_speeds; }
float GenObject::get_max_linear_speed(unsigned char stair_dir, int stair_id) const { return m_max_speeds.get_linear(stair_dir, stair_id); }
float GenObject::get_max_angular_speed() const { return m_max_speeds.get_angular(); }
float GenObject::get_max_acceleration() const { return m_max_acceleration* m_max_speeds.speed_cost; }
float GenObject::get_holonomicity(std::pair<int,int> i)
{
	if (i.first < 0) i.first *= -1;

	float h1 = m_holonomicity[i.first];

	if (i.second < 0) i.second *= -1;

	float h2 = m_holonomicity[i.second];

	return h1 < h2 ? h2 : h1;
}
bool GenObject::can_turn_on_spot() const { return m_turn_on_spot; }
bool GenObject::can_take_stair(unsigned char stair_dir, int stair_id){ return m_valid_stairs.valid(stair_dir, stair_id); }
valid_stair_dirs GenObject::get_stair_valids() const { return m_valid_stairs; }
valid_stair_dirs GenObject::get_inv_stair_valids() const
{
	valid_stair_dirs dirs = m_valid_stairs;
	dirs.invert_stair_dirs();
	return dirs;
}
int GenObject::get_floor() const { return m_floor_num; }
float GenObject::get_drive() const 
{
	return active ? m_drive : 1000.0f;
}
float GenObject::get_EX_angle() const
{
	return Angle(180.0f) - _orientation;
}
float GenObject::get_width() const
{
	return m_width;
}
float GenObject::get_length() const
{
	return m_length;
}
float GenObject::get_max_width() const
{
	return m_max_width;
}
float GenObject::get_blocking_width() const
{
	return m_blocking_width;
}

// attach / detach
bool GenObject::attach_person(person& occupant, int point_id, float& add_wait)
{
	for (std::vector<attachment_point>::iterator it = _attachment_points.begin(); it != _attachment_points.end(); ++it)
	{
		if (it->id == point_id)
		{
			it->attach(occupant);
			update_drive_fit();
			add_wait = it->attachment_time;
			return true;
		}
	}
	return false;
}
bool GenObject::remove_person(int person_id)
{
	for (std::vector<attachment_point>::iterator it = _attachment_points.begin(); it != _attachment_points.end(); ++it)
	{
		if (it->occupant.active && it->occupant.id == person_id)
		{
			it->detatch();
			update_drive_fit();
			return true;
		}
	}
	return false;
}
void GenObject::update_drive_fit()
{
	int num_occ = 0;
	m_drive = 0.0f;
	float speed_cost = 0.0f;
	active = true;
	for (std::vector<attachment_point>::iterator it = _attachment_points.begin(); it != _attachment_points.end(); ++it)
	{
		if (it->is_driver)
		{
			if (it->occupant.active)
			{
				m_drive += it->occupant.drive;
				speed_cost += it->occupant.fitness;
				++num_occ;
			}
			else // one of the driver attachment points is empty
			{
				active = false;
			}
		}
	}
	if (num_occ > 0)
	{
		m_drive *= m_integral_drive / num_occ;
		speed_cost /= num_occ;
	}
	m_max_speeds.speed_cost = speed_cost;
}

// occupation + halo
void GenObject::set_occupation_tnodes(std::vector<int>& tnodes)
{
	m_occupied_tnodes = tnodes;
}
std::vector<int> GenObject::get_occupation_tnodes() const
{
	return m_occupied_tnodes;
}
void GenObject::set_connected_halo(bool connected, float seconds)
{
	m_connected_halo = connected;
	bool add = !connected;
	if (!add)
	{
		for (velocity_obstacle& vo : m_VOs)
		{
			if (vo.valid && vo.blocking_cor)
			{
				add = true;
				break;
			}
		}
	}

	if (add)
	{
		time_blocking_cor += seconds;
		time_not_blocking_cor = 0.0f;
	}
	else
	{
		if (time_blocking_cor == 0.0f) return;

		float max_time = m_length / m_max_speeds.get_linear(NOT_STAIR_ARC, -1);
		if (time_not_blocking_cor >= max_time)
		{
			time_not_blocking_cor = 0.0f;
			time_blocking_cor = 0.0f;
			return;
		}

		time_blocking_cor -= seconds;
		time_not_blocking_cor += seconds;

		if (time_blocking_cor < 0.0f)
		{
			time_blocking_cor = 0.0f;
			time_not_blocking_cor = 0.0f;
		}
	}
}
void GenObject::clear_occ()
{
	m_occupied_tnodes.clear();
}
bool GenObject::halo_is_connected()
{
	return m_connected_halo;
}
occupation_seed GenObject::generate_occupation_seed()
{
	occupation_seed seed;

	seed.object_id = m_object_id;
	seed.floor_num = m_floor_num;
	seed.stair_id = stair_id;
	seed.node_id = m_tnode_id;
	seed.polygon = m_polygon;
	seed.attachment_points = _attachment_points;
	seed.orientation = _orientation;
	seed.position = _position;
	seed.vos = m_VOs;
	seed.velocity = velocity_current.first;

	seed.relative_to_object();

	return seed;
}
std::vector<attachment_point> GenObject::get_occupying_attachment_points() const
{
	std::vector<attachment_point> output;
	for (const attachment_point& p : _attachment_points)
	{
		if (!p.inside && p.occupant.active)
		{
			output.push_back(p);
		}
	}
	return output;
}
std::vector<x_y<float>> GenObject::get_attachment_point_pos()
{
	std::vector<x_y<float>> output;

	for (attachment_point p : _attachment_points)
	{
		p.rotate(_orientation);
		p.translate(_position);
		if (!p.inside && p.occupant.active)
		{
			output.push_back(p.get_occupant_pos());
		}
	}
	return output;
}
int GenObject::get_tnode_id()
{
	return m_tnode_id;
}
void GenObject::set_blocking_width(float w)
{
	m_blocking_width = w;
}

// action
bool GenObject::can_move()
{
	for (std::vector<attachment_point>::iterator it = _attachment_points.begin(); it != _attachment_points.end(); ++it)
	{
		if (!it->can_move())
		{
			return false;
		}
	}
	return true;
}
void GenObject::dont_move(float seconds)
{
	wait = 0.0f;
	velocity_current = { 0.0f,0.0f };
	moved = false;
	moved_backwards = false;
	not_move_cost += seconds;
	m_last_cnode = m_current_cnode;
	just_moved = false;

	if (!stopped)
	{
		just_stopped = true;
		stopped = true;
	}
	else
	{
		just_stopped = false;
	}
}
void GenObject::idle()
{
	wait = 0.0f;
	velocity_current = { 0.0f, 0.0f };
	moved = false;
	not_move_cost = 0.0f;
	moved_backwards = false;
	m_last_cnode = m_current_cnode;
	just_moved = false;
	velocity_desired = 0.0f;

	if (!stopped)
	{
		just_stopped = true;
		stopped = true;
	}
	else
	{
		just_stopped = false;
	}
}
void GenObject::match_cnodes()
{
	m_last_cnode = m_current_cnode;
}
void GenObject::move(cnode_pos new_pos, int tnode, float _wait, float move_time, vector2 vel, vector2 pos, std::map<unsigned char, std::set<unsigned char>> valid_attach, bool interpolate, int floor, int stair, bool backwards, float seconds, vector2 new_des_vel)
{
	if (new_pos == m_current_cnode) // don't move
	{
		dont_move(seconds);
		return;
	}

	wait += _wait;
	moved_backwards = backwards;
	velocity_current.first = (pos - _position) / move_time;
	velocity_current.second = orient_to_angle(new_pos.orientation).minus(_orientation) / move_time;
	velocity_desired = new_des_vel;
	match_cnodes();
	m_current_cnode = new_pos;
	m_tnode_id = tnode;
	m_floor_num = floor;
	stair_id = stair;
	not_move_cost = 0.0f;
	just_moved = true;
	moved = true;

	if (interpolate)
	{
		_position = _next_position;
		_next_position = pos;

		_orientation = _next_orientation;
		_next_orientation = orient_to_angle(new_pos.orientation);
	}
	else
	{
		_position = pos;
		_orientation = orient_to_angle(new_pos.orientation);
	}

	for (int i = 0; i < _attachment_points.size(); ++i)
	{
		_attachment_points[i].move_occ(valid_attach[_attachment_points[i].id], interpolate);
	}

	moved = true;
}
std::pair<std::pair<float,float>, vector2> GenObject::calc_waits_and_vel(vector2 from, vector2 to, float holo, bool rotated, bool translated, unsigned char stair_dir, bool& oscillate, cnode_pos new_pos)
{
	std::pair<std::pair<float, float>, vector2> out_wait_vel({ not_move_cost,0.0f }, 0.0f);

	// didn't move
	if (!translated && !rotated)
	{
		return out_wait_vel;
	}

	// if this move takes the object back to where it just was
	oscillate = new_pos == m_last_cnode;

	// just rotated
	if (rotated && !translated)
	{
		out_wait_vel.first.first = fLAYER_GAP / (m_max_speeds.get_angular());
		out_wait_vel.first.second = out_wait_vel.first.first;
		return out_wait_vel;
	}

	// translated
	vector2 displacement = to - from;
	float dist = displacement.magnitude();

	vector2 direction = displacement / dist;

	float cur_speed_sq = velocity_current.first.magnitude_squared();
	float new_speed = 0.0f;
	float max_lin_speed = m_max_speeds.get_linear(stair_dir, stair_id);
	
	if (cur_speed_sq < 0.01f) //// stopped
	{
		new_speed = sqrt(2.0f*get_max_acceleration()*dist);
	}
	else
	{
		float speed_in_direction = velocity_current.first.dot(direction); // speed in direction of move
		float speed_perp_direction = sqrt(cur_speed_sq - speed_in_direction * speed_in_direction); // speed perp to direcion of move

		new_speed = sqrt(2.0f*get_max_acceleration()*dist + cur_speed_sq);

		if (speed_perp_direction < -0.01f || speed_perp_direction > 0.01f) //// object is turning
		{
			vector2 u1 = direction; // perp of direction of move (normalised)
			vector2 u2 = velocity_current.first.norm(); // perp of vel (normalised)
			
			float centraped_radius = (u1.dot(to) - u1.dot(from)) / (u1.x*u2.y - u1.y*u2.x); // radius of centrapedal acceleration

			centraped_radius = abs(centraped_radius);

			float max_speed_cent = sqrt(get_max_acceleration()*centraped_radius);
			
			if (new_speed > max_speed_cent)
			{
				new_speed = max_speed_cent;
			}
		}
	}


	if (new_speed > max_lin_speed)
	{
		new_speed = max_lin_speed;
	}
	else if (new_speed < 0.0f)
	{
		new_speed = 0.0f;
	}
	
	new_speed *= holo;

	float t_l = 0.0f;
	float t_l_wait = 0.0f; // wait time uses average speed rather than new speed
	float t_r = 0.0f;

	if (translated && new_speed > 0.0f)
	{
		float avg_speed = 0.5f * (new_speed + sqrt(cur_speed_sq));
		t_l = dist / new_speed;
		t_l_wait = dist / avg_speed;
	}

	if (rotated)
	{
		t_r = fLAYER_GAP / m_max_speeds.get_angular();
	}

	out_wait_vel.first.first = sqrt(t_l_wait * t_l_wait + t_r * t_r);
	out_wait_vel.first.second = sqrt(t_l * t_l + t_r * t_r);
	out_wait_vel.second = displacement / out_wait_vel.first.second;
	return out_wait_vel;
}
void GenObject::set_position_orient(vector2 pos, Angle a)
{
	_position = pos;
	_orientation = a;
}

// potential
void GenObject::update_target_nodes(std::vector<cnode_pos> new_targets)
{
	m_current_targets = new_targets;
}
std::vector<cnode_pos> GenObject::get_current_targets()
{
	return m_current_targets;
}

// VO
void GenObject::add_vo(velocity_obstacle vo)
{
	m_VOs.push_back(vo);
}
void GenObject::clear_vos() // when object moves
{
	m_VOs.clear();
}
void GenObject::remove_vo(int id, bool object) // when other object / person moves
{
	auto it = find_vo(id, object);
	if (it != m_VOs.end())
	{
		m_VOs.erase(it);
	}
}
std::vector<velocity_obstacle>::iterator GenObject::find_vo(int id, bool object)
{
	for (std::vector<velocity_obstacle>::iterator it = m_VOs.begin(); it != m_VOs.end(); ++it)
	{
		if (it->other_object == object && it->other_ent_id == id)
		{
			return it;
		}
	}
	return m_VOs.end();
}
const std::vector<velocity_obstacle>& GenObject::get_vos() const
{
	return m_VOs;
}

// constuct
GenObject::GenObject(std::vector<ObjectPrefab>::iterator prefab, ObjCont::object_instance instance, int start_cnode_pos) :
	m_prefab_id(prefab->get_object_prefab_id()),
	m_polygon(prefab->get_polygon()),
	m_max_speeds(prefab->get_max_speeds()),
	m_max_acceleration(prefab->get_max_acceleration()),
	m_turn_on_spot(prefab->can_turn_on_spot()),
	m_valid_stairs(prefab->get_stair_valids()),
	m_object_id(instance.object_id),
	m_current_cnode(cnode_pos(start_cnode_pos, instance.start_orientation)),
	m_tnode_id(instance.start_node_id),
	stair_id(-1),
	m_floor_num(instance.start_floor_num),
	_attachment_points(prefab->get_attachment_points()),
	m_connected_halo(true),
	_position(instance.start_position),
	_next_position(instance.start_position),
	_orientation(orient_to_angle(instance.start_orientation)),
	_next_orientation(orient_to_angle(instance.start_orientation)),
	stopped(true),
	velocity_current(0.0f, 0.0f),
	velocity_desired(0.0f),
	moved_backwards(false),
	not_move_cost(0.0f),
	m_width(prefab->get_width()),
	m_length(prefab->get_length()),
	m_max_width(prefab->get_max_width()),
	m_blocking_width(INFINITY),
	time_blocking_cor(0.0f),
	time_not_blocking_cor(0.0f),
	just_moved(true),
	m_integral_drive(prefab->integral_drive)
{
	m_last_cnode = m_current_cnode;
	for (int i = 0; i < iNUM_HOLO; ++i)
	{
		m_holonomicity[i] = prefab->get_holonomicity(i);
	}

	for (std::vector<std::pair<int, person>>::iterator occ = instance.occupied_attachment_points.begin(); occ != instance.occupied_attachment_points.end(); ++occ)
	{
		attach_person(occ->second, occ->first, wait);
	}
	update_drive_fit();
	moved = active;
	just_stopped = active;
	wait = 0.0f;
}

GenObject::~GenObject() {}