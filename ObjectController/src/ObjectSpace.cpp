#include "ObjectSpace.h"
#include <map>
#include <set>
#include <vector>
#include <math.h>
#include <random>
#include <algorithm>

#include "FileIO.h"
#include "Log.h"

//// TCP construction

//geo
bool ObjectSpace::TCP_arc_id_check = false; // check for unique ids for arcs?
bool ObjectSpace::TCP_only_floor_1 = false;
void ObjectSpace::TCP_add_node(node n)
{
	if (TCP_node_exist(n.id))
	{
		log_main.print("Node not added: id %d already exists", n.id);
		return;
	}

	if (TCP_only_floor_1 && n.floor_num != 1)
	{
		log_main.print("Node not added: not on floor 1");
		return;
	}

	m_EX_nodes.push_back(n);
	if (m_next_node_id <= n.id)
	{
		m_next_node_id = n.id + 1;
	}
}
int ObjectSpace::GL_autoconnect_nodes(float spacing)
{
	int new_arcs = 0;
	for (int ii = 0; ii < m_EX_nodes.size(); ++ii)
	{
		float xx = m_EX_nodes[ii].position.x;
		float yy = m_EX_nodes[ii].position.y;
		int floor_num = m_EX_nodes[ii].floor_num;
		int id_ = m_EX_nodes[ii].id;


		for (int i = -1; i <= 1; ++i)
		{
			for (int j = -1; j <= 1; ++j)
			{
				if (i != 0 || j != 0)
				{
					const node* n = get_tnode(xx + i * spacing, yy + j * spacing, 0.01f, floor_num, ii+1);
					
					if (n)
					{
						TCP_add_arc(arc(-1, n->stair_id, id_, n->id, xx, yy, n->position.x, n->position.y, floor_num, n->floor_num, get_next_arc_id()));
						new_arcs++;
					}
				}
			}
		}
	}
	return new_arcs;
}
void ObjectSpace::TCP_add_arc(arc a)
{
	if (TCP_arc_id_check && TCP_arc_exist(a.id))
	{
		log_main.print("Arc not added: id %d already exists", a.id);
		return;
	}

	int start_id = a.tnode_ids.start;
	int end_id = a.tnode_ids.end;

	node* s_n = TCP_get_node_from_id(start_id);
	if (!s_n)
	{
		log_main.print("Arc not added: node_id %d does not exist", a.tnode_ids.start);
		return;
	}

	node* e_n = TCP_get_node_from_id(end_id);
	if (!e_n)
	{
		log_main.print("Arc not added: node_id %d does not exist", a.tnode_ids.end);
		return;
	}

	// set to_nodes in sn and en
	TCP_get_node_from_id(start_id)->tnodes.push_back(a.tnode_ids.end);
	TCP_get_node_from_id(end_id)->tnodes.push_back(a.tnode_ids.start);

	s_n = TCP_get_node_from_id(start_id);
	e_n = TCP_get_node_from_id(end_id);

	if (!a.all_info) // need to get start and end positions and floor numbers then calculate start_morethan_end and start_lessthan_end
	{
		a.position = start_end<x_y<float>>(s_n->position, e_n->position);
		a.floor_num = start_end<int>(s_n->floor_num, e_n->floor_num);

		a.start_morethan_end = x_y<bool>(s_n->position.x > e_n->position.x, s_n->position.y > e_n->position.y);
		a.start_lessthan_end = x_y<bool>(s_n->position.x < e_n->position.x, s_n->position.y < e_n->position.y);
		a.stair_ids = { s_n->stair_id, e_n->stair_id };
		a.land_id = max(s_n->landing_id, e_n->landing_id);
	}

	m_EX_arcs.push_back(a);

	if (m_next_arc_id <= a.id)
	{
		m_next_arc_id = a.id + 1;
	}

}
void ObjectSpace::TCP_add_stair(stair s, bool add_nodes)
{
	if (TCP_stair_exist(s.id))
	{
		log_main.print("Stair not added: id %d already exists", s.id);
		return;
	}

	if (TCP_only_floor_1 && s.floor_num != 1)
	{
		log_main.print("Stair not added: not on floor 1");
		return;
	}

	m_EX_stairs.push_back(s);
	stair& s_added = m_EX_stairs.back();

	if (m_next_stair_id <= s_added.id)
	{
		m_next_stair_id = s_added.id + 1;
	}

	log_main.print("stair added");

	if (add_nodes)
	{
		int node_id = get_next_node_id();
		int arc_id = get_next_arc_id();
		auto to_add = s_added.get_internal_tnodes_tarcs(node_id, arc_id, true);
		m_next_node_id = node_id;
		m_next_arc_id = arc_id;

		for (node& n : to_add.first)
		{
			TCP_add_node(n);
		}
		for (arc& a : to_add.second)
		{
			TCP_add_arc(a);
		}
	}
}
void ObjectSpace::TCP_add_landing(landing l)
{
	if (TCP_landing_exist(l.id))
	{
		log_main.print("Landing not added: id %d already exists", l.id);
		return;
	}

	if (TCP_only_floor_1 && l.floor != 1)
	{
		log_main.print("Landing not added: not on floor 1");
		return;
	}

	m_EX_landings.push_back(l);
	if (m_next_landing_id <= l.id)
	{
		m_next_landing_id = l.id + 1;
	}

	log_main.print("landing added");
}
void ObjectSpace::TCP_add_bound(boundary_line b)
{
	if (TCP_bound_exist(b.id))
	{
		log_main.print("Bound not added: id %d already exists", b.id);
		return;
	}

	if (TCP_only_floor_1 && b.floor_num != 1)
	{
		log_main.print("Bound not added: not on floor 1");
		return;
	}

	m_EX_bounds.push_back(b);
	if (m_next_bound_id <= b.id)
	{
		m_next_bound_id = b.id + 1;
	}
	log_main.print("bound added");
}

void ObjectSpace::TCP_clear_geometry_vec()
{
	m_EX_nodes.clear();
	m_EX_arcs.clear();
	m_EX_stairs.clear();
	m_EX_bounds.clear();
	m_next_node_id = 0;
	m_next_arc_id = 0;
	m_next_stair_id = 0;
	m_next_bound_id = 0;
}
bool ObjectSpace::TCP_node_exist(int id)
{
	for (auto it = m_EX_nodes.begin(); it != m_EX_nodes.end(); ++it)
	{
		if (it->id == id)
		{
			return true;
		}
	}
	return false;
}
bool ObjectSpace::TCP_arc_exist(int id)
{
	for (auto it = m_EX_arcs.begin(); it != m_EX_arcs.end(); ++it)
	{
		if (it->id == id)
		{
			return true;
		}
	}
	return false;
}
bool ObjectSpace::TCP_stair_exist(int id)
{
	for (auto it = m_EX_stairs.begin(); it != m_EX_stairs.end(); ++it)
	{
		if (it->id == id)
		{
			return true;
		}
	}
	return false;
}
bool ObjectSpace::TCP_landing_exist(int id)
{
	for (auto it = m_EX_landings.begin(); it != m_EX_landings.end(); ++it)
	{
		if (it->id == id)
		{
			return true;
		}
	}
	return false;
}
bool ObjectSpace::TCP_bound_exist(int id)
{
	for (auto it = m_EX_bounds.begin(); it != m_EX_bounds.end(); ++it)
	{
		if (it->id == id)
		{
			return true;
		}
	}
	return false;
}
node* ObjectSpace::TCP_get_node_from_id(int id)
{
	for (auto it = m_EX_nodes.begin(); it != m_EX_nodes.end(); ++it)
	{
		if (it->id == id)
		{
			return &(*it);
		}
	}
	return nullptr;
}
void ObjectSpace::TCP_remove_node(const int* id)
{
	if (id)
	{
		for (auto it = m_EX_nodes.begin(); it != m_EX_nodes.end(); ++it)
		{
			if (it->id == *id)
			{
				m_EX_nodes.erase(it);
				for (int a = 0; a < m_EX_arcs.size(); ++a)
				{
					arc& ar = m_EX_arcs[a];
					if (ar.tnode_ids.start == *id || ar.tnode_ids.end == *id)
					{
						m_EX_arcs.erase(m_EX_arcs.begin() + a);
						--a;
					}
				}
				return;
			}
		}
	}
}
void ObjectSpace::TCP_remove_arc(const int* id)
{
	if (id)
	{
		for (auto it = m_EX_arcs.begin(); it != m_EX_arcs.end(); ++it)
		{
			if (it->id == *id)
			{
				m_EX_arcs.erase(it);
				return;
			}
		}
	}
}
void ObjectSpace::TCP_remove_stair(const int* id)
{
	if (id)
	{
		for (auto it = m_EX_stairs.begin(); it != m_EX_stairs.end(); ++it)
		{
			if (it->id == *id)
			{
				m_EX_stairs.erase(it);
				return;
			}
		}
	}
}
void ObjectSpace::TCP_remove_bound(const int* id)
{
	if (id)
	{
		for (auto it = m_EX_bounds.begin(); it != m_EX_bounds.end(); ++it)
		{
			if (it->id == *id)
			{
				m_EX_bounds.erase(it);
				return;
			}
		}
	}
}

//obj / person
void ObjectSpace::TCP_add_prefab(object_prefab p)
{
	for (auto it = m_prefabs.begin(); it != m_prefabs.end(); ++it)
	{
		if (it->get_object_prefab_id() == p.object_prefab_id)
		{
			log_main.print("Prefab not added: prefab id already exists");
			return;
		}
	}

	m_prefabs.emplace_back(p);

	if (m_next_prefab_id <= p.object_prefab_id)
	{
		m_next_prefab_id = p.object_prefab_id + 1;
	}
	log_main.print("prefab added");
}
void ObjectSpace::TCP_add_prefab_vertex(int prefab_id, float x, float y)
{
	for (auto it = m_prefabs.begin(); it != m_prefabs.end(); ++it)
	{
		if (it->get_object_prefab_id() == prefab_id)
		{
			it->add_vertex(x, y);
			log_main.print("vertex added to prefab");
			return;
		}
	}
	log_main.print("vertex not added to prefab: prefab not found");
}
void ObjectSpace::TCP_add_prefab_attach_point(int prefab_id, int point_id, start_end<x_y<float>> line, int num_points, float radius, float attachment_time, int required_attendants, bool is_driver)
{
	for (auto it = m_prefabs.begin(); it != m_prefabs.end(); ++it)
	{
		if (it->get_object_prefab_id() == prefab_id)
		{
			it->add_attachment_point(point_id, line, num_points, radius, attachment_time, required_attendants, is_driver);
			log_main.print("attach point added to prefab");
			return;
		}
	}
	log_main.print("attach point not added to prefab: prefab not found");
}
void ObjectSpace::TCP_add_prefab_attach_point(int prefab_id, int point_id, x_y<float> point, float radius, float attachment_time, int required_attendants, bool is_driver)
{
	for (auto it = m_prefabs.begin(); it != m_prefabs.end(); ++it)
	{
		if (it->get_object_prefab_id() == prefab_id)
		{
			it->add_attachment_point(point_id, point, radius, attachment_time, required_attendants, is_driver);
			log_main.print("attach point added to prefab");
			return;
		}
	}
	log_main.print("attach point not added to prefab: prefab not found");
}
void ObjectSpace::TCP_add_prefab_attach_point(int prefab_id, int point_id, start_end<x_y<float>> line, int num_points, float radius, int default_occ, float attachment_time, int required_attendants, bool is_driver)
{
	for (auto it = m_prefabs.begin(); it != m_prefabs.end(); ++it)
	{
		if (it->get_object_prefab_id() == prefab_id)
		{
			it->add_attachment_point(point_id, line, num_points, radius, default_occ, attachment_time, required_attendants, is_driver);
			log_main.print("attach point added to prefab");
			return;
		}
	}
	log_main.print("attach point not added to prefab: prefab not found");
}
void ObjectSpace::TCP_add_object(object_instance i)
{
	for (auto it = m_objects.begin(); it != m_objects.end(); ++it)
	{
		if (it->get_object_id() == i.object_id)
		{
			log_main.print("Object not added: object id already exists");
			return;
		}
	}

	std::vector<ObjectPrefab>::iterator prefab_it;
	for (prefab_it = m_prefabs.begin(); prefab_it != m_prefabs.end(); ++prefab_it)
	{
		if (prefab_it->get_object_prefab_id() == i.object_prefab_id)
		{
			break;
		}
	}

	if (prefab_it != m_prefabs.end())
	{
		auto cnode_vec_it = m_tnode_to_cnode.find(i.start_node_id);

		if (cnode_vec_it != m_tnode_to_cnode.end())
		{
			int vec_pos = -1;
			for (auto cnode_it = cnode_vec_it->second.begin(); cnode_it != cnode_vec_it->second.end(); ++cnode_it)
			{
				if (!get_node(*cnode_it)->divider && !get_node(*cnode_it)->fuse)
				{
					vec_pos = cnode_it->vec_pos;
				}
			}
			if (vec_pos != -1)
			{
				m_objects.emplace_back(prefab_it, i, vec_pos);
				update_occupation_halo(i.object_id, 0.0f);
			}
			else
			{
				log_main.print("Object not added: no cnode exists that is neither a divider nor a fuse");
			}
		}
		else
		{
			log_main.print("Object not added: corresponding cnode not found");
		}
	}
	else
	{
		log_main.print("Object not added: prefab_id not found");
	}

	if (m_next_object_id <= i.object_id)
	{
		m_next_object_id = i.object_id + 1;
	}

	log_main.print("object added");
}
void ObjectSpace::TCP_add_object_EX(int iObjectId, int iType, int iLevel, float fAngle, float sX, float sZ)
{
	for (auto it = m_objects.begin(); it != m_objects.end(); ++it)
	{
		if (it->get_object_id() == iObjectId)
		{
			log_main.print("Object not added: object id %d already exists", iObjectId);
			return;
		}
	}

	std::vector<ObjectPrefab>::iterator prefab_it;
	for (prefab_it = m_prefabs.begin(); prefab_it != m_prefabs.end(); ++prefab_it)
	{
		if (prefab_it->get_object_prefab_id() == iType)
		{
			break;
		}
	}
	if (prefab_it == m_prefabs.end())
	{
		log_main.print("Object not added: object prefab id %d does not exists", iType);
		return;
	}

	if (!cspace_ready)
	{
		generate_cspace(true);
	}

	Orient orient = EX_ang_to_orient(fAngle);
	CSNode* cnode = get_nearest_cnode(sX, sZ, orient, iLevel);

	if (!cnode)
	{
		log_main.print("Object not added: cnode not found");
		return;
	}
	
	m_objects.emplace_back(prefab_it, object_instance(iObjectId, iType, *cnode->get_tnode_ids()->begin(), iLevel, orient, cnode->get_position().x, cnode->get_position().y), cnode->get_vec_pos().vec_pos);

	update_occupation_halo(iObjectId, 0.0f);

	if (m_next_object_id <= iObjectId)
	{
		m_next_object_id = iObjectId + 1;
	}

	log_main.print("object added");
}
bool ObjectSpace::TCP_add_person_to_object(int object_id, int point_id, int person_id, bool during_sim)
{
	person* person_pt = get_person(person_id);
	if (person_pt)
	{
		if (!person_pt->in_object)
		{
			GenObject* obj = get_object(object_id);
			if (obj)
			{
				float add_wait = 0.0f;
				if (obj->attach_person(*person_pt, point_id, add_wait))
				{
					if (add_wait > 0.0f && during_sim)
						wait_for_seconds(object_id, -1, add_wait);

					person_pt->active = true;
					move_occupation(person_pt->node_id, -1, person_id);
					person_pt->enter_object(object_id);
					log_main.print("person added to object");
					update_occupation_halo(object_id, 0.0f);
					return true;
				}
				else
				{
					log_main.print("person could not be added to object: attach point not found");
					return false;
				}
			}
			else
			{
				log_main.print("person could not be added to object: object not found");
				return false;
			}
		}
		else
		{
			log_main.print("person could not be added to object: person already in an object / not active");
			return false;
		}
	}
	else
	{
		log_main.print("person could not be added to object: person not found");
		return false;
	}
}
int ObjectSpace::TCP_pickup_person(int object_id, int person_id, float max_radius)
{
	GenObject* obj = get_object(object_id);
	if (!obj)
	{
		log_main.print("person could not be added to object: object not found");
		return -1;
	}

	person* person_pt;
	if (person_id < 0) // pick up closest person
	{
		person_pt = get_closest_person(obj->_position, obj->get_floor(), true);
	}
	else // pick up person id
	{
		person_pt = get_person(person_id);
	}
	if (!person_pt)
	{
		log_main.print("person could not be added to object: person not found");
		return -1;
	}
	if (person_pt->in_object)
	{
		log_main.print("person could not be added to object: person already in an object / not active");
		return -1;
	}

	float add_wait = 0.0f;
	int point_id = obj->attach_person_internal(*person_pt, add_wait);
	if (point_id == -1)
	{
		log_main.print("person could not be added to object: free internal attach point not found");
		return -1;
	}

	if (add_wait > 0.083f)
		wait_for_seconds(object_id, -1, add_wait);
	person_pt->active = true;
	move_occupation(person_pt->node_id, -1, person_id);
	person_pt->enter_object(object_id);
	log_main.print("person added to object");
	update_occupation_halo(object_id, 0.0f);

	attached_people_store.push_back({ object_id , person_pt->id, point_id }); // for TCP message

	return point_id;
}
void ObjectSpace::TCP_remove_person_from_object(int object_id, int person_id, int node_id)
{
	GenObject* obj = get_object(object_id);
	if (!obj)
	{
		log_main.print("Person not removed from object: object not found");
		return;
	}

	person* per = get_person(person_id);
	if (!per)
	{
		log_main.print("Person not removed from object: person not found");
		return;
	}

	node* tnode = TCP_get_node_from_id(node_id);
	if (!tnode)
	{
		log_main.print("Person not removed from object: node not found");
		return;
	}

	obj->remove_person(person_id);
	per->leave_object(tnode->position.x, tnode->position.y, 0.0f, 0.0f, node_id);
	per->active = true;
	move_occupation(node_id, node_id, person_id);

	log_main.print("Person removed from object");

	update_occupation_halo(object_id, 0.0f);
}
void ObjectSpace::TCP_remove_object(int object_id)
{
	for (auto it = m_objects.begin(); it != m_objects.end(); ++it)
	{
		if (it->get_object_id() == object_id)
		{
			for (attachment_point& p : it->_attachment_points)
			{
				if (p.occupant.active)
				{
					m_people.erase(p.occupant.id);
					p.detatch();
					log_main.print("person removed");
				}
			}
			m_objects.erase(it);
			remove_occupation_obj(object_id);
			log_main.print("object removed");
			return;
		}
	}
	log_main.print("object not removed: object not found");
}
void ObjectSpace::TCP_add_person(int id, person p)
{
	m_people[id] = p;
	move_occupation(p.node_id, p.node_id, id);
	log_main.print("person added");
}
void ObjectSpace::TCP_add_person(int node_id)
{
	node* n = get_tnode_pnt(node_id);
	if (!n)
	{
		log_main.print("person not added: node id not found");
		return;
	}

	int nextid = 1;
	for (const std::pair<int, person>& p : m_people)
	{
		if (p.first > nextid) nextid = p.first;
	}
	++nextid;

	m_people[nextid] = person(nextid, n->position.x, n->position.y, 0.0f, 0.0f, n->floor_num, 10.0f, 5.0f, node_id);

	move_occupation(node_id, node_id, nextid);

	log_main.print("person added");
}
void ObjectSpace::TCP_add_person(int id, int node_id, float drive, float fitness, int obj_id, int pnt_id, bool during_sim)
{
	node* n = get_tnode_pnt(node_id);
	if (!n)
	{
		log_main.print("Person not added: node not found");
		return;
	}

	m_people[id] = person(id, n->position.x, n->position.y, 0.0f, 0.0f, n->floor_num, drive, fitness, node_id);

	bool in_object = obj_id != -1 && pnt_id != -1;
	if (in_object)
	{
		in_object = TCP_add_person_to_object(obj_id, pnt_id, id, during_sim);
	}
	if (!in_object)
	{
		move_occupation(node_id, node_id, id);
	}

	log_main.print("person added");
}
void ObjectSpace::TCP_remove_person(int id)
{
	auto it = m_people.find(id);
	if (it != m_people.end())
	{
		move_occupation(it->second.node_id, -1, id);
		it->second.active = false;
		log_main.print("person removed");
		for (GenObject& o : m_objects)
		{
			if (o.remove_person(id))
			{
				log_main.print("person removed from object");
				update_occupation_halo(o.get_object_id(), 0.0f);
				return;
			}
		}
	}
	else
	{
		log_main.print("person not removed: id %d not found", id);
	}
}
void ObjectSpace::GL_fill_objects_people(bool during_sim)
{
	int next_id = m_people.empty() ? 0: m_people.rbegin()->first + 1;
	float drive = 1.0f;
	for (GenObject& obj : m_objects)
	{
		for (attachment_point& point : obj._attachment_points)
		{
			if (!point.occupant.active)
			{
				TCP_add_person(next_id, person(next_id, 0.0f, 0.0f, 0.0f, 0.0f, obj.get_floor(), drive, 5.0f, -1));
				TCP_add_person_to_object(obj.get_object_id(), point.id, next_id, during_sim);
				++next_id;
			}
		}
		// drive test
		drive += 1.0f;
	}
}
void ObjectSpace::TCP_clear_object_vec()
{
	m_prefabs.clear();
	m_objects.clear();
	m_next_prefab_id = 1;
	m_next_object_id = 1;

	log_main.print("prefabs and objects cleared\n");
}
std::vector<int> ObjectSpace::TCP_get_all_object_ids()
{
	std::vector<int> ids;

	for (GenObject& obj : m_objects)
	{
		ids.push_back(obj.get_object_id());
	}

	REMOVE_DUPLICATES(ids);

	return ids;
}

// TCP simulation
void ObjectSpace::TCP_move_person(int person_id, int new_node_id, float speed, float drive, float fitness, int object_id, int point_id)
{
	person* per = get_person(person_id);
	if (!per)
	{
		log_main.print("person not moved: person id not found");
		return;
	}

	if (per->in_object)
	{
		log_main.print("person not moved: person in object %d", per->object_id);
		return;
	}

	per->active = true;
	per->drive = drive;
	per->fitness = fitness;

	int old_node_id = per->node_id;
	if (new_node_id == old_node_id) // not moved, just update speed
	{
		per->velocity.resize(speed);
	}
	else
	{
		node* old_node = get_tnode_pnt(old_node_id);
		node* new_node = get_tnode_pnt(new_node_id);
		if (!new_node) // can't find new node
		{
			log_main.print("person not moved: new node id not recognised");
			per->velocity.resize(speed);
			return;
		}
		if (!old_node) // can't find old node
		{
			move_occupation(new_node_id, new_node_id, person_id);
			per->node_id = new_node_id;
			per->position = new_node->position;
			per->velocity.resize(speed);
			return;
		}
			
		// both nodes found
		per->velocity = new_node->position - old_node->position;
		per->velocity.resize(speed);
		move_occupation(old_node_id, new_node_id, person_id);
		per->node_id = new_node_id;
		per->position = new_node->position;
		per->moved = true;
		per->stopped = false;
	}
}

//// Generating cspace and objects

// Cspace
void ObjectSpace::generate_cspace(bool auto_landings)
{
	if (m_EX_nodes.empty() || m_EX_arcs.empty())
	{
		cspace_ready = false;
		return;
	}
	

	// clear then reserve enough memory for estimated size
	m_cspace.clear();
	m_cspace.reserve(m_EX_nodes.size());

	m_bounds = m_EX_bounds;

	// set up landings
	if (auto_landings)
		s_object_space.auto_gen_landings();
	set_all_arc_landing_ids();

	// populate first layer of cnodes
	int vector_position = 0;
	for (std::vector<node>::iterator it_nodes = m_EX_nodes.begin(); it_nodes != m_EX_nodes.end(); ++it_nodes)
	{
		// add cnode
		m_cspace.push_back(cnode_pos(vector_position, 0), *it_nodes);
		for (int i = 0; i < iNUM_ORIENT; ++i)
		{
			m_tnode_to_cnode[it_nodes->id].push_back({ vector_position, i });
		}

		// add new carc storing vector position of cnodes instead of tnode_id
		for (std::vector<arc>::iterator it_arcs = m_EX_arcs.begin(); it_arcs != m_EX_arcs.end(); ++it_arcs)
		{
			if (it_arcs->tnode_ids.start == it_nodes->id && !it_arcs->is_cnode.start)
			{
				it_arcs->cnode_ids.start = vector_position;
				it_arcs->is_cnode.start = true;
			}
			if (it_arcs->tnode_ids.end == it_nodes->id && !it_arcs->is_cnode.end)
			{
				it_arcs->cnode_ids.end = vector_position;
				it_arcs->is_cnode.end = true;
			}
		}

		++vector_position;
	}

	// generate TSBS
	generate_TSBS(m_EX_arcs);

	// construct LD cspace
	m_cspace.LD_construct();

	// duplicate to all layers
	m_cspace.duplicate_layer(0);

	// connect all layers
	for (int i = 0; i < iNUM_ORIENT; ++i)
	{
		m_cspace.connect_to_neigh(i);
	}

	log_main.print("_cnodes.size(0)*num_orient = %d\n", m_cspace.size(0)*iNUM_ORIENT);
	cspace_ready = true;
}
int ObjectSpace::intersect_test2_orient(const x_y<float>& p, const x_y<float>& q, const x_y<float>& r)
{
	float val = (q.y - p.y) * (r.x - q.x) -
		(q.x - p.x) * (r.y - q.y);

	if (val == 0.0f) return 0; // colinear 

	return (val > 0) ? 1 : 2; // clock or anti clock wise 
}
bool ObjectSpace::intersect_test2(const arc& a1, const arc& a2)
{
	int o1 = intersect_test2_orient(a1.position.start, a1.position.end, a2.position.start);
	int o2 = intersect_test2_orient(a1.position.start, a1.position.end, a2.position.end);
	int o3 = intersect_test2_orient(a2.position.start, a2.position.end, a1.position.start);
	int o4 = intersect_test2_orient(a2.position.start, a2.position.end, a1.position.end);

	if (o1*o2*o3*o4 == 0) return false;
	
	return o1 != o2	&& o3 != o4;
}
x_y<float> ObjectSpace::arc_intersection(const arc& arc1, const arc& arc2)
{
	// arc1 represented as a1x + b1y = c1 
	float a1 = arc1.position.end.y - arc1.position.start.y;
	float b1 = arc1.position.start.x - arc1.position.end.x;
	float c1 = a1 * (arc1.position.start.x) + b1 * (arc1.position.start.y);

	// arc2 represented as a2x + b2y = c2 
	float a2 = arc2.position.end.y - arc2.position.start.y;
	float b2 = arc2.position.start.x - arc2.position.end.x;
	float c2 = a2 * (arc2.position.start.x) + b2 * (arc2.position.start.y);

	float determinant = a1 * b2 - a2 * b1;

	if (determinant == 0.0f)
	{
		// should never happen but just in case this will stop it breaking
		return x_y<float>((arc1.position.start.x+arc1.position.end.x)/2.0f, (arc1.position.start.y + arc1.position.end.y)/2.0f);
	}
	else
	{
		return x_y<float>((b2*c1 - b1 * c2) / determinant, (a1*c2 - a2 * c1) / determinant);
	}
}
void ObjectSpace::add_arcs(cnode_pos vec_pos1, cnode_pos vec_pos2, bool squared)
{
	if (vec_pos1 == vec_pos2) return;
	CSNode* cnode1 = m_cspace.get_cnode_pt(vec_pos1);
	CSNode* cnode2 = m_cspace.get_cnode_pt(vec_pos2);

	Angle a1 = atan2(cnode2->get_position().x - cnode1->get_position().x, cnode2->get_position().y - cnode1->get_position().y) * RAD2DEG;
	Angle a2 = a1 + 180.0f;
	float length = distance(cnode2->get_position(), cnode1->get_position());

	int holo1 = to_holo(a1, vec_pos1.orientation);
	int holo2 = to_holo(a2, vec_pos2.orientation);

	CSArc carcf(vec_pos2, cnode2->get_position(), cnode2->get_floor_num(), a1, length, squared, holo1, holo2);
	CSArc carcb(vec_pos1, cnode1->get_position(), cnode1->get_floor_num(), a2, length, squared, holo2, holo1);

	m_cspace.get_cnode_pt(vec_pos1)->_arcs.push_back(carcf);
	m_cspace.get_cnode_pt(vec_pos2)->_arcs.push_back(carcb);
}
void ObjectSpace::add_arcs_angle_check(cnode_pos vec_pos1, cnode_pos vec_pos2, bool squared, Angle tolarence)
{
	if (vec_pos1 == vec_pos2) return;
	CSNode* cnode1 = m_cspace.get_cnode_pt(vec_pos1);
	CSNode* cnode2 = m_cspace.get_cnode_pt(vec_pos2);

	Angle a1 = atan2(cnode2->get_position().x - cnode1->get_position().x, cnode2->get_position().y - cnode1->get_position().y) * RAD2DEG;// angle from 1 to 2

	for (std::vector<CSArc>::iterator it = cnode1->_arcs.begin(); it != cnode1->_arcs.end(); ++it)
	{
		if (it->angle.min_diff(a1) < tolarence)
		{
			return;
		}
	}
	for (std::vector<CSArc>::iterator it = cnode2->_arcs.begin(); it != cnode2->_arcs.end(); ++it)
	{
		if (it->angle.min_diff(a1 + 180.0f) < tolarence)
		{
			return;
		}
	}

	float length = distance(cnode2->get_position(), cnode1->get_position());
	Angle a2 = a1 + 180.0f;

	int holo1 = to_holo(a1, vec_pos1.orientation);
	int holo2 = to_holo(a2, vec_pos2.orientation);

	CSArc carcf(vec_pos2, cnode2->get_position(), cnode2->get_floor_num(), a1, length, squared, holo1, holo2);
	m_cspace.get_cnode_pt(vec_pos1)->_arcs.push_back(carcf);

	cnode1 = m_cspace.get_cnode_pt(vec_pos1);
	CSArc carcb(vec_pos1, cnode1->get_position(), cnode1->get_floor_num(), a2, length, squared, holo2, holo1);
	m_cspace.get_cnode_pt(vec_pos2)->_arcs.push_back(carcb);
}
void ObjectSpace::generate_TSBS(std::vector<arc> &arcs)
{
	//find all nodes connected to original nodes in the network
	std::map<int, std::set<int>> original_node_adjacency;
	for (std::vector<arc>::iterator it = arcs.begin(); it != arcs.end(); ++it)
	{
		int start = it->cnode_ids.start == -1 ? it->tnode_ids.start : it->cnode_ids.start;
		int end = it->cnode_ids.end == -1 ? it->tnode_ids.end : it->cnode_ids.end;
		std::map<int, std::set<int>>::iterator position_it = original_node_adjacency.find(start);
		if (position_it == original_node_adjacency.end()) // start id not already used, so create element
		{

			position_it = original_node_adjacency.insert(std::pair<int, std::set<int>>(start, std::set<int>())).first;
		}
		position_it->second.insert(end);

		position_it = original_node_adjacency.find(end);
		if (position_it == original_node_adjacency.end()) // end id not already used, so create element
		{
			position_it = original_node_adjacency.insert(std::pair<int, std::set<int>>(end, std::set<int>())).first;
		}
		position_it->second.insert(start);
	}

	// Fuse arcs that intersect
	for (std::vector<arc>::iterator it1 = arcs.begin(); it1 != arcs.end(); ++it1)
	{
		if (!it1->divided && it1->is_cnode.start && it1->is_cnode.end)
		{
			for (std::vector<arc>::iterator it2 = it1+1; it2 != arcs.end(); ++it2)
			{
				if (!it2->divided && it2->is_cnode.start && it2->is_cnode.end)
				{
					// test if on same floor or (if one is stair and one is not) 
					if ((it1->floor_num.start == it2->floor_num.start && it1->floor_num.end == it2->floor_num.end)
						|| (it1->floor_num.start == it2->floor_num.end && it1->floor_num.end == it2->floor_num.start))
					{
						// test if *it1 and *it2 intersect
						if (intersect_test2(*it1, *it2)) // they do intersect
						{
							// mark *it1 and *it2 as divided
							it1->divided = true;
							it2->divided = true;

							// find point of intersection
							x_y<float> intersect = arc_intersection(*it1, *it2);

							// add new cnode at point of intersection marked as fuse
							cnode_pos vec_pos(m_cspace.size(0),0);
							std::set<int> tnode_ids;
							tnode_ids.insert(it1->tnode_ids.start);
							tnode_ids.insert(it1->tnode_ids.end);
							tnode_ids.insert(it2->tnode_ids.start);
							tnode_ids.insert(it2->tnode_ids.end);

							m_cspace.push_back(
								vec_pos, tnode_ids, intersect, 
								min(min(it1->floor_num.start, it1->floor_num.end), min(it2->floor_num.start, it2->floor_num.end)), 
								true, false, 
								min(min(it1->stair_ids.start, it1->stair_ids.end), min(it2->stair_ids.start, it2->stair_ids.end)),
								max(it1->land_id, it2->land_id)
							);

							// add 8 new arcs from new node to it1 and it2 and vica versa
							add_arcs(vec_pos, cnode_pos(it1->cnode_ids.start,0), false);
							add_arcs(vec_pos, cnode_pos(it1->cnode_ids.end, 0), false);
							add_arcs(vec_pos, cnode_pos(it2->cnode_ids.start, 0), false);
							add_arcs(vec_pos, cnode_pos(it2->cnode_ids.end, 0), false);

						}
					}
				}
			}
		}
	}
	
	// subdivide non-divided arcs
	for (std::vector<arc>::iterator it = arcs.begin(); it != arcs.end(); ++it)
	{
		if (!it->divided && it->is_cnode.start && it->is_cnode.end)
		{
			// add node
			cnode_pos vec_pos(m_cspace.size(0),0);
			x_y<float> position((it->position.start.x + it->position.end.x) / 2.0f, (it->position.start.y + it->position.end.y) / 2.0f);

			std::set<int> tnode_ids;
			tnode_ids.insert(it->tnode_ids.start);
			tnode_ids.insert(it->tnode_ids.end);

			m_cspace.push_back(vec_pos, tnode_ids, position, min(it->floor_num.start, it->floor_num.end), false, true, min(it->stair_ids.start, it->stair_ids.end), it->land_id);

			// add 4 arcs
			add_arcs(vec_pos, cnode_pos(it->cnode_ids.start,0), false);
			add_arcs(vec_pos, cnode_pos(it->cnode_ids.end,0), false);
		}
	}

	// square divider nodes based on old network
	for (int n_pos = 0; n_pos < m_cspace.size(0); ++n_pos) // for all nodes n in network
	{
		if (m_cspace.get_cnode_pt(0,n_pos)->divider && m_cspace.get_cnode_pt(0, n_pos)->_arcs.size() == 2) // if node n is divider
		{
			int m1 = m_cspace.get_cnode_pt(0, n_pos)->_arcs[0].cnode_to_vec_pos.vec_pos;
			int m2 = m_cspace.get_cnode_pt(0, n_pos)->_arcs[1].cnode_to_vec_pos.vec_pos;

			std::set<int> m1_nodes = original_node_adjacency.find(m1)->second;
			std::set<int> m2_nodes = original_node_adjacency.find(m2)->second;

			for (std::set<int>::iterator it = m1_nodes.begin(); it != m1_nodes.end(); ++it)
			{
				if (m2_nodes.find(*it) != m2_nodes.end())
				{
					add_arcs(cnode_pos(n_pos,0), cnode_pos(*it,0), true);
				}
			}
		}
	}

	// square divider nodes based on new network
	for (int n_pos = 0; n_pos < m_cspace.size(0); ++n_pos) // for all nodes n in network
	{
		if (m_cspace.get_cnode_pt(0, n_pos)->divider) // if node n is divider
		{
			// get m's (from not squared arcs)
			for (int to_arc_pos = 0; to_arc_pos < m_cspace.get_cnode_pt(0, n_pos)->_arcs.size(); ++to_arc_pos)
			{
				if (!m_cspace.get_cnode_pt(0, n_pos)->_arcs[to_arc_pos].squared)
				{
					int m_pos = m_cspace.get_cnode_pt(0, n_pos)->_arcs[to_arc_pos].cnode_to_vec_pos.vec_pos;

					// loop through to_nodes and get to_to_node ids (from not squared arcs)
					for (int to_to_arc_pos = 0; to_to_arc_pos < m_cspace.get_cnode_pt(0, m_pos)->_arcs.size(); ++to_to_arc_pos)
					{
						if (!m_cspace.get_cnode_pt(0, m_pos)->_arcs[to_to_arc_pos].squared)
						{
							add_arcs_angle_check(cnode_pos(n_pos,0), m_cspace.get_cnode_pt(0, m_pos)->_arcs[to_to_arc_pos].cnode_to_vec_pos, true, 1.0f);
						}
					}

				}
			}
		}
	}
	
	
	
	
	log_main.print("_nodes.size(): %d\n", m_cspace.size(0));


}
bool ObjectSpace::is_valid(cnode_pos node, int prefab_id)
{
	return get_node(node)->_valid[prefab_id];
}
void ObjectSpace::print_arcs(int layer)
{
	for (auto it1 = m_cspace._cspace[layer].begin(); it1 != m_cspace._cspace[layer].end(); ++it1)
	{
		for (auto it2 = it1->_arcs.begin(); it2 != it1->_arcs.end(); ++it2)
		{
			//log_main.print("(%d, %d)\n", it1->get_vec_pos().vec_pos, it2->cnode_to_vec_pos.vec_pos);
		}
	}
	log_main.print("\n");
}

// Objects
void ObjectSpace::generate_freespace()
{
	if (!cspace_ready || m_prefabs.empty() || m_objects.empty())
	{
		freespace_ready = false;
		log_main.print("no objects\n");
		TCP_clear_object_vec();
		return;
	}

	// initiate occupations for objects and people
	init_all_occupations();

	// get all instantiated prefabs + get all start nodes + initiate heuristics for objects
	std::set<int> prefab_ids;
	for (std::vector<GenObject>::iterator it = m_objects.begin(); it != m_objects.end(); ++it)
	{
		prefab_ids.insert(it->get_object_prefab_id());
		m_cspace.LD_init_heuristics(it->get_object_id());
	}
	
	// generate attachment_point_validity_template (assumes all prefab_ids are unique and attachment_point_ids are unique within each prefab)
	std::map<unsigned char, std::map<unsigned char, std::set<unsigned char>>> attachment_point_validity_template;
	for (std::vector<ObjectPrefab>::iterator prefab_it = m_prefabs.begin(); prefab_it != m_prefabs.end(); ++prefab_it)
	{
		std::vector<attachment_point> points = prefab_it->get_extern_attachment_points();
		std::map<unsigned char, std::set<unsigned char>> attachment_points;
		for (std::vector<attachment_point>::iterator point_it = points.begin(); point_it != points.end(); ++point_it)
		{
			attachment_points.insert(std::pair<unsigned char, std::set<unsigned char>>(point_it->id, std::set<unsigned char>()));
		}
		attachment_point_validity_template.insert(std::pair<unsigned char, std::map<unsigned char, std::set<unsigned char>>>(prefab_it->get_object_prefab_id(), attachment_points));
	}
	log_main.print("generate attachment_point_validity_template\n");

	// instantiate attachment_point_validity in cnodes
	for (int i = 0; i < iNUM_ORIENT; ++i)
	{
		for (std::vector<CSNode>::iterator cnode_it = m_cspace._cspace[i].begin(); cnode_it != m_cspace._cspace[i].end(); ++cnode_it)
		{
			cnode_it->_attachment_point_validity = attachment_point_validity_template;
		}
	}
	log_main.print("instantiate attachment_point_validity\n");

	// initiate validities
	for (std::set<int>::iterator it = prefab_ids.begin(); it != prefab_ids.end(); ++it)
	{
		m_cspace.init_validity(*it, false);
	}
	log_main.print("initiate validities\n");

	// set validities
	for (std::vector<ObjectPrefab>::iterator prefab_it = m_prefabs.begin(); prefab_it != m_prefabs.end(); ++prefab_it)
	{
		int prefab_id = prefab_it->get_object_prefab_id();
		std::set<cnode_pos> nodes_set;

		// if prefab has been instantiated
		if (prefab_ids.find(prefab_id) != prefab_ids.end())
		{
			log_main.print("Set validities for prefab_id %d:", prefab_id);

			// create landing curves
			for (landing& l : m_EX_landings)
			{
				l.construct_bezier(prefab_id, prefab_it->get_landing_entrance().first, prefab_it->get_landing_entrance().second, 20);
			}

			// vector of 16 polygons for each orientation
			std::vector<polygon> polygons;
			polygon poly = prefab_it->get_polygon();
			polygons.push_back(poly);
			for (int i = 1; i < iNUM_ORIENT; ++i)
			{
				poly.rotate(fLAYER_GAP);
				polygons.push_back(poly);
			}

			// vector of 16 std::vector<attachment_point>
			std::vector<std::vector<attachment_point>> points;
			std::vector<attachment_point> pnt = prefab_it->get_extern_attachment_points();
			points.push_back(pnt);
			for (int i = 1; i < iNUM_ORIENT; ++i)
			{
				for (std::vector<attachment_point>::iterator it = pnt.begin(); it != pnt.end(); ++it)
				{
					it->rotate(fLAYER_GAP);
				}
				points.push_back(pnt);
			}

			int count1 = 0;
			int count2 = 0;
			int num_valid = 0;
			int num_invalid = 0;
			int num_nodes = iNUM_ORIENT * m_cspace.size(0);

			for (int o = 0; o < iNUM_ORIENT; ++o)
			{
				for (int i = 0; i < m_cspace.size(o); ++i)
				{
					cnode_pos cnode(i, o);
					CSNode* n_pt = get_node(cnode);
					if (!n_pt) continue;

					// set validity
					if ( set_validity(cnode, &(*prefab_it), polygons[o], points[o], n_pt->get_floor_num(), n_pt->_stair_id) ) // valid
					{
						++num_valid;

						// set LD cspace to valid
						m_cspace.LD_set_to_valid(cnode, prefab_id);

						// set landing distance
						set_landing_distance(n_pt, &(*prefab_it));
						
					}
					else // invalid or already tested
					{
						++num_invalid;
					}

					if (count2 >= 997)
					{
						log_main.print("Pass %d:", count1);
						log_main.print(4,"num valid:   %d", num_valid);
						log_main.print(4,"num invalid: %d", num_invalid);
						log_main.print(4,"complete:    %d%%", 100*(num_valid+num_invalid)/num_nodes);

						count2 = 0;
					}
					++count1;
					++count2;
				}
			}
			
			log_main.print("Pass %d:", count1);
			log_main.print(4,"num valid:      %d", num_valid);
			log_main.print(4,"num invalid:    %d", num_invalid);

			num_valid = 0;
			num_invalid = 0;
			for (int i = 0; i < iNUM_ORIENT; ++i)
			{
				for (std::vector<CSNode>::iterator it = m_cspace._cspace[i].begin(); it != m_cspace._cspace[i].end(); ++it)
				{
					if (it->_valid[prefab_id])
					{
						++num_valid;
					}
					else
					{
						++num_invalid;
					}
				}
			}
			log_main.print("Total:");
			log_main.print("    num valid:      %d", num_valid);
			log_main.print("    num invalid:    %d", num_invalid);
		}
	}

	set_all_stair_nodes();
	freespace_ready = true;

	output_setup_data();
}
bool ObjectSpace::set_validity(cnode_pos cnode, ObjectPrefab* prefab, polygon poly, std::vector<attachment_point> points, int floor, int stair_id)
{
	int id = prefab->get_object_prefab_id();
	if (m_cspace.get_cnode_pt(cnode)->_valid[id]) // already set to valid
	{
		return false;
	}

	//translate polygon and attachment points
	poly.translate(m_cspace.get_cnode_pt(cnode)->get_position());
	for (std::vector<attachment_point>::iterator it = points.begin(); it != points.end(); ++it)
	{
		it->translate(m_cspace.get_cnode_pt(cnode)->get_position());
	}

	//get boundary lines on this floor (or stairway)
	std::vector<start_end<x_y<float>>> bounds;
	if (stair_id != -1)
	{
		stair* s = get_stairpt_from_id(stair_id);
		if (s)
		{
			bounds.push_back(s->left_wall);
			bounds.push_back(s->right_wall);
		}
	}
	else
	{
		for (boundary_line& bl : m_bounds)
		{
			if (bl.floor_num == floor)
			{
				bounds.push_back(bl.position);
			}
		}
	}

	// set validity of cnode for prefab and return true if valid, false if invalid
	/// test polygon
	for (start_end<x_y<float>>& pos : bounds)
	{
		if (poly.does_line_intersect(pos)) // invalid
		{
			return false;
		}
	}

	std::map<unsigned char, std::set<unsigned char>> invalid_points; //map<attachment_point_id, set<invalid_circles>>
	for (std::vector<attachment_point>::iterator pnt = points.begin(); pnt != points.end(); ++pnt)
	{
		pnt->occupation_circles.invalid_points.clear();
		for (start_end<x_y<float>>& pos : bounds)
		{
			if (pnt->occupation_circles.line_intersect(pos)) // line intersects (invalid)
			{
				return false;
			}
		}
		invalid_points.insert(std::pair<unsigned char, std::set<unsigned char>>(pnt->id,pnt->occupation_circles.invalid_points));
	}

	// valid
	m_cspace.get_cnode_pt(cnode)->_valid[id]= true;
	m_cspace.get_cnode_pt(cnode)->_attachment_point_validity.at(id) = invalid_points;
	return true;
}
unsigned char ObjectSpace::get_stair_type(float stair_direction, float arc_angle, float tolarence)
{
	arc_angle = 180.0f - arc_angle;

	float diff = arc_angle - stair_direction;
	while (diff < -180.0f)
		diff += 360.0f;
	while (diff > 180.0f)
		diff -= 360.0f;
	diff = abs(diff);

	if (diff <= tolarence) return STAIR_ARC_UP;
	if (diff < 90.0f - tolarence) return STAIR_ARC_SIDEUP;
	if (diff <= 90.0f + tolarence) return STAIR_ARC_SIDE;
	if (diff < 180.0f - tolarence) return STAIR_ARC_SIDEDOWN;
	return STAIR_ARC_DOWN;
}
void ObjectSpace::set_all_stair_nodes()
{
	float tolarence = 11.25f; // angle tolarence for working out stair_type

	std::vector<int> prefab_ids;
	for (ObjectPrefab& pre : m_prefabs)
	{
		prefab_ids.push_back(pre.get_object_prefab_id());
	}

	const std::map<int, std::map<int, std::vector<cnode_pos>>> prefab_stair_node = m_cspace.init_stairs(prefab_ids);

	for (ObjectPrefab& p : m_prefabs)
	{
		int prefab_id = p.get_object_prefab_id();
		auto stair_node = prefab_stair_node.find(prefab_id);
		if (stair_node == prefab_stair_node.end()) continue;
		
		// get 16 poly for prefab_id (in object coords)
		std::vector<polygon> polygons; // polygons[o] is the polygon at orientation o
		polygon poly = p.get_polygon();
		polygons.push_back(poly);
		for (int i = 1; i < iNUM_ORIENT; ++i)
		{
			poly.rotate(fLAYER_GAP);
			polygons.push_back(poly);
		}

		for (std::pair<const int, std::vector<cnode_pos>> s : stair_node->second)
		{
			int stair_id = s.first;
			stair* stairpt = get_stairpt_from_id(stair_id);
			if (!stairpt) continue;

			std::vector<cnode_pos> open_nodes = s.second;

			// get top and bottom lines of stair
			start_end<x_y<float>> top_line = stairpt->get_top_line(true);
			start_end<x_y<float>> bot_line = stairpt->get_bottom_line(true);

			// loop through open_nodes setting arc_stair type of all arcs and makeing cnodes that intersect with top or bottom line part of the stair
			while (!open_nodes.empty())
			{
				cnode_pos this_node = open_nodes.back();
				open_nodes.pop_back();

				CSNode* this_node_pt = get_node(this_node);
				std::vector<CSArc*> opposite_arcs = get_opposed_arcs(this_node_pt);
				
				CSNode* this_LD_node_pt = m_cspace.LD_get_cnode_pt(this_node);
				std::vector<CSArc*> opposite_LD_arcs = get_opposed_LD_arcs(this_LD_node_pt);

				// loop through all arcs on node
				int arc_pos = -1;
				for (CSArc& this_arc : this_node_pt->_arcs)
				{
					++arc_pos;
					// if stair_dir is false, set arc_stair type based on angle and stair direction (turn on spot --> STAIR_ARC_SIDE)
					if (this_arc.stair_dir != NOT_STAIR_ARC) continue;
					
					// find stair_type
					unsigned char stair_type = STAIR_ARC_SIDE;
					if (!this_arc.turn_on_spot)
					{
						stair_type = get_stair_type(stairpt->direction, this_arc.angle, tolarence);
					}
					unsigned char opp_stair_type = invert_stair_type(stair_type);

					// set stair_type
					this_arc.stair_dir = stair_type;
					if (!opposite_arcs.empty()) opposite_arcs[arc_pos]->stair_dir = opp_stair_type;

					if (stair_type != NOT_STAIR_ARC && arc_pos < this_LD_node_pt->_arcs.size() && this_node_pt->_stair_id != -1)
					{
						this_LD_node_pt->_arcs[arc_pos].stair_dir = stair_type;
						if (!opposite_LD_arcs.empty()) opposite_LD_arcs[arc_pos]->stair_dir = opp_stair_type;
					}

					// find node at other end of arc
					cnode_pos other_node = this_arc.cnode_to_vec_pos;
					CSNode* other_node_pt = get_node(other_node);
					if (!other_node_pt || !other_node_pt->_valid[prefab_id]) continue;

					if (other_node_pt->_stair_ids[prefab_id] == -1)
					{
						polygon poly = polygons[other_node.orientation];
						poly.translate(other_node_pt->get_position());
						// if object (without attendants) would overlap with stair top or bottom for this config
						if (poly.does_line_intersect(top_line)
							|| poly.does_line_intersect(bot_line))
						{
							// set stair_id on this node and add it to open_nodes
							other_node_pt->_stair_ids[prefab_id] = stair_id;
							open_nodes.push_back(other_node);
						}
					}
				}
			}
		}
	}
}
std::vector<CSArc*> ObjectSpace::get_opposed_arcs(CSNode* node)
{
	std::vector<CSArc*> opp_arcs;
	cnode_pos this_pos = node->get_vec_pos();

	for (CSArc& arc_for : node->_arcs)
	{
		bool arc_not_found = true;
		CSNode* other_node = get_node(arc_for.cnode_to_vec_pos);
		for (CSArc& arc_back : other_node->_arcs)
		{
			if (arc_back.cnode_to_vec_pos == this_pos)
			{
				opp_arcs.push_back(&arc_back);
				arc_not_found = false;
				break;
			}
		}
		if (arc_not_found) return std::vector<CSArc*>();
	}

	if (opp_arcs.size() == node->_arcs.size())
		return opp_arcs;
	
	return std::vector<CSArc*>();
}
std::vector<CSArc*> ObjectSpace::get_opposed_LD_arcs(CSNode* node)
{
	std::vector<CSArc*> opp_arcs;
	cnode_pos this_pos = node->get_vec_pos();

	for (CSArc& arc_for : node->_arcs)
	{
		bool arc_not_found = true;
		
		CSNode* other_node = get_LD_node(arc_for.cnode_to_vec_pos.vec_pos);
		for (CSArc& arc_back : other_node->_arcs)
		{
			if (arc_back.cnode_to_vec_pos == this_pos)
			{
				opp_arcs.push_back(&arc_back);
				arc_not_found = false;
				break;
			}
		}
		if (arc_not_found) return std::vector<CSArc*>();
	}

	if (opp_arcs.size() == node->_arcs.size())
		return opp_arcs;

	return std::vector<CSArc*>();
}
void ObjectSpace::set_landing_distance(CSNode* node_pt, ObjectPrefab* prefab_pt)
{
	if (node_pt->landing_id == -1) return;
	landing* land = get_landing_pt(node_pt->landing_id);
	if (!land) return;

	float dist = land->get_distance_to_curve(prefab_pt->get_object_prefab_id(), node_pt->get_position(), node_pt->get_theta(), prefab_pt->get_max_linear_speed(), prefab_pt->get_max_angular_speed());

	if (dist < 0.0f) return;
	
	node_pt->_landing_dist_curve[prefab_pt->get_object_prefab_id()] = dist;
}
GenObject* ObjectSpace::get_object(int object_id)
{
	for (std::vector<GenObject>::iterator it = m_objects.begin(); it != m_objects.end(); ++it)
	{
		if (it->get_object_id() == object_id)
		{
			return &(*it);
		}
	}
	return nullptr;
}
person* ObjectSpace::get_person(int person_id)
{
	if (m_people.find(person_id) == m_people.end()) return nullptr;

	return &m_people[person_id];
}
person* ObjectSpace::get_closest_person(vector2 pos, int floor, bool has_to_be_available)
{
	float closest_dist_sq = INFINITY;
	int closest_person_id = -1;

	for (auto const& p : m_people)
	{
		if (!p.second.active || p.second.floor_id != floor || (has_to_be_available && p.second.in_object)) continue;

		float dist_sq = (p.second.position - pos).magnitude_squared();

		if (dist_sq < closest_dist_sq)
		{
			closest_dist_sq = dist_sq;
			closest_person_id = p.second.id;
		}
	}

	if (closest_person_id == -1) return nullptr;

	return &m_people[closest_person_id];
}

// draw nodes / arcs / bound from cspace
int ObjectSpace::get_num_bounds()
{
	return m_EX_bounds.size();
}
boundary_line ObjectSpace::get_bound_line(int pos)
{
	return m_EX_bounds[pos];
}
int ObjectSpace::get_num_tnodes()
{
	return m_EX_nodes.size();
}
node ObjectSpace::get_tnode(int pos)
{
	if (pos >= m_EX_nodes.size()) return m_EX_nodes.back();

	return m_EX_nodes[pos];
}
node* ObjectSpace::get_tnode_pnt(int id)
{
	for (node& n : m_EX_nodes)
	{
		if (n.id == id)
		{
			return &n;
		}
	}
	return 0;
}
int ObjectSpace::get_num_tarcs()
{
	return m_EX_arcs.size();
}
arc ObjectSpace::get_tarc(int pos)
{
	if (pos < m_EX_arcs.size())
		return m_EX_arcs[pos];
	else
		return m_EX_arcs[m_EX_arcs.size() - 1];
}
int ObjectSpace::get_num_stairs()
{
	return m_EX_stairs.size();
}
stair ObjectSpace::get_stair(int pos_in_vec)
{
	return m_EX_stairs[pos_in_vec];
}
int ObjectSpace::get_num_landings()
{
	return m_EX_landings.size();
}
landing ObjectSpace::get_landing(int pos_in_vec)
{
	return m_EX_landings[pos_in_vec];
}
landing* ObjectSpace::get_landing_pt(int landing_id)
{
	for (landing& l : m_EX_landings)
	{
		if (l.id == landing_id) return &l;
	}
	return 0;
}
stair* ObjectSpace::get_stairpt_from_id(int id)
{
	if (id < 0) return 0;

	for (stair& s : m_EX_stairs)
	{
		if (s.id == id)
		{
			return &s;
		}
	}

	return 0;
}
stair* ObjectSpace::get_stairpt(float x, float y, float floor_num)
{
	for (int i = m_EX_stairs.size() - 1; i >= 0; --i)
	{
		float tolarence = m_EX_stairs[i].width / 2.0f;
		if ((m_EX_stairs[i].floor_num == floor_num || m_EX_stairs[i].floor_num + 1 == floor_num ) &&
			m_EX_stairs[i].position.x > x - tolarence &&
			m_EX_stairs[i].position.x < x + tolarence &&
			m_EX_stairs[i].position.y > y - tolarence &&
			m_EX_stairs[i].position.y < y + tolarence)
		{
			return &m_EX_stairs[i];
		}
	}
	return nullptr;
}
int ObjectSpace::get_num_obj()
{
	return m_objects.size();
}
int ObjectSpace::get_num_people()
{
	return m_people.size();
}
const node* ObjectSpace::get_tnode(float x, float y, float tolarence, int floor)
{
	for (int i = 0; i < m_EX_nodes.size(); ++i)
	{
		if ((m_EX_nodes[i].floor_num == floor || (m_EX_nodes[i].stair_id != -1 && m_EX_nodes[i].floor_num + 1 == floor)) &&
			m_EX_nodes[i].position.x > x - tolarence &&
			m_EX_nodes[i].position.x < x + tolarence &&
			m_EX_nodes[i].position.y > y - tolarence &&
			m_EX_nodes[i].position.y < y + tolarence)
		{
			return &m_EX_nodes[i];
		}
		
	}
	return nullptr;
}
const node* ObjectSpace::get_tnode(float x, float y, float tolarence, int floor, int start_search_pos)
{
	for (int i = start_search_pos; i < m_EX_nodes.size(); ++i)
	{
		if ((m_EX_nodes[i].floor_num == floor || (m_EX_nodes[i].stair_id != -1 && m_EX_nodes[i].floor_num + 1 == floor)) &&
			m_EX_nodes[i].position.x > x - tolarence &&
			m_EX_nodes[i].position.x < x + tolarence &&
			m_EX_nodes[i].position.y > y - tolarence &&
			m_EX_nodes[i].position.y < y + tolarence)
		{
			return &m_EX_nodes[i];
		}

	}
	return nullptr;
}
const int* ObjectSpace::select_tnode(float x, float y, float tolarence, const int* ignore, int floor)
{
	for (int i = m_EX_nodes.size() - 1; i >= 0; --i)
	{
		if (ignore == nullptr)
		{
			if (m_EX_nodes[i].floor_num == floor &&
				m_EX_nodes[i].position.x > x - tolarence &&
				m_EX_nodes[i].position.x < x + tolarence &&
				m_EX_nodes[i].position.y > y - tolarence &&
				m_EX_nodes[i].position.y < y + tolarence)
			{
				return &(m_EX_nodes[i].id);
			}
		}
		else if (m_EX_nodes[i].id != *ignore)
		{
			if (m_EX_nodes[i].floor_num == floor &&
				m_EX_nodes[i].position.x > x - tolarence &&
				m_EX_nodes[i].position.x < x + tolarence &&
				m_EX_nodes[i].position.y > y - tolarence &&
				m_EX_nodes[i].position.y < y + tolarence)
			{
				return &(m_EX_nodes[i].id);
			}
		}
	}
	return nullptr;
}
const int* ObjectSpace::select_tarc(float x, float y, float tolarence, const int* ignore, int floor)
{
	for (int i = m_EX_arcs.size() - 1; i >= 0; --i)
	{
		if (ignore == nullptr)
		{
			float xp = (m_EX_arcs[i].position.start.x + m_EX_arcs[i].position.end.x) / 2.0f;
			float yp = (m_EX_arcs[i].position.start.y + m_EX_arcs[i].position.end.y) / 2.0f;
			if ((m_EX_arcs[i].floor_num.start == floor || m_EX_arcs[i].floor_num.end == floor) &&
				xp > x - tolarence &&
				xp < x + tolarence &&
				yp > y - tolarence &&
				yp < y + tolarence)
			{
				return &(m_EX_arcs[i].id);
			}
		}
		else if (m_EX_arcs[i].id != *ignore)
		{
			float xp = (m_EX_arcs[i].position.start.x + m_EX_arcs[i].position.end.x) / 2.0f;
			float yp = (m_EX_arcs[i].position.start.y + m_EX_arcs[i].position.end.y) / 2.0f;
			if ((m_EX_arcs[i].floor_num.start == floor || m_EX_arcs[i].floor_num.end == floor) &&
				xp > x - tolarence &&
				xp < x + tolarence &&
				yp > y - tolarence &&
				yp < y + tolarence)
			{
				return &(m_EX_arcs[i].id);
			}
		}
	}
	return nullptr;
}
const int* ObjectSpace::select_bound(float x, float y, float tolarence, const int* ignore, int floor)
{
	for (int i = m_EX_bounds.size() - 1; i >= 0; --i)
	{
		if (ignore == nullptr)
		{
			float xp = (m_EX_bounds[i].position.start.x + m_EX_bounds[i].position.end.x) / 2.0f;
			float yp = (m_EX_bounds[i].position.start.y + m_EX_bounds[i].position.end.y) / 2.0f;
			if (m_EX_bounds[i].floor_num == floor &&
				xp > x - tolarence &&
				xp < x + tolarence &&
				yp > y - tolarence &&
				yp < y + tolarence)
			{
				return &(m_EX_bounds[i].id);
			}
		}
		else if (m_EX_bounds[i].id != *ignore)
		{
			float xp = (m_EX_bounds[i].position.start.x + m_EX_bounds[i].position.end.x) / 2.0f;
			float yp = (m_EX_bounds[i].position.start.y + m_EX_bounds[i].position.end.y) / 2.0f;
			if (m_EX_bounds[i].floor_num == floor &&
				xp > x - tolarence &&
				xp < x + tolarence &&
				yp > y - tolarence &&
				yp < y + tolarence)
			{
				return &(m_EX_bounds[i].id);
			}
		}
	}
	return nullptr;
}
int ObjectSpace::get_next_node_id()
{
	return m_next_node_id;
}
int ObjectSpace::get_next_arc_id()
{
	return m_next_arc_id;
}
int ObjectSpace::get_next_bound_id()
{
	return m_next_bound_id;
}
int ObjectSpace::get_next_stair_id()
{
	return m_next_stair_id;
}
int ObjectSpace::get_next_landing_id()
{
	return m_next_landing_id;
}
int ObjectSpace::get_next_prefab_id()
{
	return m_next_prefab_id;
}
int ObjectSpace::get_next_object_id()
{
	return m_next_object_id;
}
std::vector<GenObject> ObjectSpace::get_objects()
{
	std::vector<GenObject> out = m_objects;

	for (auto& obj : m_objects)
	{
		obj.just_moved = false;
	}

	return out;
}
std::vector<ObjectPrefab> ObjectSpace::get_prefabs()
{
	return m_prefabs;
}
std::map<int, person> ObjectSpace::get_people()
{
	return m_people;
}
GenObject ObjectSpace::get_object_from_vec(int vec_pos)
{
	return m_objects[vec_pos];
}

// stair / landing
void ObjectSpace::auto_landing_nodes(int landing_id)
{
	landing* land = get_landing_pt(landing_id);
	if (!land) return;

	float floor = land->floor;
	polygon poly = land->get_polygon_boundary();

	for (node& n : m_EX_nodes)
	{
		if (n.floor_num == floor && n.stair_id == -1 && poly.is_point_inside(n.position))
		{
			set_landing_id(n, landing_id);
		}
	}
}
void ObjectSpace::auto_all_landing_nodes()
{
	std::map<landing*, polygon> polys;
	for (landing& land : m_EX_landings)
	{
		polys[&land] = land.get_polygon_boundary();
	}

	for (node& n : m_EX_nodes)
	{
		if (n.stair_id == -1)
		{
			n.landing_id = -1;
			for (auto& p : polys)
			{
				if (p.first->floor == n.floor_num && p.second.is_point_inside(n.position))
				{
					set_landing_id(n, p.first->id);
					break;
				}
			}

		}
	}
}
bool ObjectSpace::is_node_in_landing_area(int node_id, int landing_id)
{
	landing* l = get_landing_pt(landing_id);
	if (!l) return false;

	node* n = get_tnode_pnt(node_id);
	if (!n) return false;

	if (l->floor != n->floor_num) return false;

	return l->get_polygon_boundary().is_point_inside(n->position);
}
void ObjectSpace::set_landing_id(node& n, int land_id)
{
	n.landing_id = land_id;
	int node_id = n.id;

	for (arc& a : m_EX_arcs)
	{
		if (a.tnode_ids.start == node_id || a.tnode_ids.end == node_id)
		{
			a.land_id = land_id;
		}
	}
}
void ObjectSpace::set_all_arc_landing_ids()
{
	for (arc& a : m_EX_arcs)
	{
		node* ns = get_tnode_pnt(a.tnode_ids.start);
		node* ne = get_tnode_pnt(a.tnode_ids.end);

		int land = -1;
		if (ns)
		{
			land = max(land, ns->landing_id);
		}
		if (ne)
		{
			land = max(land, ne->landing_id);
		}

		a.land_id = land;
	}
}
void ObjectSpace::auto_gen_landings()
{
	std::map<int, bool> used_up_stairs; // bottom of stair is on landing
	for (const stair& s : m_EX_stairs)
	{
		used_up_stairs[s.id] = false;
	}
	std::map<int, bool> used_down_stairs = used_up_stairs; // top of stair is on landing

	for (const landing& land : m_EX_landings)
	{
		used_up_stairs[land.up_stair_id] = true;
		used_down_stairs[land.down_stair_id] = true;
	}

	int land_id = get_next_landing_id();

	// up stairs
	float max_depth = 5.0f;
	for (stair& this_stair : m_EX_stairs)
	{
		for (int i = 0; i < 2; ++i)
		{
			bool this_stair_up = i;
			if (this_stair_up)
			{
				if (used_up_stairs[this_stair.id]) continue;
			}
			else
			{
				if (used_down_stairs[this_stair.id]) continue;
			}

			int other_stair_id = -1;
			std::vector<node*> landing_nodes = get_landing_nodes(this_stair, !this_stair_up, other_stair_id);

			stair* other_stair = get_stairpt_from_id(other_stair_id);
			if (!other_stair) continue;
			if (this_stair_up)
			{
				if (used_down_stairs[other_stair->id]) continue;
			}
			else
			{
				if (used_up_stairs[other_stair->id]) continue;
			}

			int land_floor = this_stair_up ? this_stair.floor_num : other_stair->floor_num;

			if (landing_nodes.empty() || !other_stair) continue;

			// find opposite wall for landing
			start_end<x_y<float>> this_left = this_stair.left_wall;
			start_end<x_y<float>> this_right = this_stair.right_wall;
			start_end<x_y<float>> other_left = other_stair->left_wall;
			start_end<x_y<float>> other_right = other_stair->right_wall;

			if (this_stair_up)
			{
				other_left.invert_this();
				other_right.invert_this();
			}
			else
			{
				this_left.invert_this();
				this_right.invert_this();
			}

			/// Now all left and right walls start at entrance to landing and go away from landing

			float tl_ol = distsq_x_y(this_left.start, other_left.start);
			float tl_or = distsq_x_y(this_left.start, other_right.start);
			float tr_ol = distsq_x_y(this_right.start, other_left.start);
			float tr_or = distsq_x_y(this_right.start, other_right.start);

			int largest = get_largest(tl_ol, tl_or, tr_ol, tr_or);

			start_end<x_y<float>>& this_proj = largest == 0 || largest == 1 ? this_left : this_right;
			start_end<x_y<float>>& other_proj = largest == 0 || largest == 2 ? other_left : other_right;

			vector2 this_v = this_proj.end - this_proj.start;
			vector2 other_v = other_proj.end - other_proj.start;

			this_v.resize(-max_depth);
			other_v.resize(-max_depth);

			this_proj.end = this_proj.start + this_v;
			other_proj.end = other_proj.start + other_v;

			for (const boundary_line& b : m_bounds)
			{
				if (b.floor_num != land_floor) continue;

				intersection(this_proj, b.position, this_proj.end);
				intersection(other_proj, b.position, other_proj.end);
			}

			start_end<x_y<float>> opposite_wall = { this_proj.end, other_proj.end };
			if (!this_stair_up) opposite_wall.invert_this();

			// construct landing
			TCP_add_landing(landing(land_id, &this_stair, other_stair, opposite_wall));

			for (node*& land_node : landing_nodes)
			{
				set_landing_id(*land_node, land_id);
			}

			// set values
			++land_id;
			if (this_stair_up)
			{
				used_up_stairs[this_stair.id] = true;
				used_down_stairs[other_stair->id] = true;
			}
			else
			{
				used_up_stairs[other_stair->id] = true;
				used_down_stairs[this_stair.id] = true;
			}
		}
	}
}
std::vector<node*> ObjectSpace::get_landing_nodes(const stair& this_stair, bool top, int& other_stair)
{
	std::vector<int> bot_stair_nodes = this_stair.get_top_bot_nodes(top);

	std::vector<node*> open_nodes;
	std::vector<node*> closed_nodes;

	// find all landing nodes connected to bottom of stair
	for (int& t_id : bot_stair_nodes)
	{
		node* this_node = get_tnode_pnt(t_id);
		if (!this_node) continue;

		std::vector<int> con_nodes = this_node->tnodes;

		for (int& o_id : con_nodes)
		{
			node* other_node = get_tnode_pnt(o_id);
			if (!other_node) continue;

			if (other_node->stair_id == -1 && other_node->landing_id == 0)
			{
				open_nodes.push_back(other_node);
			}
		}
	}

	REMOVE_DUPLICATES(open_nodes);

	while (!open_nodes.empty())
	{
		node* back_node = open_nodes.back();
		open_nodes.pop_back();
		closed_nodes.push_back(back_node);

		for (int& other_node_id : back_node->tnodes)
		{
			node* other_node = get_tnode_pnt(other_node_id);
			if (!other_node) continue;
			if (other_node->stair_id != -1)
			{
				if (other_node->stair_id != this_stair.id) other_stair = other_node->stair_id;

				continue;
			}
			if (other_node->landing_id != 0 // other node not landing
				|| IN_VECTOR_FUNC(open_nodes, node*, == other_node) || IN_VECTOR_FUNC(closed_nodes, node*, == other_node) // other node already in open or closed nodes
				)
				continue;

			open_nodes.push_back(other_node);
		}
	}

	return closed_nodes;
}

// File IO
/// geo
void ObjectSpace::save(const char* filename, float zoom, float camerax, float cameray)
{
	log_main.print("File saving...");

	bool success = FileIO::save_geometry(filename, m_EX_nodes, m_EX_arcs, m_EX_bounds, m_EX_stairs, m_EX_landings, zoom, camerax, cameray);

	success ? log_main.print("File saved successfully") : log_main.print("File could not save!");
}
void ObjectSpace::load(const char* filename, float* zoom, float* camerax, float* cameray)
{
	//create node, arc and bound constructor vectors
	std::vector<FileIO::Node_construct> nodes;
	std::vector<FileIO::Arc_construct> arcs;
	std::vector<FileIO::BoundaryLine_construct> bounds;
	std::vector<FileIO::Stair_construct> stairs;
	std::vector<FileIO::Landing_construct> landings;

	//populate vectors with contents of file
	log_main.print("Reading file...");
	bool file_exists = FileIO::read_file(filename, &nodes, &arcs, &bounds, &stairs, &landings, zoom, camerax, cameray);

	if (!file_exists)
	{
		log_main.print("Failed to read file!");
		return;
	}
	log_main.print("File read");

	//clear node, arc and bound vectors
	TCP_clear_geometry_vec();

	//nodes
	for (auto it = nodes.begin(); it != nodes.end(); ++it)
		TCP_add_node(node(it->id, it->xPos, it->yPos, it->landingid, it->stairid, it->floor));

	//arcs
	for (auto it = arcs.begin(); it != arcs.end(); ++it)
		TCP_add_arc(arc(-1,-1,it->start_node, it->end_node, it->start_x, it->start_y, it->end_x, it->end_y, 1, 1, it->id));

	//bounds
	for (auto it = bounds.begin(); it != bounds.end(); ++it)
		TCP_add_bound(boundary_line(it->startX, it->startY, it->endX, it->endY, it->floor, it->id));

	//stairs
	for (auto it = stairs.begin(); it != stairs.end(); ++it)
		TCP_add_stair(stair(it->id, it->xposition, it->yposition, it->direction, it->lanes, it->riser_num, it->length, it->width, it->height, it->floor_num), false);

	//landings
	for (auto it = landings.begin(); it != landings.end(); ++it)
	{
		stair* s1 = get_stairpt_from_id(it->up_stair_id); if (!s1) break;
		stair* s2 = get_stairpt_from_id(it->down_stair_id); if (!s2) break;
		TCP_add_landing(landing(it->id, s1, s2, { {it->wall_start_x,it->wall_start_y},{it->wall_end_x,it->wall_end_y} }));
	}

	reset_all_arc_land_stair_ids();
}
void ObjectSpace::reset_all_arc_land_stair_ids()
{
	for (arc& a : m_EX_arcs)
	{
		node* ns = get_tnode_pnt(a.tnode_ids.start);
		node* ne = get_tnode_pnt(a.tnode_ids.end);

		int land = -1;
		if (ns)
		{
			land = max(land, ns->landing_id);
			a.stair_ids.start = ns->stair_id;
			a.floor_num.start = ns->floor_num;
		}
		else
		{
			a.stair_ids.start = -1;
		}

		if (ne)
		{
			land = max(land, ne->landing_id);
			a.stair_ids.end = ne->stair_id;
			a.floor_num.end = ne->floor_num;
		}
		else
		{
			a.stair_ids.end = -1;
		}

		a.land_id = land;
	}
}
/// setup data
void ObjectSpace::output_setup_data()
{
	log_main.print("Saving setup data...");

	if (data_adj_matrix)
	{
		output_tnode_adj_matrix("data_setup_tnode_adj.csv");
		output_cnode_adj_matrix("data_setup_cnode_adj.csv");
	}
	if (data_adj_list)
	{
		output_tnode_adj_list("data_setup_tnode_adj.csv");
		output_cnode_adj_list("data_setup_cnode_adj.csv");
	}
	if (data_tnode_arc_info)
	{
		output_tnode_tarc_info("data_setup_tnode_info.csv");
	}
	if (data_cnode_arc_info)
	{
		output_cnode_carc_info("data_setup_cnode_info.csv");
	}
	if (data_object_info)
	{
		output_setup_object_info("data_setup_object_info.csv");
	}

	log_main.print("Setup data output complete");
}
void ObjectSpace::output_tnode_adj_matrix(const char* filename)
{
	std::vector<int> nodes;
	std::vector<bool> adj;

	for (const node& n : m_EX_nodes)
	{
		nodes.push_back(n.id);
	}

	int num_nodes = nodes.size();

	// init adj
	for (int i = 0; i < num_nodes*num_nodes; ++i)
	{
		adj.push_back(false);
	}

	// fill adj
	for (const arc& a : m_EX_arcs)
	{
		int start_id = a.tnode_ids.start;
		int end_id = a.tnode_ids.end;

		int start_pos = -1;
		bool start_not_found = true;
		int end_pos = -1;
		bool end_not_found = true;
		for (int pos = 0; pos < num_nodes; ++pos)
		{
			int this_id = nodes[pos];

			if (start_not_found && this_id == start_id)
			{
				start_pos = pos;
				start_not_found = false;
			}
			if (end_not_found && this_id == end_id)
			{
				end_pos = pos;
				end_not_found = false;
			}

			if (!start_not_found && !end_not_found)
				break;
		}

		if (!end_not_found && !start_not_found)
		{
			adj[start_pos*num_nodes + end_pos] = true;
			adj[end_pos*num_nodes + start_pos] = true;
		}
	}


	// output to file
	if (FileIO::data_output_node_adj_matrix(filename, nodes, adj))
		log_main.print("tnode adjacency matrix saved to csv file");
	else
	{
		log_main.print("tnode adjacency matrix could not be saved");
	}
}
void ObjectSpace::output_cnode_adj_matrix(const char* filename)
{
	std::vector<int> nodes;
	std::vector<bool> adj;

	int num_nodes = m_cspace._LD_cspace.size();

	for (int i = 0; i < num_nodes*num_nodes; ++i)
	{
		adj.push_back(false);
	}
	
	// arcs go from top line to left line
	for (const CSNode& n : m_cspace._LD_cspace)
	{
		int this_pos = n.get_vec_pos().vec_pos;
		nodes.push_back(this_pos);

		for (const CSArc& a : n._arcs)
		{
			int other_pos = a.cnode_to_vec_pos.vec_pos;

			adj[this_pos + num_nodes * other_pos] = true;
		}
	}

	// output to file
	if (FileIO::data_output_node_adj_matrix(filename, nodes, adj))
		log_main.print("cnode adjacency matrix saved to csv file");
	else
	{
		log_main.print("cnode adjacency matrix could not be saved");
	}
		
}
void ObjectSpace::output_tnode_adj_list(const char* filename)
{
	std::map<int, std::vector<int>> node_adj;

	for (const arc& a : m_EX_arcs)
	{
		node_adj[a.tnode_ids.start].push_back(a.tnode_ids.end);
		node_adj[a.tnode_ids.end].push_back(a.tnode_ids.start);
	}

	// output to file
	if (FileIO::data_output_node_adj_list(filename, node_adj))
		log_main.print("tnode adjacency list saved to csv file");
	else
	{
		log_main.print("tnode adjacency list could not be saved");
	}
}
void ObjectSpace::output_cnode_adj_list(const char* filename)
{
	cnode_pos p;
	std::map<int, std::vector<int>> node_adj;

	int layer_size = m_cspace.size(0);
	for (const auto& layer : m_cspace._cspace)
	{
		for (const CSNode& n : layer)
		{
			auto& n_adj = node_adj[n.get_vec_pos().abs_pos(layer_size)];
			for (const CSArc& a : n._arcs)
			{
				n_adj.push_back(a.cnode_to_vec_pos.abs_pos(layer_size));
			}
		}
	}

	// output to file
	if (FileIO::data_output_node_adj_list(filename, node_adj))
		log_main.print("cnode adjacency list saved to csv file");
	else
	{
		log_main.print("cnode adjacency list could not be saved");
	}
}
void ObjectSpace::output_cnode_carc_info(const char* filename)
{
	// output to file
	if (FileIO::data_output_cnode_carc_info(filename, m_cspace._cspace))
		log_main.print("cnode and carc info saved to csv file");
	else
	{
		log_main.print("cnode and carc info could not be saved");
	}
}
void ObjectSpace::output_tnode_tarc_info(const char* filename)
{
	// output to file
	if (FileIO::data_output_tnode_tarc_info(filename, m_EX_nodes, m_EX_arcs))
		log_main.print("tnode and tarc info saved to csv file");
	else
	{
		log_main.print("tnode and tarc info could not be saved");
	}
}
void ObjectSpace::output_setup_object_info(const char * filename)
{
	// output to file
	if (FileIO::data_output_object_info(filename, true, 0, 0, 0, 0.0f, m_objects))
	{
		log_main.print("object info saved to csv file");
	}
	else
	{
		log_main.print("object info could not be saved");
	}
}
/// sim data
void ObjectSpace::output_sim_data(const char * filename, int step, int sub_step, int comp_time_ms, float time, bool first)
{
	if (data_sim)
	{
		// output to file
		if (FileIO::data_output_object_info(filename, first, step, sub_step, comp_time_ms, time, m_objects))
		{
			log_main.print("object info saved to csv file");
		}
		else
		{
			log_main.print("object info could not be saved");
		}
	}
}

//// Simulation

// occupation + halo
occ_nodes ObjectSpace::try_to_occupy(GenObject& obj, CSNode& cnode, float radius)
{
	occ_nodes out;

	polygon poly = obj.get_polygon();
	poly.rotate(orient_to_angle(cnode.get_vec_pos().orientation));
	poly.translate(cnode.get_position());

	std::vector<attachment_point> points = obj.get_occupying_attachment_points();

	for (attachment_point& p : points)
	{
		p.move_occ(cnode._attachment_point_validity[obj.get_object_prefab_id()][p.id], false);
		p.rotate(orient_to_angle(cnode.get_vec_pos().orientation));
		p.translate(cnode.get_position());
	}

	//// OBJECT COLLISIONS ////
	for (const GenObject& other_obj : m_objects)
	{
		if (other_obj.get_object_id() == obj.get_object_id()) continue; // same object

		if (other_obj.get_floor() != obj.get_floor()) // floor test
		{
			if (other_obj.stair_id == -1 && obj.stair_id == -1) continue; // neither stair
			if (other_obj.stair_id == -1 && obj.get_floor() + 1 != other_obj.get_floor()) continue; // this stair, other not stair
			if (obj.stair_id == -1 && other_obj.get_floor() + 1 != obj.get_floor()) continue; // this not stair, other stair
			if (other_obj.stair_id != obj.stair_id) continue; // both stair
		}
		
		float min_dist_sq = obj.get_max_width() + other_obj.get_max_width();
		min_dist_sq = min_dist_sq * min_dist_sq;
		if ((obj._position - other_obj._position).magnitude_squared() > min_dist_sq) continue; // objects are far enough away to not collide
		
		if (collision(poly, points, other_obj))
		{
			out.valid = false;
			return out;
		}
	}


	//// PERSON COLLISIONS & OCCUPATION ////
	std::vector<int> open_nodes;
	std::vector<int> closed_nodes;
	std::vector<int> halo_nodes;
	for (auto& i : *cnode.get_tnode_ids())
	{
		open_nodes.push_back(i);
	}

	bool first_occ_found = false;
	std::vector<int> tested_other_ids;

	while (!open_nodes.empty())
	{
		int tnode_id = open_nodes.back();
		open_nodes.pop_back();
		closed_nodes.push_back(tnode_id);

		node* tnode_pt = get_tnode_pnt(tnode_id);
		if (!tnode_pt) continue;
		if (test_intersect(tnode_pt, poly, radius, points)) // object position would intersect with node
		{
			if (tnode_pt->occupied_by_person) // node already occupied by person
			{
				out.valid = false;
				return out;
			}

			if (!first_occ_found)
			{
				first_occ_found = true;
				open_nodes.clear();
				closed_nodes.clear();
			}
			out.occupied_nodes.push_back(tnode_id);

			for (int i : tnode_pt->tnodes) // add all neigh to open
			{
				if (std::find(closed_nodes.begin(), closed_nodes.end(), i) == closed_nodes.end()) // if i not in closed nodes
				{
					if (std::find(open_nodes.begin(), open_nodes.end(), i) == open_nodes.end()) // if i not already in open nodes
					{
						open_nodes.push_back(i);
					}
				}
			}
		}
		else if (first_occ_found) // object does not intersect with node - halo
		{
			halo_nodes.push_back(tnode_id);
		}
		else // if not found an occupied node yet, add all neigh to open 
		{
			for (int i : tnode_pt->tnodes)
			{
				if (std::find(closed_nodes.begin(), closed_nodes.end(), i) == closed_nodes.end()) // if i not in closed nodes
				{
					if (std::find(open_nodes.begin(), open_nodes.end(), i) == open_nodes.end()) // if i not already in open nodes
					{
						open_nodes.push_back(i);
					}
				}
			}
		}
	}

	if (!first_occ_found)
	{
		out.valid = false;
		return out;
	}
	out.valid = true;

	// check if halo is connected
	open_nodes.clear();
	closed_nodes.clear();

	if (!(halo_nodes.empty()))
	{
		open_nodes.push_back(halo_nodes.back());
	}

	while (!open_nodes.empty())
	{
		node* n = get_tnode_pnt(open_nodes.back());
		closed_nodes.push_back(open_nodes.back());
		open_nodes.pop_back();

		for (int i : n->tnodes)
		{
			// if i not in closed and not in open and in halo then add to open
			if (!in_vector(closed_nodes, i) && !in_vector(open_nodes, i) && in_vector(halo_nodes, i))
			{
				open_nodes.push_back(i);
			}
		}
	}
	out.connected_halo = (closed_nodes.size() == halo_nodes.size());

	return out;
}
bool ObjectSpace::collision(const polygon& this_poly, const std::vector<attachment_point>& this_points, char other_object_id)
{
	GenObject* other_obj_pt = get_object(other_object_id);
	if (!other_obj_pt) return false;

	std::vector<attachment_point> other_points = other_obj_pt->get_occupying_attachment_points();
	for (attachment_point& p : other_points)
	{
		p.rotate(other_obj_pt->_orientation);
		p.translate(other_obj_pt->_position);
	}
	polygon other_poly = other_obj_pt->get_polygon();
	other_poly.rotate(other_obj_pt->_orientation);
	other_poly.translate(other_obj_pt->_position);

	// this attach point intersect with other attach point?
	for (const attachment_point& t_p : this_points)
	{
		for (const attachment_point& o_p : other_points)
		{
			float rad_sum = t_p.occupation_circles.radius + o_p.occupation_circles.radius;
			if (distsq_x_y(o_p.get_occupant_pos(), t_p.get_occupant_pos()) < rad_sum*rad_sum)
			{
				return true;
			}
		}
	}

	// this attach point intersect with other poly?
	for (const attachment_point& p : this_points)
	{
		if (other_poly.does_circle_intersect(p.get_occupant_pos(), p.occupation_circles.radius))
		{
			return true;
		}
	}

	// this poly intersect with other attach point?
	for (const attachment_point& p : other_points)
	{
		if (this_poly.does_circle_intersect(p.get_occupant_pos(), p.occupation_circles.radius))
		{
			return true;
		}
	}

	// this poly intersect with other poly?
	if (this_poly.does_poly_intersect(other_poly))
	{
		return true;
	}

	// no collision
	return false;
}
bool ObjectSpace::collision(const polygon& this_poly, const std::vector<attachment_point>& this_points, const GenObject& other_obj)
{
	std::vector<attachment_point> other_points = other_obj.get_occupying_attachment_points();
	for (attachment_point& p : other_points)
	{
		p.rotate(other_obj._orientation);
		p.translate(other_obj._position);
	}
	polygon other_poly = other_obj.get_polygon();
	other_poly.rotate(other_obj._orientation);
	other_poly.translate(other_obj._position);

	// this attach point intersect with other attach point?
	for (const attachment_point& t_p : this_points)
	{
		for (const attachment_point& o_p : other_points)
		{
			float rad_sum = t_p.occupation_circles.radius + o_p.occupation_circles.radius;
			if (distsq_x_y(o_p.get_occupant_pos(), t_p.get_occupant_pos()) < rad_sum*rad_sum)
			{
				return true;
			}
		}
	}

	// this attach point intersect with other poly?
	for (const attachment_point& p : this_points)
	{
		if (other_poly.does_circle_intersect(p.get_occupant_pos(), p.occupation_circles.radius))
		{
			return true;
		}
	}

	// this poly intersect with other attach point?
	for (const attachment_point& p : other_points)
	{
		if (this_poly.does_circle_intersect(p.get_occupant_pos(), p.occupation_circles.radius))
		{
			return true;
		}
	}

	// this poly intersect with other poly?
	if (this_poly.does_poly_intersect(other_poly))
	{
		return true;
	}

	// no collision
	return false;
}
void ObjectSpace::remove_occupation_obj(int object_id)
{
	for (node& n : m_EX_nodes)
	{
		if (n.occupied_by_person) continue;

		n.occupier_ids.erase(object_id);
	}
}
void ObjectSpace::set_occupation_halo_obj(occ_nodes& o_h, GenObject& obj, float seconds)
{
	remove_occupation_obj(obj.get_object_id());

	obj.clear_occ();
	obj.set_occupation_tnodes(o_h.occupied_nodes);
	obj.set_connected_halo(o_h.connected_halo, seconds);

	// set occupied nodes
	int counter = 0;
	for (int i : o_h.occupied_nodes)
	{
		for (node& n : m_EX_nodes)
		{
			if (n.id == i)
			{
				n.occupied_by_person = false;
				n.occupier_ids.insert(obj.get_object_id());
				++counter;
				if (counter == o_h.occupied_nodes.size())
				{
					return;
				}
				break;
			}
		}
	}
}
void ObjectSpace::set_occupation_halo_obj(int object_id, float radius, float seconds)
{
	GenObject* obj = get_object(object_id);

	// get polygon and attachment points (in world space)
	polygon poly = obj->get_polygon();
	poly.rotate(obj->_orientation);
	poly.translate(obj->_position);

	std::vector<attachment_point> points = obj->get_occupying_attachment_points();

	for (attachment_point& p : points)
	{
		p.rotate(obj->_orientation);
		p.translate(obj->_position);
	}

	obj->clear_occ();

	std::vector<node*> occupied_nodes;
	std::vector<node*> halo_nodes;
	
	std::vector<int> open_nodes;
	std::vector<int> closed_nodes;
	std::vector<int> open_nodes_first;
	std::vector<int> closed_nodes_first;

	for (auto it = get_node(obj->get_cnode_id())->get_tnode_ids()->begin(); it != get_node(obj->get_cnode_id())->get_tnode_ids()->end(); ++it)
	{
		open_nodes_first.push_back(*it);
	}

	// find first occupied node, make this occupied and add all connected nodes to open_nodes
	bool intersect_first = false;
	while (!intersect_first && !open_nodes_first.empty())
	{
		node* n = TCP_get_node_from_id(open_nodes_first.back());
		closed_nodes_first.push_back(open_nodes_first.back());
		open_nodes_first.pop_back();
		if (!n) continue;

		intersect_first = test_intersect(n, poly, radius, points);

		if (intersect_first)
		{
			occupied_nodes.push_back(n);
			closed_nodes.push_back(n->id);
			for (int i : n->tnodes)
			{
				open_nodes.push_back(i);
			}
			break;
		}
		else
		{
			for (int i : n->tnodes)
			{
				if (std::find(closed_nodes_first.begin(), closed_nodes_first.end(), i) == closed_nodes_first.end()) // if i not in closed nodes first
				{
					if (std::find(open_nodes_first.begin(), open_nodes_first.end(), i) == open_nodes_first.end()) // if i not already in open nodes first
					{
						open_nodes_first.push_back(i);
					}
				}
			}
		}
	}

	// find occupied and halo nodes
	while (!open_nodes.empty())
	{
		node* n = TCP_get_node_from_id(open_nodes.back());
		closed_nodes.push_back(open_nodes.back());
		open_nodes.pop_back();
		if (!n) continue;

		bool intersect = test_intersect(n, poly, radius, points);

		if (intersect)
		{
			occupied_nodes.push_back(n);
			for (int i : n->tnodes)
			{
				if (std::find(closed_nodes.begin(), closed_nodes.end(), i) == closed_nodes.end()) // if i not in closed nodes
				{
					if (std::find(open_nodes.begin(), open_nodes.end(), i) == open_nodes.end()) // if i not already in open nodes
					{
						open_nodes.push_back(i);
					}
				}
			}
		}
		else
		{
			halo_nodes.push_back(n);
		}
	}

	// make occupied and halo into int vectors
	std::vector<int> ioccupied_nodes;
	std::vector<int> ihalo_nodes;
	for (node* n : occupied_nodes)
	{
		ioccupied_nodes.push_back(n->id);
	}
	for (node* n : halo_nodes)
	{
		ihalo_nodes.push_back(n->id);
	}

	// check if halo is connected
	open_nodes.clear();
	closed_nodes.clear();

	if (!ihalo_nodes.empty())
		open_nodes.push_back(ihalo_nodes.back());

	while (!open_nodes.empty())
	{
		node* n = TCP_get_node_from_id(open_nodes.back());
		closed_nodes.push_back(open_nodes.back());
		open_nodes.pop_back();

		for (int i : n->tnodes)
		{
			// if i not in closed and not in open and in halo then add to open
			if (!in_vector(closed_nodes, i) && !in_vector(open_nodes, i) && in_vector(ihalo_nodes, i))
			{
				open_nodes.push_back(i);
			}
		}
	}
	
	// set occupied nodes and halo nodes in object
	obj->set_occupation_tnodes(ioccupied_nodes);
	obj->set_connected_halo(closed_nodes.size() == ihalo_nodes.size(), seconds);
}
bool ObjectSpace::test_intersect(node* n, polygon& poly, float radius, std::vector<attachment_point>& points)
{
	if (poly.does_circle_intersect(n->position, radius))
	{
		return true;
	}
	else
	{
		for (attachment_point& p : points)
		{
			if (p.does_circle_intersect(n->position, radius))
			{
				return true;
			}
		}
	}

	return false;
}
void ObjectSpace::add_occ_to_nodes_from_obj(int object_id)
{
	std::vector<int> vec = get_object(object_id)->get_occupation_tnodes();
	int counter = 0;
	for (int i : vec)
	{
		for (node& n : m_EX_nodes)
		{
			if (n.id == i)
			{
				n.occupied_by_person = false;
				n.occupier_ids.insert(object_id);
				++counter;
				if (counter == vec.size())
				{
					return;
				}
				break;
			}
		}
	}
}
void ObjectSpace::update_occupation_halo(int object_id, float seconds)
{
	remove_occupation_obj(object_id);
	set_occupation_halo_obj(object_id, 0.2f, seconds);
	add_occ_to_nodes_from_obj(object_id);
}
void ObjectSpace::move_occupation(int old_id, int new_id, int person_id) // add new set old_id == new_id; remove person set new_id < 0;
{
	bool new_done = new_id < 0;
	bool old_done = old_id == new_id;
	if (new_done && old_done) return;

	for (node& n : m_EX_nodes)
	{
		if (!old_done && n.id == old_id)
		{
			if (n.occupied_by_person && *n.occupier_ids.begin() == person_id)
			{
				n.occupied_by_person = false;
				n.occupier_ids.clear();
			}

			if (new_done) return;
			old_done = true;
		}
		else if (!new_done && n.id == new_id)
		{
			n.occupied_by_person = true;
			n.occupier_ids.clear();
			n.occupier_ids.insert(person_id);

			if (old_done) return;
			new_done = true;
		}
	}

	if (!new_done) log_main.print("new node not occupied: node %d not found", new_id);
	if (!old_done) log_main.print("old node still occupied: node %d not found", old_id);
}
void ObjectSpace::init_all_occupations()
{
	for (GenObject& obj : m_objects)
	{
		set_occupation_halo_obj(obj.get_object_id(), 0.2f, 0.0f);
		add_occ_to_nodes_from_obj(obj.get_object_id());
	}

	for (auto it = m_people.begin(); it != m_people.end(); ++it)
	{
		if (it->second.active && !it->second.in_object)
		{
			if (it->second.node_id == -1)
			{
				if (find_occupation_person(it->first))
				{
					move_occupation(it->second.node_id, it->second.node_id, it->first);
				}
			}
			else
			{
				move_occupation(it->second.node_id, it->second.node_id, it->first);
			}
		}
	}
}
bool ObjectSpace::find_occupation_person(int person_id)
{
	person* p;
	try
	{
		p = &m_people.at(person_id);
	}
	catch (const std::exception& e)
	{
		log_main.print("cannot occupy node: person id does not exist");
		return false;
	}

	if (p->in_object) return false;

	node* n = get_nearest_tnode(p->position.x, p->position.y, p->floor_id);
	if (n)
	{
		p->node_id = n->id;
		p->position = n->position;
	}
	else
	{
		log_main.print("cannot occupy node: node does not exist");
		return false;
	}

	return true;
}
node* ObjectSpace::get_nearest_tnode(float x, float y, int floor)
{
	if (m_EX_nodes.empty()) return 0;

	vector2 xy(x, y);

	float distsq_close = 0.0f;
	node* node_close = 0;

	for (node& n : m_EX_nodes)
	{
		if (n.floor_num == floor)
		{
			float distsq_cur = xy.distsq(n.position);
			if (!node_close || distsq_cur < distsq_close)
			{
				distsq_close = distsq_cur;
				node_close = &n;
			}
		}
	}

	return node_close;
}

// vos
x_y<float> ObjectSpace::get_tnode_xypos(int id)
{
	node* n = get_tnode_pnt(id);
	if (!n)
		return { 0.0f,0.0f };

	return n->position;
}
bool ObjectSpace::intersection(start_end<x_y<float>> l1, start_end<x_y<float>> l2)
{
	int o1 = intersect_test2_orient(l1.start, l1.end, l2.start);
	int o2 = intersect_test2_orient(l1.start, l1.end, l2.end);
	int o3 = intersect_test2_orient(l2.start, l2.end, l1.start);
	int o4 = intersect_test2_orient(l2.start, l2.end, l1.end);

	return o1 != o2 && o3 != o4;
}
bool ObjectSpace::intersection(start_end<x_y<float>> l1, start_end<x_y<float>> l2, x_y<float>& intersect)
{
	if (!intersection(l1, l2)) return false;

	// arc1 represented as a1x + b1y = c1 
	float a1 = l1.end.y - l1.start.y;
	float b1 = l1.start.x - l1.end.x;
	float c1 = a1 * (l1.start.x) + b1 * (l1.start.y);

	// arc2 represented as a2x + b2y = c2 
	float a2 = l2.end.y - l2.start.y;
	float b2 = l2.start.x - l2.end.x;
	float c2 = a2 * (l2.start.x) + b2 * (l2.start.y);

	float determinant = a1 * b2 - a2 * b1;

	if (determinant == 0.0f) return false; // just in case this will stop it breaking, will only happen very very rarely due to rounding error

	intersect = { (b2*c1 - b1 * c2) / determinant, (a1*c2 - a2 * c1) / determinant };

	return true;
}
struct pos_floor_stair
{
	x_y<float> pos;
	int floor;
	int stair;

	pos_floor_stair(node* n) : pos(n->position), floor(n->floor_num), stair(n->stair_id) {}
	pos_floor_stair() {}
};
bool ObjectSpace::check_visable(node* n1, node* n2)
{
	if (n1->stair_id == n2->stair_id && n1->stair_id != -1) return true; // both on same stair then true

	std::map<int, start_end<x_y<float>>> floor_line;

	// same floor and neither are stair (level)
	if (n2->floor_num == n1->floor_num && n1->stair_id == n2->stair_id)
	{
		floor_line[n1->floor_num] = { n1->position,n2->position };
	}
	else
	{
		// find which node is lower and which is higher
		pos_floor_stair lower;
		pos_floor_stair higher;
		if (n1->floor_num < n2->floor_num)
		{
			lower = n1;
			higher = n2;
		}
		else if (n2->floor_num < n1->floor_num)
		{
			lower = n2;
			higher = n1;
		}
		else if (n1->stair_id == -1) // same floor (n2 is stair)
		{
			lower = n1;
			higher = n2;
		}
		else
		{
			lower = n2;
			higher = n1;
		}
		start_end<x_y<float>> full_line = { lower.pos,higher.pos };

		// move lower and higher to top and bottom of stairs (respectively) if on stairs
		if (lower.stair != -1) // if lower stair, then move to top of stair
		{
			stair* s = get_stairpt_from_id(lower.stair);
			if (!s) return false;

			x_y<float> intersect;
			if (intersection(full_line, s->get_top_line(true), intersect))
			{
				lower.pos = intersect;
				lower.floor += 1;
				lower.stair = -1;
			}
			else
			{
				return false;
			}
		}

		if (higher.stair != -1) // if higher stair, then move to bottom of stair
		{
			stair* s = get_stairpt_from_id(higher.stair);
			if (!s) return false;

			x_y<float> intersect;
			if (intersection(full_line, s->get_bottom_line(true), intersect))
			{
				higher.pos = intersect;
				higher.stair = -1;
			}
			else
			{
				return false;
			}
		}

		while (lower.floor < higher.floor) // move lower to top of next stair if full_line goes across a stair
		{
			bool no_stair = true;
			for (stair& s : m_EX_stairs)
			{
				if (s.floor_num == lower.floor)
				{
					x_y<float> bot_pos;
					if (intersection(full_line, s.get_bottom_line(true), bot_pos))
					{
						x_y<float> top_pos;
						if (intersection(full_line, s.get_top_line(true), top_pos))
						{
							floor_line[lower.floor] = { lower.pos,bot_pos };
							lower.pos = top_pos;
							lower.floor += 1;
							no_stair = false;
						}
					}
				}
			}
			if (no_stair)
			{
				return false;
			}
		}

		floor_line[lower.floor] = { lower.pos, higher.pos };
	}

	for (auto& f_l : floor_line)
	{
		for (boundary_line& b : m_bounds)
		{
			if (b.floor_num == f_l.first)
			{
				if (intersection(f_l.second, b.position)) return false; // bound blocks view from n1 to n2
			}
		}
	}
	
	return true;
}
bool ObjectSpace::check_visable(std::vector<int> n1s, int n2)
{
	node* n2_pt = get_tnode_pnt(n2);
	if (!n2_pt) return false;
	for (int i : n1s)
	{
		node* n1 = get_tnode_pnt(i);
		if (!n1) continue;
		if (check_visable(n1, n2_pt)) return true;
	}
	return false;
}
bool ObjectSpace::check_visable(std::vector<int> n1s, std::vector<int> n2s)
{
	for (int i1 : n1s)
	{
		node* n1 = get_tnode_pnt(i1);
		if (!n1) continue;
		for (int i2 : n2s)
		{
			node* n2 = get_tnode_pnt(i2);
			if (!n2) continue;
			if (check_visable(n1, n2)) return true;
		}
	}
	return false;
}
velocity_obstacle ObjectSpace::generate_pvo(GenObject* obj, person* per, bool generalised, bool hybrid, float time_to_collision, float radius, float VO_dist_cost)
{
	obj->remove_vo(per->id, false);

	std::vector<x_y<float>> other_occ = { get_tnode_xypos(per->node_id) };
	
	x_y<float> obj_vel = obj->velocity_desired * vo_des_vel_f + obj->velocity_current.first * (1.0f - vo_des_vel_f);

	velocity_obstacle pvo;
	velocity_obstacle ovo(
		obj->get_object_id(), per->id, false,
		obj->get_verticies_relative(), {},
		obj->get_attachment_point_pos(), { per->position },
		radius,
		(x_y<float>)obj->_position, (x_y<float>)per->position,
		obj_vel, (x_y<float>)per->velocity,
		obj->moved_backwards, false,
		obj->get_max_acceleration(),1.0f,
		obj->get_max_linear_speed(NOT_STAIR_ARC,-1),per->default_speed,
		VO_dist_cost,
		vo_l_r_priority, min_time_scale_stopped_ped,
		obj->stopped, per->stopped,
		!obj->halo_is_connected(), false,
		obj->time_blocking_cor, 0.0f,
		obj->get_drive(), per->drive,
		pvo,
		time_to_collision,
		generalised, hybrid
	);

	if (ovo.valid)
		obj->add_vo(ovo);

	return pvo;
}
void ObjectSpace::generate_ovo(GenObject* this_obj, GenObject* other_obj, bool generalised, bool hybrid, float time_to_collision, float radius, float VO_dist_cost)
{
	this_obj->remove_vo(other_obj->get_object_id(), true);
	other_obj->remove_vo(this_obj->get_object_id(), true);
	
	std::vector<x_y<float>> other_occ;
	for (int id : other_obj->get_occupation_tnodes())
	{
		other_occ.push_back({ get_tnode_xypos(id) });
	}

	x_y<float> this_obj_vel = this_obj->velocity_desired * vo_des_vel_f + this_obj->velocity_current.first * (1.0f - vo_des_vel_f);
	x_y<float> other_obj_vel = other_obj->velocity_desired * vo_des_vel_f + other_obj->velocity_current.first * (1.0f - vo_des_vel_f);

	// TODO: change velocity_current.first to velocity_desired
	velocity_obstacle other_vo;
	velocity_obstacle this_vo(
		this_obj->get_object_id(), other_obj->get_object_id(), true,
		this_obj->get_verticies_relative(), other_obj->get_verticies_relative(),
		this_obj->get_attachment_point_pos(), other_obj->get_attachment_point_pos(),
		radius,
		this_obj->_position, other_obj->_position,
		this_obj_vel, other_obj_vel,
		this_obj->moved_backwards, other_obj->moved_backwards,
		this_obj->get_max_acceleration(), other_obj->get_max_acceleration(),
		this_obj->get_max_linear_speed(NOT_STAIR_ARC, -1), other_obj->get_max_linear_speed(NOT_STAIR_ARC, -1),
		VO_dist_cost,
		vo_l_r_priority, min_time_scale_stopped_ped,
		this_obj->stopped, other_obj->stopped,
		!this_obj->halo_is_connected() || this_obj->get_blocking_width() < other_obj->get_width(), !other_obj->halo_is_connected() || other_obj->get_blocking_width() < this_obj->get_width(),
		this_obj->time_blocking_cor, other_obj->time_blocking_cor,
		this_obj->get_drive(), other_obj->get_drive(),
		other_vo,
		time_to_collision,
		generalised, hybrid
	);

	if (this_vo.valid)
		this_obj->add_vo(this_vo);
	if (other_vo.valid)
		other_obj->add_vo(other_vo);
}
void ObjectSpace::update_all_vos(bool first, bool generalised, bool hybrid, float time_to_collision, float person_radius, float VO_dist_cost)
{
	if (first)
	{
		for (auto& p : m_people)
		{
			p.second.moved = p.second.in_object ? false : p.second.active;
		}
	}

	//// clear vos for moved objects or objects that have just stopped
	std::vector<int> moved_objs;

	// clear vos from moved objects
	for (GenObject& o : m_objects)
	{
		if (first)
			o.moved = true;

		if (o.moved || o.just_stopped)
		{
			o.clear_vos();
			moved_objs.push_back(o.get_object_id());
		}
	}

	if (!first)
	{
		// remove moved objects from people and non-moved objects
		for (int& id : moved_objs)
		{
			for (GenObject& o : m_objects)
			{
				if (!(o.moved || o.just_stopped))
				{
					o.remove_vo(id, true);
				}
			}

			for (auto& per : m_people)
			{
				per.second.remove_vo(id);
			}
		}

		// clear vos from moved people and remove moved people from all objects
		for (auto& per : m_people)
		{
			if (per.second.moved)
			{
				per.second.pvos.clear();

				for (GenObject& o : m_objects)
				{
					if (!(o.moved || o.just_stopped))
					{
						o.remove_vo(per.first, false);
					}
				}

			}
		}
	}

	//// Calculate VOs and PVOs
	for (auto this_obj = m_objects.begin(); this_obj != m_objects.end(); ++this_obj) // go through all objects
	{

		for (auto other_obj = this_obj + 1; other_obj != m_objects.end(); ++other_obj) // go through all other objects
		{
			if (this_obj->moved || other_obj->moved)
			{
				if (check_visable(other_obj->get_occupation_tnodes(), this_obj->get_occupation_tnodes()))
				{
					generate_ovo(&*this_obj, &*other_obj, generalised, hybrid, time_to_collision, person_radius, VO_dist_cost);
				}
			}
		}

		for (auto per = m_people.begin(); per != m_people.end(); ++per) // go through all people
		{
			if (per->second.active && !per->second.in_object && (this_obj->moved || per->second.moved))
			{
				if (check_visable(this_obj->get_occupation_tnodes(), per->second.node_id))
				{
					per->second.add_vo(generate_pvo(&*this_obj, &per->second, generalised, hybrid, time_to_collision, person_radius, VO_dist_cost));
				}
			}
		}

		this_obj->moved = false;
	}

	for (auto per = m_people.begin(); per != m_people.end(); ++per)
	{
		per->second.moved = false;
	}
}

// heuristics
void ObjectSpace::reset_heuristic(int object_id, cnode_pos start)
{
	m_heuristic_edges[object_id].clear_edges();
	m_heuristic_edges[object_id].add_new_node(start.vec_pos, 0.0f);
	m_cspace.LD_reset_heuristics(object_id);
	m_cspace.LD_set_heuristic(start.vec_pos, object_id, -2.0f);
}
std::pair<float, bool> ObjectSpace::get_heuristic(int object_id, int prefab_id, speeds max_speeds, cnode_pos end, float h_mult)
{
	if (h_mult <= 0.0f)
	{
		return { 0.0f, false };
	}

	GenObject* obj = get_object(object_id);
	if (!obj)
	{
		return { 0.0f, false };
	}

	if (m_cspace.LD_get_heuristic(end, object_id) >= 0.0f) // if end node has a heuristic, then return it
	{
		return { m_cspace.LD_get_heuristic(end, object_id), true };
	} // else, calculate heuristics using m_heuristic_edges until end node has a heuristic

	float end_h = 0.0f;

	int num_heu = 0;

	bool stop = false;
	while (!m_heuristic_edges[object_id].empty() && !stop)
	{
		std::pair<int, float> new_node = m_heuristic_edges[object_id].pop_first();

		if (m_cspace.LD_get_heuristic(new_node.first, object_id) == -2.0f)
		{
			// set potential of node_id to new_pot
			m_cspace.LD_set_heuristic(new_node.first, object_id, new_node.second);
			++num_heu;

			// if just set end node, then exit
			if (new_node.first == end.vec_pos)
			{
				stop = true;
				end_h = new_node.second;
			}

			int stair_id = m_cspace.LD_get_stair_id(new_node.first);

			// add all nodes connected to new_node_id to stack with pot = new_pot + distance
			for (std::vector<CSArc>::iterator it = m_cspace.LD_get_arcs(new_node.first)->begin(); it != m_cspace.LD_get_arcs(new_node.first)->end(); ++it)
			{
				if (m_cspace.LD_get_heuristic(it->cnode_to_vec_pos, object_id) < 0.0f) // not fixed
				{
					if (m_cspace.LD_node_is_valid(it->cnode_to_vec_pos.vec_pos, prefab_id)) // if valid for some orientation
					{
						if (obj->can_take_stair(it->stair_dir, stair_id)) // if can take stair dir
						{
							m_heuristic_edges[object_id].add_node(it->cnode_to_vec_pos.vec_pos,  new_node.second + h_mult*(it->length /max_speeds.get_linear(it->stair_dir, stair_id)));
							m_cspace.LD_set_heuristic(it->cnode_to_vec_pos, object_id, -2.0f);
						}
					}
				}
			}
		}
	}
	if (num_heu > 0)
		log_main.print("Heuristics set = %d\n", num_heu);
	return { end_h, end_h >= 0.0f };
}

// potential corridor
float ObjectSpace::calc_weight(bool trans, bool rot, float xy_displacement, float max_acel, float max_lin_speed, float max_ang_speed)
{
	if (trans)
	{
		float t_t = xy_displacement / max_lin_speed;
		
		if (rot)
		{
			// trans and rot
			float t_a = fLAYER_GAP / max_ang_speed;
			return sqrt(t_t*t_t + t_a * t_a);
		} 
		// trans and !rot
		return t_t;
	}
	if (rot)
	{
		// !trans and rot
		return fLAYER_GAP / max_ang_speed;
	}

	// !trans and !rot
	return 0.0f;
}
bool ObjectSpace::generate_potentials(int object_id, std::vector<cnode_pos> empty_neigh_nodes, std::vector<cnode_pos> new_target_nodes, float h_mult)
{
	if (empty_neigh_nodes.empty())
	{
		log_main.print("Return 1\n");
		return false;
	}
	GenObject* obj_pt = get_object(object_id);
	if (!obj_pt) return false;

	int prefab_id = obj_pt->get_object_prefab_id();

	speeds max_speeds_for = obj_pt->get_max_speeds();
	speeds max_speeds_back = max_speeds_for;
	max_speeds_back.invert_stair_dirs();
	valid_stair_dirs valid_stairs_for = obj_pt->get_stair_valids();
	valid_stair_dirs valid_stairs_back = valid_stairs_for;
	valid_stairs_back.invert_stair_dirs();
	cnode_pos current_node = obj_pt->get_cnode_id();

	// remove duplicates in empty_neigh_nodes
	sort(empty_neigh_nodes.begin(), empty_neigh_nodes.end());
	empty_neigh_nodes.erase(unique(empty_neigh_nodes.begin(), empty_neigh_nodes.end()), empty_neigh_nodes.end());

	// update targets
	if (new_target_nodes.empty() || new_target_nodes == obj_pt->get_current_targets()) // keep old targets
	{
		if (m_corridor_edges[object_id].empty() || obj_pt->get_current_targets().empty())
		{
			log_main.print("Return 2");
			return true;
		}

		// update heuristics in edge for the current location
		reset_edge_heuristics(object_id, prefab_id, max_speeds_for, current_node, h_mult);
	}
	else // new targets -> update targets and reset pots
	{
		sort(new_target_nodes.begin(), new_target_nodes.end());
		new_target_nodes.erase(unique(new_target_nodes.begin(), new_target_nodes.end()), new_target_nodes.end());

		obj_pt->update_target_nodes(new_target_nodes);
		m_corridor_edges[object_id].clear_edges();
		reset_potentials(object_id);
		reset_heuristic(object_id, current_node);

		// get heu for target node *it; add this node to corridor_edges (pot = 0)
		for (std::vector<cnode_pos>::iterator it = new_target_nodes.begin(); it != new_target_nodes.end(); ++it)
		{
			if (is_valid(*it, prefab_id))
			{
				std::pair<float, bool> heu = get_heuristic(object_id, prefab_id, max_speeds_for, *it, h_mult);
				if (!heu.second) return false; // if no heuristics were calculated, then target is unreachable
				add_new_cnode_to_edges(*it, 0.0f, heu.first, object_id);
			}
		}
	}

	int num_nodes_set = 0;
	int count = 0;
	bool stop = false;
	while (!stop && !m_corridor_edges[object_id].empty())
	{
		// pop lowest cnode in edges (A* heuristic)
		std::pair<cnode_pos, float> node_pot = m_corridor_edges[object_id].pop_first();
		CSNode* old_node = get_node(node_pot.first);

		if (old_node->_potential_fixed[object_id]) // pot is already fixed so discard and find another old_node
		{
			bool stop = false;
			while (!stop)
			{
				if (m_corridor_edges[object_id].empty()) return true;

				node_pot = m_corridor_edges[object_id].pop_first();
				old_node = get_node(node_pot.first);

				if (!old_node->_potential_fixed[object_id])
				{
					stop = true;
				}
			}
		}
		
		old_node->_potential_fixed[object_id] = true;
		old_node->_potentials[object_id] = node_pot.second;

		// remove this from empty_neigh_nodes if in
		for (std::vector<cnode_pos>::iterator it = empty_neigh_nodes.begin(); it != empty_neigh_nodes.end(); ++it)
		{
			if (*it == node_pot.first)
			{
				empty_neigh_nodes.erase(it);
				break;
			}
		}

		// if empty_neigh_nodes is empty, expand corridor one more time then terminate
		if (empty_neigh_nodes.empty()) stop = true;

		// add connected nodes to stack with updated heu and pot
		for (std::vector<CSArc>::iterator it = old_node->_arcs.begin(); it != old_node->_arcs.end(); ++it)
		{
			CSNode* new_node = get_node(it->cnode_to_vec_pos);
			float landing_curve_cost = 0.0f;
			if (new_node->_valid[prefab_id] && !new_node->_potential_fixed[object_id] && landing_valid_pot_gen(old_node,new_node,it->stair_dir,prefab_id, landing_curve_cost))
			{
				if (obj_pt->can_turn_on_spot() || !it->turn_on_spot)
				{
					// get holo of inverse arc
					float holo = obj_pt->get_holonomicity(it->inv_holo);

					if (holo > 0.0f && valid_stairs_back.valid(it->stair_dir, new_node->_stair_ids[prefab_id])) // object can traverse this arc
					{
						float current_pot = new_node->_potentials[object_id];

						// get heuristic
						std::pair<float, bool> heu = get_heuristic(object_id, prefab_id, max_speeds_for, it->cnode_to_vec_pos, h_mult);
						if (!heu.second) return false; // if no heuristics were calculated, then target is unreachable

						// calc pot dep on turn_on_spot and same_layer
						float weight = calc_weight(!it->turn_on_spot,!it->same_layer, it->length, obj_pt->get_max_acceleration(), max_speeds_back.get_linear(it->stair_dir, new_node->_stair_ids[prefab_id])*holo, max_speeds_back.get_angular());

						float pot = node_pot.second + weight + landing_curve_cost;

						if (current_pot < 0.0f || pot < current_pot) // not been set or has higher pot
						{
							add_new_cnode_to_edges(it->cnode_to_vec_pos, pot, heu.first, object_id);
						}
					}
				}
			}
		}
	}
	return true;
}
void ObjectSpace::reset_potentials(int object_id)
{
	log_main.print("reset_potentials");
	for (int i = 0; i < iNUM_ORIENT; ++i)
	{
		for (std::vector<CSNode>::iterator it = m_cspace._cspace[i].begin(); it != m_cspace._cspace[i].end(); ++it)
		{
			it->_potential_fixed[object_id] = false;
			it->_potentials[object_id] = -1.0f;
		}
	}
}
void ObjectSpace::reset_all_potentials()
{
	for (int i = 0; i < iNUM_ORIENT; ++i)
	{
		for (std::vector<CSNode>::iterator it = m_cspace._cspace[i].begin(); it != m_cspace._cspace[i].end(); ++it)
		{
			for (auto& pf : it->_potential_fixed)
			{
				pf.second = false;
			}
			for (auto& p : it->_potentials)
			{
				p.second = -1.0f;
			}
		}
	}

	for (auto& ce : m_corridor_edges)
	{
		ce.second.clear_edges();
	}

	for (auto& he : m_heuristic_edges)
	{
		he.second.clear_edges();
	}
}
void ObjectSpace::add_new_cnode_to_edges(cnode_pos node, float potential, float heuristic, int object_id)
{
	// add to stack
	set_potential(node, potential, object_id);
	m_corridor_edges[object_id].add_new_cnode(node, potential, heuristic);
}
void ObjectSpace::set_potential(cnode_pos node, float potential, int object_id)
{
	get_node(node)->_potentials[object_id] = potential;
}
void ObjectSpace::reset_edge_heuristics(int object_id, int prefab_id, speeds max_speeds, cnode_pos start, float h_mult)
{
	reset_heuristic(object_id, start);

	std::vector<cnode_pos> cnode_order = m_corridor_edges[object_id].get_ordered_cnode_pos();
	
	std::vector<float> ordered_heuristics;
	for (std::vector<cnode_pos>::iterator it = cnode_order.begin(); it != cnode_order.end(); ++it)
	{
		ordered_heuristics.push_back(get_heuristic(object_id, prefab_id, max_speeds, *it, h_mult).first);
	}

	m_corridor_edges[object_id].update_heuristics(ordered_heuristics);
}
bool ObjectSpace::landing_valid_pot_gen(CSNode* from, CSNode* to, unsigned char stair_dir, int prefab_id, float& additional_cost)
{
	additional_cost = 0.0f;

	bool from_land = from->landing_id != -1;
	bool to_land = to->landing_id != -1;
	if (!from_land && !to_land) return true; // neither are landing
	
	bool to_stair = to->_stair_id != -1;
	if (!to_land && !to_stair) return true; // to is not landing or stair
	bool from_stair = from->_stair_id != -1;
	if (!from_land && !from_stair) return true; // from is not landing or stair

	float from_dist = from_land ? from->_landing_dist_curve[prefab_id] : 0.0f;
	float to_dist = to_land ? to->_landing_dist_curve[prefab_id] : 0.0f;

	if (from_stair || to_stair) // at least one is stair
	{
		if (from_dist > 0.0f || to_dist > 0.0f)
			return false;

		return stair_dir == STAIR_ARC_UP || stair_dir == STAIR_ARC_DOWN || stair_dir == NOT_STAIR_ARC;
	}

	// both are landing and not stair
	float diff_dist = from_dist - to_dist;
	if (diff_dist > 0.0f) //if potential spread is towards curve, add cost
	{
		additional_cost = diff_dist;
	}

	return true;
}

// tasks
void ObjectSpace::go_to_target(int object_id, int priority, int tnode_id)
{
	get_object(object_id)->itinerary.insert(task(priority, m_tnode_to_cnode[tnode_id]));
}
void ObjectSpace::go_to_target(int object_id, int tnode_id)
{
	GenObject* obj = get_object(object_id);
	if (!obj)
	{
		log_main.print("Go to target failed: object id %d not found", object_id);
		return;
	}
	obj->itinerary.insert(task(obj->itinerary.size(), m_tnode_to_cnode[tnode_id]));
}
void ObjectSpace::wait_for_seconds(int object_id, int priority, float delay)
{
	get_object(object_id)->itinerary.insert(task(priority, delay));
}
void ObjectSpace::wait_for_seconds(int object_id,float delay)
{
	get_object(object_id)->itinerary.insert(task(get_object(object_id)->itinerary.size(), delay));
}
void ObjectSpace::remove_object_task(int object_id)
{
	GenObject* obj = get_object(object_id);
	if (!obj)
	{
		log_main.print("Object remove failed: object id %d not found", object_id);
		return;
	}
	obj->itinerary.insert(task(obj->itinerary.size() + 99));
}
void ObjectSpace::pick_up_person(int object_id, int person_id)
{
	GenObject* obj = get_object(object_id);
	if (!obj)
	{
		log_main.print("Pick up person failed: object id %d not found", object_id);
		return;
	}
	obj->itinerary.insert(task(obj->itinerary.size(), person_id));
}

// action
std::pair<bool, bool> ObjectSpace::action_task(int object_id, float seconds)
{
	bool interpolate = false;

	log_main.print("action_task");
	GenObject* obj = get_object(object_id);
	if (!obj || !obj->active) return { false,false };

	obj->wait -= seconds;

	bool task_changed = manage_tasks(object_id);

	task* t = &obj->current_task;

	switch (t->type)
	{
	case GO_TO:
		move_obj(object_id, 1.0f, seconds, interpolate);
		break;
	case WAIT:
		if (t->started)
		{
			log_main.print("obj->wait = %f", obj->wait);
			break;
		}
		else
		{
			t->started = true;
			delay_obj(object_id);
		}
		log_main.print("obj->wait = %f", obj->wait);
		break;
	case EMPTY:
		idle_obj(object_id);
		break;
	case REMOVE:
		TCP_remove_object(object_id);
		return { true,true };
	case PICKUP:
		TCP_pickup_person(object_id, t->person_id, 2.0f);
		break;
	default:
		break;
	}

	return { task_changed,false };
}
bool ObjectSpace::manage_tasks(int object_id)
{
	log_main.print("manage_tasks\n");
	GenObject* obj = get_object(object_id);
	if (!obj) return false;

	task* t = &obj->current_task;

	// check if current_task is completed if so, replace with next
	bool replace = false;
	bool task_changed = false;
	switch (t->type)
	{
	case GO_TO:
		if (t->STOP)
		{
			if (obj->wait <= 0.0f)
			{
				replace = true;
				task_changed = true;
			}
		}
		else
		{
			for (std::vector<cnode_pos>::iterator it = t->cnodes.begin(); it != t->cnodes.end(); ++it)
			{
				if (obj->get_cnode_id() == *it)
				{
					t->STOP = true;
					replace = true;
					task_changed = true;
					break;
				}
			}
		}
		break;
	case WAIT:
		if (t->STOP)
		{
			replace = true;
		}
		else
		{
			if (obj->wait <= 0.0f)
			{
				replace = true;
			}
		}
		break;
	case EMPTY:
		if (!obj->itinerary.empty() && obj->can_move())
			replace = true;
		break;
	case PICKUP:
		if (obj->contains_person(t->person_id))
		{
			replace = true;
		}
		break;
	default:
		break;
	}
	
	if (replace)
	{
		log_main.print("current task replaced");
		if (obj->itinerary.empty())
		{
			t->make_empty();
		}
		else
		{
			t->replace(obj->itinerary.begin());
			obj->itinerary.erase(obj->itinerary.begin());
		}
	}

	return task_changed;
}
std::pair<potential_move, occ_nodes> ObjectSpace::get_best_move_occ(std::vector<potential_move>& moves, GenObject* obj)
{
	std::pair<potential_move, occ_nodes> out;
	if (moves.empty() || !obj)
		return out;

	float min_drive_scale = 0.5f; // the most a velocity can be scaled down before it is forced to be in a VO

	bool all_stopped = true;
	for (const velocity_obstacle& vo : obj->get_vos())
	{
		if (!vo.other_not_moving)
		{
			all_stopped = false;
			break;
		}
	}

	// set velocity and vo cost for all moves, sort them based on potential_move::operator< then move to smallest that doesn't cause a collision
	float not_move_cost = 0.0f;
	for (potential_move& move : moves)
	{
		bool oscillate = false;
		auto wait_vel = obj->calc_waits_and_vel(move.old_position, move.new_position, move.holo, move.rot, move.trans, move.stair_dir, oscillate, move.node);
		move.wait = wait_vel.first.first;
		move.move_time = wait_vel.first.second;
		move.velocity = wait_vel.second;
		move.set_VO_cost(obj->get_vos(), min_drive_scale);
		if (move.not_move)
		{
			if (all_stopped)
				not_move_cost = move.wait;
			move.wait = 0.0f;
		}
		else if (oscillate)
		{
			move.add_cost += move.wait;
		}
	}
	if (not_move_cost > 0.0f)
	{
		for (potential_move& move : moves)
		{
			if (!move.trans && !move.towards_goal)
			{
				move.add_cost += not_move_cost;
			}
		}
	}

	// sort moves
	std::sort(moves.begin(), moves.end());

	// move "not move" to first position where comparison_not_move is less
	potential_move* moves_back = &moves.back();
	bool done = false;
	if (moves_back->not_move)
	{
		for (int i = 0; i < moves.size(); ++i) // find first non trans move with pot >= no move (or last element of moves if non found)
		{
			if ((!moves[i].trans && moves[i].potential >= moves_back->potential) || i == moves.size() - 1)
			{
				int insert_pos = 0;
				if (i > 0)
				{
					--i;
					for (; i > -1; --i) // work from this point forward and place no move when comparison returns true
					{
						if (moves_back->comparison() >= moves[i].comparison_not_move())
						{
							insert_pos = i + 1;
							break;
						}
					}
				}

				moves.insert(moves.begin() + insert_pos, *moves_back);
				moves.pop_back();
				break;
			}
		}
	}

	// find best move that doesnt cause a collision
	for (potential_move& move : moves)
	{
		if (!move.valid) break;

		occ_nodes o_n = try_to_occupy(*obj, *get_node(move.node), 0.2f);
		if (o_n.valid)
		{
			out.first = move;
			out.second = o_n;
			return out;
		}
	}

	return out;
}
void ObjectSpace::move_obj(int object_id, float h_mult, float seconds, bool interpolate)
{
	log_main.print("move_obj\n");
	GenObject* obj = get_object(object_id);
	if (!obj)
	{
		log_main.print("!obj\n");
		return;
	}
	task* t = &obj->current_task;

	obj->stopped = false;
	obj->just_stopped = false;
	int prefab_id = obj->get_object_prefab_id();
	
	// if ready to move
	log_main.print("obj->wait = %f\n", obj->wait);
	if (obj->wait <= 0.0f)
	{
		CSNode* current_cnode = get_node(obj->get_cnode_id());

		vector2 cur_pos(current_cnode->get_position().x, current_cnode->get_position().y);
		obj->set_position_orient(cur_pos, orient_to_angle(obj->get_cnode_id().orientation));

		if (!t->started)
		{
			reset_potentials(object_id);
		}

		// get neigh with no pot
		std::vector<cnode_pos> neigh_no_pot;

		for (std::vector<CSArc>::iterator it = current_cnode->_arcs.begin(); it != current_cnode->_arcs.end(); ++it)
		{
			if (is_valid(it->cnode_to_vec_pos, prefab_id))
			{
				if (obj->can_turn_on_spot() || !it->turn_on_spot)
				{
					if (obj->get_holonomicity(it->holo) > 0.0f && obj->can_take_stair(it->stair_dir, current_cnode->_stair_ids[prefab_id]))
					{
						if (!get_node(it->cnode_to_vec_pos)->_potential_fixed[object_id])
						{
							neigh_no_pot.push_back(it->cnode_to_vec_pos);
						}
					}
				}
			}
		}

		log_main.print("neigh_no_pot has %d elements\n", (int)neigh_no_pot.size());

		if (!t->started) // task not started -> generate potentials (with non empty targets vector)
		{
			log_main.print("!t->started\n");
			bool reachable = generate_potentials(object_id, neigh_no_pot, t->cnodes, h_mult);
			t->started = true;

			// if goal is unreachable then stop task
			if (current_cnode->_potentials[object_id] < 0.0f || !reachable)
			{
				t->STOP = true;
				log_main.print("task stopped: goal unreachable");
			}
		}
		else if (!neigh_no_pot.empty()) // some neigh have no pot -> generate potentials
		{
			log_main.print("!neigh_no_pot.empty()\n");
			bool reachable = generate_potentials(object_id, neigh_no_pot, std::vector<cnode_pos>(), h_mult);

			// if goal is unreachable then stop task
			if (current_cnode->_potentials[object_id] < 0.0f || !reachable)
			{
				t->STOP = true;
				log_main.print("task stopped: goal unreachable");
			}
		}

		float old_pot = current_cnode->get_potential(object_id);
		
		// store all moves
		std::vector<potential_move> moves;

		// push back current node (don't move)
		moves.push_back(potential_move(obj->get_cnode_id(), current_cnode->get_position(), 0.0f, current_cnode->get_position(), false, false, true, current_cnode->get_potential(object_id), old_pot, 1.0f, NOT_STAIR_ARC, obj->get_max_linear_speed(NOT_STAIR_ARC, current_cnode->_stair_ids[prefab_id]), obj->get_max_angular_speed(), obj->get_max_acceleration(), seconds));

		// find and push back all moves
		for (CSArc& this_arc : current_cnode->_arcs)
		{
			// if can take arc:
			if (obj->can_turn_on_spot() || !this_arc.turn_on_spot)
			{
				float holo = obj->get_holonomicity(this_arc.holo);
				if (holo > 0.0f && obj->can_take_stair(this_arc.stair_dir, current_cnode->_stair_ids[prefab_id]))
				{
					// if node is valid:
					cnode_pos new_pos = this_arc.cnode_to_vec_pos;
					CSNode* new_node = get_node(new_pos);
					if (new_node && new_node->is_valid(prefab_id))
					{
						float pot = new_node->get_potential(object_id);
						if (pot >= 0.0f)
						{
							potential_move move(new_pos, new_node->get_position(), this_arc.length, current_cnode->get_position(), !this_arc.turn_on_spot, !this_arc.same_layer, false, new_node->get_potential(object_id), old_pot, holo, this_arc.stair_dir, obj->get_max_linear_speed(this_arc.stair_dir,current_cnode->_stair_ids[prefab_id]),obj->get_max_angular_speed(), obj->get_max_acceleration(), seconds);

							if (move.valid)
							{
								moves.push_back(move);
							}
						}
					}
				}
			}
		}
		REMOVE_DUPLICATES(moves);

		auto move_occ = get_best_move_occ(moves, obj);
		potential_move& move = move_occ.first;
		occ_nodes& occ = move_occ.second;


		if (!move.valid || !occ.valid || move.not_move) // no move is possible or not moving is preferred over moving
		{
			obj->dont_move(seconds);
		}
		else // object can move
		{
			CSNode* new_node = get_node(move.node);
			
			obj->move(move.node, *new_node->get_tnode_ids()->begin(), move.wait, move.move_time, move.velocity, move.new_position, new_node->_attachment_point_validity[prefab_id], interpolate, new_node->get_floor_num(), new_node->_stair_ids[prefab_id], old_pot < move.potential, seconds, get_desired_vel(obj, new_node));

			set_occupation_halo_obj(occ, *obj, seconds);
			
			obj->set_blocking_width(calc_blocking_dist(move.velocity, obj->get_verticies_relative(), obj->get_floor(), obj->stair_id));
		}
	}
	else if (interpolate) // wait > 0
	{
		obj->_position += (obj->velocity_current.first * seconds);
		obj->_orientation += (obj->velocity_current.second * seconds);
		obj->moved = false;
	}
	else
	{
		obj->moved = false;
		obj->just_moved = false;
	}
}
void ObjectSpace::delay_obj(int object_id)
{
	log_main.print("delay_obj");
	GenObject* obj = get_object(object_id);

	obj->wait = obj->current_task.delay;

	obj->moved = false;
	obj->not_move_cost = 0.0f;
	obj->match_cnodes();
	obj->velocity_desired = { 0.0f,0.0f };
	if (!obj->stopped)
	{
		obj->just_stopped = true;
		obj->stopped = true;
	}
	else
	{
		obj->just_stopped = false;
	}
}
void ObjectSpace::idle_obj(int object_id)
{
	log_main.print("idle_obj\n");
	GenObject* obj = get_object(object_id);
	if (obj)
	{
		obj->idle();
	}
}
std::pair<std::map<int, bool>, std::map<int, bool>> ObjectSpace::action_all_tasks(float time_sec) // first = task completed, second = object deleted
{
	log_main.print("action_all_tasks");

	std::pair<std::map<int, bool>, std::map<int, bool>> out;

	std::vector<int> task_order = rnd_get_task_order();

	log_main.print("task order:");
	for (int i : task_order)
	{
		log_main.print("%d",i);
	}

	for (int i : task_order)
	{
		std::pair<bool, bool> comp_del = action_task(i, time_sec);
		
		out.first[i] = comp_del.first;
		out.second[i] = comp_del.second;
	}

	return out;
}
std::vector<int> ObjectSpace::rnd_get_task_order()
{
	std::vector<int> order;

	float total_drive = 0.0f;
	std::map<int, float> id_drive;

	// populate id_drive and total_drive
	for (GenObject& obj : m_objects)
	{
		if (obj.active && obj.get_drive() > 0.0f)
		{
			id_drive[obj.get_object_id()] = obj.get_drive();
			total_drive += obj.get_drive();
		}
	}

	while (!id_drive.empty() && total_drive > 0.0f)
	{
		std::uniform_real_distribution<float> distribution(0.0, total_drive);
		float value = distribution(m_generator);
		for (std::pair<const int, float>& p : id_drive)
		{
			value -= p.second;
			if (value < 0.0f)
			{
				order.push_back(p.first);
				total_drive -= p.second;
				id_drive.erase(p.first);
				break;
			}
		}

		if (value >= 0.0f) // if nothing was added to id_drive then add last element
		{
			order.push_back(id_drive.rbegin()->first);
			total_drive -= id_drive.rbegin()->second;
			id_drive.erase(id_drive.rbegin()->first);
		}
	}

	for (std::pair<const int, float>& p : id_drive) // add rest of id_drive in case one was missed
	{
		order.push_back(p.first);
	}

	return order;
}
float ObjectSpace::calc_blocking_dist(vector2 velocity, std::vector<x_y<float>> verts, int floor, int stair_id)
{
	if (velocity.is_zero()) return INFINITY;
	
	velocity = { velocity.y, -velocity.x };
	velocity.norm_this();

	std::vector<start_end<x_y<float>>> bounds;

	stair* st = get_stairpt_from_id(stair_id);

	if (st)
	{
		bounds.push_back(st->left_wall);
		bounds.push_back(st->right_wall);
	}
	else
	{
		for (boundary_line& b : m_bounds)
		{
			if (b.floor_num == floor)
			{
				bounds.push_back(b.position);
			}
		}
	}
	
	float smallest_neg = INFINITY;
	float smallest_pos = INFINITY;

	for (start_end<x_y<float>>& b1 : bounds)
	{
		for (auto v_start = verts.begin(); v_start != verts.end(); ++v_start)
		{
			auto v_end = v_start + 1;
			if (v_end == verts.end()) v_end = verts.begin();

			start_end<x_y<float>> b2 = { b1.start + *v_start - *v_end , b1.end + *v_start - *v_end };

			std::optional<float> d_4[4] =
			{
				point_to_line_in_direction(*v_start, b1.start, b1.end,   velocity),
				point_to_line_in_direction(*v_start, b1.end,   b2.end,   velocity),
				point_to_line_in_direction(*v_start, b1.start, b2.start, velocity),
				point_to_line_in_direction(*v_start, b2.start, b2.end,   velocity)
			};

			for (std::optional<float>& d : d_4)
			{
				if (d)
				{
					if (d.value() < 0.0f)
					{
						float f = -d.value();
						if (f < smallest_neg) smallest_neg = f;
					}
					else if (d.value() > 0.0f)
					{
						if (d.value() < smallest_pos) smallest_pos = d.value();
					}
				}
			}
		}
	}

	return MAX(smallest_pos, smallest_neg);
}
vector2 ObjectSpace::get_desired_vel(GenObject* obj, CSNode* n)
{
	int prefab_id = obj->get_object_prefab_id();
	int object_id = obj->get_object_id();

	float max_lin_speed = obj->get_max_linear_speed(NOT_STAIR_ARC, -1);
	float max_ang_speed = obj->get_max_angular_speed();

	x_y<float> pos = n->get_position();

	float lowest_pot = INFINITY;
	vector2 des_vel = { 0.0f,0.0f };

	for (CSArc& arc : n->_arcs)
	{
		if (arc.turn_on_spot && !obj->can_turn_on_spot()) continue;

		float holo = obj->get_holonomicity(arc.holo);
		if (holo == 0.0f || !obj->can_take_stair(arc.stair_dir, n->_stair_ids[prefab_id])) continue;

		cnode_pos this_vec_pos = arc.cnode_to_vec_pos;
		CSNode* this_node = get_node(this_vec_pos);
		if (!this_node || !this_node->is_valid(prefab_id)) continue;

		float this_pot = this_node->get_potential(object_id);
		if (this_pot < 0.0f) continue;

		float weight = 0.0f;

		if (!arc.turn_on_spot) // trans
		{
			float t_t = arc.length / (max_lin_speed);
			if (!arc.same_layer)
			{
				// trans and rot
				float t_a = fLAYER_GAP / max_ang_speed;
				weight = sqrt(t_t * t_t + t_a * t_a);
			}
			else
			{
				// trans and !rot
				weight = t_t;
			}
		}
		else if (!arc.same_layer) // rot
		{
			// !trans and rot
			weight = fLAYER_GAP / max_ang_speed;
		}
		else
		{
			// !trans and !rot
			continue;
		}

		weight /= holo;
		
		this_pot += weight;

		if (this_pot < lowest_pot)
		{
			lowest_pot = this_pot;

			des_vel = this_node->get_position() - pos;
			des_vel /= weight;
		}
	}

	return des_vel;
}

// save state
void ObjectSpace::save_state()
{
	m_state.saved = true;
	m_state.objects = m_objects;
	m_state.people = m_people;
	m_state.nodes = m_EX_nodes;

	log_main.print("sim state saved");
}
void ObjectSpace::retrieve_state()
{
	if (m_state.saved)
	{
		reset_all_potentials();

		m_objects = m_state.objects;
		m_people = m_state.people;
		m_EX_nodes = m_state.nodes;
		sim_time = 0.0f;

		log_main.print("sim state retrieved");
	}

	log_main.print("sim state not retrieved - was never saved");
}

ObjectSpace::ObjectSpace()
{
	cspace_ready = false;
	freespace_ready = false;
	m_next_node_id = 0;
	m_next_arc_id = 0;
	m_next_stair_id = 0;
	m_next_landing_id = 0;
	m_next_bound_id = 0;
	m_next_prefab_id = 1;
	m_next_object_id = 1;
	m_generator.seed(GetTickCount());
	sim_time = 0.0f;

	// default options
	vo_hybrid = true;
	vo_generalised = true;
	vo_time_to_collision = 4.0f;
	vo_add_dist_cost = 0.36f;
	vo_l_r_priority = 0.5f;
	min_time_scale_stopped_ped = 0.25f;
	num_runs_per_sim = 2;
	tcp_PortID = 9002;
	data_sim = true;
	data_adj_matrix = false;
	data_adj_list = true;
	data_cnode_arc_info = true;
	data_tnode_arc_info = true;
	data_object_info = true;
	vo_des_vel_f = 0.5f;
}
ObjectSpace::~ObjectSpace(){}

std::vector<CSNode>::iterator ObjectSpace::nodes_begin(Orient layer)
{
	return m_cspace._cspace[layer].begin();
}
std::vector<CSNode>::iterator ObjectSpace::nodes_end(Orient layer)
{
	return m_cspace._cspace[layer].end();
}
std::vector<CSNode>::iterator ObjectSpace::nodes_begin()
{
	return m_cspace._LD_cspace.begin();
}
std::vector<CSNode>::iterator ObjectSpace::nodes_end()
{
	return m_cspace._LD_cspace.end();
}
CSNode* ObjectSpace::get_node(cnode_pos pos)
{
	if (pos.vec_pos < m_cspace.size(pos.orientation))
		return m_cspace.get_cnode_pt(pos);
	else
		return nullptr;
}
CSNode* ObjectSpace::get_LD_node(int pos)
{
	if (pos < m_cspace.size(0))
		return m_cspace.LD_get_cnode_pt(pos);
	else
		return nullptr;
}
CSNode* ObjectSpace::get_nearest_cnode(float x, float y, Orient o, int floor)
{
	return m_cspace.get_cnode_pt(x, y, o, floor);
}
bool ObjectSpace::is_ready()
{
	return cspace_ready && freespace_ready;
}
int ObjectSpace::layer_size(Orient layer)
{
	return m_cspace.size(layer);
}

std::vector<occupation_seed> ObjectSpace::get_occupation_seeds()
{
	std::vector<occupation_seed> seeds;
	if (!m_objects.empty())
	{
		for (std::vector<GenObject>::iterator it = m_objects.begin(); it != m_objects.end(); ++it)
		{
			seeds.push_back(it->generate_occupation_seed());
		}
	}
	return seeds;
}
std::vector<data_for_TCP::object_config> ObjectSpace::main_sim_init_1()
{
	using namespace data_for_TCP;

	std::vector<object_config> configs;

	for (GenObject& obj : m_objects)
	{
		configs.push_back(object_config(
			obj.get_object_id(),
			obj.get_object_prefab_id(),
			obj.get_floor(),
			obj._position.x,
			obj._position.y,
			obj.get_EX_angle(),
			obj.velocity_current.first.magnitude(),
			0
		));
	}

	return configs;
}
std::vector<data_for_TCP::occupied_nodes> ObjectSpace::main_sim_init_2()
{
	using namespace data_for_TCP;

	std::vector<occupied_nodes> occ_nodes;

	for (GenObject& obj : m_objects)
	{
		occ_nodes.push_back(occupied_nodes(
			obj.get_object_id(),
			obj.get_occupation_tnodes()
		));
	}

	return occ_nodes;
}
std::vector<data_for_TCP::stair_node> ObjectSpace::main_sim_init_stair_node()
{
	std::vector<data_for_TCP::stair_node> out;
	for (const stair& st : m_EX_stairs)
	{
		std::vector<data_for_TCP::stair_node> temp = data_for_TCP::get_stair_nodes(st);
		out.insert(out.end(), temp.begin(), temp.end());
	}

	return out;
}
std::vector<data_for_TCP::object_config> ObjectSpace::main_sim_step_1(float time_sec, bool first)
{
	// Action current task

	log_main.print("main_sim_step_1");
	sim_time = time_sec;
	std::pair<std::map<int, bool>, std::map<int, bool>> completed_deleted = action_all_tasks(time_sec);

	using namespace data_for_TCP;

	std::vector<object_config> configs;

	for (GenObject& obj : m_objects)
	{
		if (obj.moved)
		{
			configs.push_back(object_config(
				obj.get_object_id(),
				obj.get_object_prefab_id(),
				obj.get_floor(),
				obj._position.x,
				obj._position.y,
				obj.get_EX_angle(),
				obj.velocity_current.first.magnitude(),
				completed_deleted.first[obj.get_object_id()] ? 2 : 0
			));
		}
		else if (completed_deleted.first[obj.get_object_id()])
		{
			configs.push_back(object_config(
				obj.get_object_id(),
				obj.get_object_prefab_id(),
				obj.get_floor(),
				obj._position.x,
				obj._position.y,
				obj.get_EX_angle(),
				obj.velocity_current.first.magnitude(),
				1
			));
		}
	}

	for (const auto& del : completed_deleted.second)
	{
		if (del.second)
		{
			configs.push_back(object_config(del.first, true));
		}
	}

	return configs;
}
std::vector<data_for_TCP::occupied_nodes> ObjectSpace::main_sim_step_2()
{
	// Set new occupied nodes

	log_main.print("main_sim_step_2");
	using namespace data_for_TCP;

	std::vector<occupied_nodes> occ_nodes;

	for (GenObject& obj : m_objects)
	{
		if (obj.moved)
		{
			occ_nodes.push_back(occupied_nodes(
				obj.get_object_id(),
				obj.get_occupation_tnodes()
			));
		}
	}

	return occ_nodes;
}
std::vector<data_for_TCP::pvo> ObjectSpace::main_sim_step_3(bool first)
{
	// Calculate new VOs and PVOs for avoidance

	log_main.print("main_sim_step_3");
	update_all_vos(first, vo_generalised, vo_hybrid, vo_time_to_collision, 0.2f, vo_add_dist_cost);

	using namespace data_for_TCP;

	std::vector<pvo> pvos;

	for (auto& per : m_people)
	{
		if (per.second.in_object) continue;
		std::map<int, std::map<int, float>> node_obj_cost;
		node* current_node = get_tnode_pnt(per.second.node_id);
		if (!current_node) continue;
		vector2 current_pos = current_node->position;

		float speed = per.second.velocity.magnitude();
		if (speed < per.second.default_speed) speed = per.second.default_speed;
		
		std::vector<int> nodes = current_node->tnodes; // all nodes connected to current node
		nodes.push_back(per.second.node_id);
		REMOVE_DUPLICATES(nodes);


		for (int& next_id : nodes)
		{
			vector2 next_pos = get_tnode_pnt(next_id)->position;
			for (velocity_obstacle& vo : per.second.pvos)
			{
				if (!vo.valid) continue;

				vector2 next_vel = (next_pos - current_pos);
				next_vel.resize(speed);
				float time_cost = 0.0f;
				if (vo.velocity_in_vo_col_time(next_vel, time_cost))
				{
					time_cost *= speed;
					(node_obj_cost[next_id])[vo.other_ent_id] = time_cost;
				}
			}
		}

		if (!node_obj_cost.empty())
		{
			pvos.push_back(pvo(per.first, node_obj_cost));
		}
	}

	pvos_store.clear();
	pvos_store = pvos;

	return pvos;
}

data_for_TCP::all_sim_data ObjectSpace::main_sim_step_ALL(float time_sec, bool first)
{
	data_for_TCP::all_sim_data data;

	data.configs = main_sim_step_1(time_sec, first);
	data.occ_nodes = main_sim_step_2();
	data.vos = main_sim_step_3(first);

	return data;
}

// test / debug
std::map<int, int> ObjectSpace::test_duplicate_ids_tnodes()
{
	std::map<int, int> duplicates; // id, number
	for (node& n : m_EX_nodes)
	{
		++duplicates[n.id];
	}

	return duplicates;
}
std::map<int, int> ObjectSpace::test_duplicate_ids_tarcs()
{
	std::map<int, int> duplicates; // id, number
	for (arc& a : m_EX_arcs)
	{
		++duplicates[a.id];
	}

	return duplicates;
}
std::map<cnode_pos, int> ObjectSpace::test_duplicate_ids_cnodes()
{
	std::map<cnode_pos, int> duplicates; // id, number
	for (int i = 0; i < iNUM_ORIENT; i++)
	{
		for (CSNode& n : m_cspace._cspace[i])
		{
			++duplicates[n.get_vec_pos()];
		}
	}

	return duplicates;
}

void ObjectSpace::print_details_tnode(int id)
{
	node* n = TCP_get_node_from_id(id);
	if (!n)
	{
		log_main.print("cannot show details: node id %d does not exist", id);
		return;
	}
	log_main.print(2, "Node_id: %d", id);
	log_main.print(2, "xpos: %f", n->position.x);
	log_main.print(2, "ypos: %f", n->position.y);
	log_main.print(2, "landing: %d", n->landing_id);
	log_main.print(2, "stair:   %d", n->stair_id);
	log_main.print(2, "connected tnodes:");
	for (int i : n->tnodes)
	{
		log_main.print(4, "id: %d", i);
	}
}
void ObjectSpace::print_details_tarc(int id)
{
	
}
void ObjectSpace::print_details_bound(int id)
{

}
void ObjectSpace::print_move(potential_move& move)
{
	log_main.print(move.valid ? "potential_move to node %d (valid)" : "potential_move to node %d (invalid)", move.node.vec_pos);
	log_main.print(4, "potential %f", move.potential);
	log_main.print(4, "weight %f", move.weight);
	log_main.print(4, "wait %f", move.wait);
}

void ObjectSpace::correct_tarc_pos()
{
	for (arc& a : m_EX_arcs)
	{
		a.position.start = TCP_get_node_from_id(a.tnode_ids.start)->position;
		a.position.end = TCP_get_node_from_id(a.tnode_ids.end)->position;
	}
}
