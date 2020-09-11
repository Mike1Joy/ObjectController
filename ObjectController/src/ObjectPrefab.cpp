#include "ObjectPrefab.h"

// getters
int ObjectPrefab::get_object_prefab_id() const { return m_prefab_id; }
polygon ObjectPrefab::get_polygon() const { return m_polygon; }
std::vector<attachment_point> ObjectPrefab::get_attachment_points() { return m_attachment_points; }
std::vector<attachment_point> ObjectPrefab::get_extern_attachment_points()
{
	std::vector<attachment_point> output;
	
	for (const attachment_point& point : m_attachment_points)
	{
		if (!point.inside)
		{
			output.emplace_back(point);
		}
	}
	return output;
}
speeds ObjectPrefab::get_max_speeds() { return m_max_speeds; }

float ObjectPrefab::get_max_linear_speed() { return m_max_speeds.linear; }
float ObjectPrefab::get_max_linear_speed(unsigned char stair_dir, int stair_id) { return m_max_speeds.get_linear(stair_dir, stair_id); }
float ObjectPrefab::get_max_angular_speed() { return m_max_speeds.get_angular(); }
float ObjectPrefab::get_max_acceleration() { return m_max_acceleration; }
float ObjectPrefab::get_holonomicity(int i) { return m_holonomicity[i]; }
float ObjectPrefab::get_holonomicity(std::pair<int, int> i)
{ 
	if (i.first < 0) i.first *= -1;

	float h1 = m_holonomicity[i.first];

	if (i.second < 0) i.second *= -1;

	float h2 = m_holonomicity[i.second];

	return h1 < h2 ? h2 : h1;
}
bool ObjectPrefab::can_turn_on_spot() { return m_turn_on_spot; }
bool ObjectPrefab::can_take_stair(unsigned char stair_dir, int stair_id) { return m_valid_stairs.valid(stair_dir, stair_id); }
valid_stair_dirs ObjectPrefab::get_stair_valids() { return m_valid_stairs; }
valid_stair_dirs ObjectPrefab::get_inv_stair_valids()
{
	valid_stair_dirs dirs = m_valid_stairs;
	dirs.invert_stair_dirs();
	return dirs;
}
std::pair<float,bool> ObjectPrefab::get_landing_entrance()
{
	return { m_landing_entrance,m_landing_inside };
}

float ObjectPrefab::get_width() const
{
	return m_width;
}
float ObjectPrefab::get_length() const
{
	return m_length;
}
float ObjectPrefab::get_max_width() const
{
	return m_max_width;
}

void ObjectPrefab::add_vertex(float x, float y)
{
	m_polygon.add_vertex(x, y);

	m_width = m_polygon.get_width(true);
	m_length = m_polygon.get_width(false);

	float w = m_polygon.get_max_width();
	if (w > m_max_width)
		m_max_width = w;
}
void ObjectPrefab::add_attachment_point(int point_id, start_end<x_y<float>> line, int num_points, float radius, float attachment_time, int required_attendants, bool is_driver)
{
	m_attachment_points.push_back(attachment_point(point_id, line, num_points, radius, attachment_time, required_attendants, is_driver));

	float w = m_attachment_points.back().occupation_circles.get_max_width();
	if (w > m_max_width)
		m_max_width = w;
}
void ObjectPrefab::add_attachment_point(int point_id, start_end<x_y<float>> line, int num_points, float radius, int default_occ, float attachment_time, int required_attendants, bool is_driver)
{
	m_attachment_points.push_back(attachment_point(point_id, line, num_points, radius, default_occ, attachment_time, required_attendants, is_driver));

	float w = m_attachment_points.back().occupation_circles.get_max_width();
	if (w > m_max_width)
		m_max_width = w;
}
void ObjectPrefab::add_attachment_point(int point_id, x_y<float> point, float radius, float attachment_time, int required_attendants, bool is_driver)
{
	m_attachment_points.push_back(attachment_point(point_id, point, radius, attachment_time, required_attendants, is_driver));

	float w = m_attachment_points.back().occupation_circles.get_max_width();
	if (w > m_max_width)
		m_max_width = w;
}


ObjectPrefab::ObjectPrefab(const object_prefab& form) :
	m_prefab_id(form.object_prefab_id),
	m_polygon(form.polygon),
	m_attachment_points(form.attachment_points),
	m_max_acceleration(form.max_acceleration),
	m_turn_on_spot(form.turn_on_spot),
	m_valid_stairs(form),
	m_max_speeds(form),
	m_landing_entrance(form.landing_entrance),
	m_landing_inside(form.landing_inside),
	m_length(0.0f),
	m_width(0.0f),
	m_max_width(0.0f),
	integral_drive(form.integral_drive)
{
	for (int i = 0; i < iNUM_HOLO; ++i)
	{
		m_holonomicity[i] = form.holonomicity[i];
	}
}

ObjectPrefab::ObjectPrefab() {}
ObjectPrefab::~ObjectPrefab() {}