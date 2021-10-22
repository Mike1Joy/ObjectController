#include "FileIO.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "DataStructs.h"

#define FILE_NODE 0
#define FILE_ARC 1
#define FILE_BOUND 2
#define FILE_STAIR 3
#define FILE_LAND 4
#define FILE_CAMERA 5

using namespace std;
using namespace ObjCont;

#define FILL_INIT(T)											\
T in;															\
																\
bool failed = false;											\
int fail_col = 1;												\
std::string fail_type;											\
																\
if (blocks.size() < in.types.size() + 1)						\
{																\
	failed = true;												\
	fail_type = "not enough data for this code";				\
}																\
																\
if (!failed)													\
{																\
	std::optional<int> iopt;									\
	std::optional<float> fopt;									\
	std::optional<bool> bopt;									\
																\
	for (int i = 1; i < in.types.size() + 1; ++i)				\
	{															\
		switch (in.types[i - 1])								\
		{														\
		case INT:												\
			iopt = string_to_int(&blocks[i]);					\
			if (iopt.has_value())								\
			{													\
				in.content.push_back(*iopt);					\
			}													\
			else												\
			{													\
				failed = true;									\
				fail_col = i + 1;								\
				fail_type = "expected an int";					\
			}													\
			iopt.reset();										\
			break;												\
		case FLOAT:												\
			fopt = string_to_float(&blocks[i]);					\
			if (fopt.has_value())								\
			{													\
				in.content.push_back(*fopt);					\
			}													\
			else												\
			{													\
				failed = true;									\
				fail_col = i + 1;								\
				fail_type = "expected a float";					\
			}													\
			fopt.reset();										\
			break;												\
		case BOOL:												\
			bopt = string_to_bool(&blocks[i]);					\
			if (bopt.has_value())								\
			{													\
				in.content.push_back(*bopt);					\
			}													\
			else												\
			{													\
				failed = true;									\
				fail_col = i + 1;								\
				fail_type = "expected a bool";					\
			}													\
			bopt.reset();										\
			break;												\
		default:												\
			failed = true;										\
			break;												\
			}													\
																\
			if (failed) break;									\
		}														\
}

#define STRING_TO_VAR(type, var)			\
type##opt = string_to_##type(&block);		\
if (type##opt.has_value())					\
{											\
	var = *type##opt;						\
}											\
else										\
{											\
	failed = true;							\
}											\
type##opt.reset();

// INIT
const std::string FileIO::prefab_init::csv_code("ADD_PREFAB_22");
const std::string FileIO::vertex_init::csv_code("ADD_VERTEX_3");
const std::string FileIO::ext_attach_init::csv_code("ADD_EXT_ATT_POINT_12");
const std::string FileIO::int_attach_init::csv_code("ADD_INT_ATT_POINT_8");
const std::string FileIO::options_init::csv_code("OPTION_14");
const std::string FileIO::mode_init::csv_code("MODE_5");

FileIO::init FileIO::get_init()
{
	init init_out;
	ifstream input("init.csv");
	if (!input.good()) return init_out;
	init_out.failed = false;

	int counter = 0;
	string line;
	while (getline(input, line))
	{
		counter++;
		vector<string> blocks = get_blocks(line);

		if (blocks[0] == prefab_init::csv_code)
		{
			FILL_INIT(prefab_init);
			if (!failed)
				init_out.prefabs.push_back(in);
			else
			{
				char buff[100];
				sprintf(buff, "Failed to read line %d, col %d: %s", counter, fail_col, fail_type.c_str());
				init_out.errors.push_back(buff);
			}
		}
		else if (blocks[0] == vertex_init::csv_code)
		{
			FILL_INIT(vertex_init);
			if (!failed)
				init_out.vertices.push_back(in);
			else
			{
				char buff[100];
				sprintf(buff, "Failed to read line %d, col %d: %s", counter, fail_col, fail_type.c_str());
				init_out.errors.push_back(buff);
			}
		}
		else if (blocks[0] == ext_attach_init::csv_code)
		{
			FILL_INIT(ext_attach_init);
			if (!failed)
				init_out.ext_points.push_back(in);
			else
			{
				char buff[100];
				sprintf(buff, "Failed to read line %d, col %d: %s", counter, fail_col, fail_type.c_str());
				init_out.errors.push_back(buff);
			}
		}
		else if (blocks[0] == int_attach_init::csv_code)
		{
			FILL_INIT(int_attach_init);
			if (!failed)
				init_out.int_points.push_back(in);
			else
			{
				char buff[100];
				sprintf(buff, "Failed to read line %d, col %d: %s", counter, fail_col, fail_type.c_str());
				init_out.errors.push_back(buff);
			}
		}
		else if (blocks[0] == options_init::csv_code)
		{
			FILL_INIT(options_init);
			if (!failed)
				init_out.options = in;
			else
			{
				char buff[100];
				sprintf(buff, "Failed to read line %d, col %d: %s", counter, fail_col, fail_type.c_str());
				init_out.errors.push_back(buff);
			}
		}
		else if (blocks[0] == mode_init::csv_code)
		{
			FILL_INIT(mode_init);
			if (!failed)
				init_out.mode = in;
			else
			{
				char buff[100];
				sprintf(buff, "Failed to read line %d, col %d: %s", counter, fail_col, fail_type.c_str());
				init_out.errors.push_back(buff);
			}
		}
		else if (blocks[0] == string("PRE_PRINT") && blocks.size() > 1)
		{
			init_out.pre_prints.push_back(blocks[1]);
		}
		else if (blocks[0] == string("POST_PRINT") && blocks.size() > 1)
		{
			init_out.post_prints.push_back(blocks[1]);
		}

		line.clear();
	}

	return init_out;
}

std::vector<std::string> FileIO::get_blocks(const string& line)
{
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(line);
	while (std::getline(tokenStream, token, ','))
	{
		tokens.push_back(token);
	}
	return tokens;
}

//save
void FileIO::output_line_to_file(const node* node, ofstream& output)
{
	output << node->position.x << "," << node->position.y << "," << (int)(node->id) << "," << (int)(node->floor_num) << "," << (int)(node->stair_id) << "," << (int)(node->landing_id) << "," << endl;
}
void FileIO::output_line_to_file(const arc* arc, std::ofstream& output)
{
	output << (int)(arc->tnode_ids.start) << "," << (int)(arc->tnode_ids.end) << "," << arc->position.start.x << "," << arc->position.start.y << "," << arc->position.end.x << "," << arc->position.end.y << "," << arc->id << "," << endl;
}
void FileIO::output_line_to_file(const boundary_line* bound, ofstream& output)
{
	output << bound->position.start.x << "," << bound->position.start.y << "," << bound->position.end.x << "," << bound->position.end.y << ","<< bound->id << "," << bound->floor_num << "," << endl;
}
void FileIO::output_line_to_file(const stair* st, ofstream& output)
{
	output <<
		st->id << "," <<
		st->position.x << "," <<
		st->position.y << "," <<
		st->direction << "," <<
		st->lanes << "," <<
		st->riser_num << "," <<
		st->length << "," <<
		st->width << "," <<
		st->height << "," <<
		st->floor_num << "," <<
		endl;
}
void FileIO::output_line_to_file(const landing* land, ofstream& output)
{
	output <<
		land->id << "," <<
		land->down_stair_id << "," <<
		land->up_stair_id << "," <<
		land->opposite_wall.start.x << "," <<
		land->opposite_wall.start.y << "," <<
		land->opposite_wall.end.x << "," <<
		land->opposite_wall.end.y << "," <<
		endl;
}
void FileIO::output_line_to_file(const char* text, ofstream& output)
{
	output << text << endl;
}
void FileIO::output_line_to_file(float zoom, float camerax, float cameray, ofstream& output)
{
	output << zoom << "," << camerax << "," << cameray << "," << endl;
}

bool FileIO::save_geometry(const char* filename, const vector<node>& node_vec, const vector<arc>& arc_vec, const vector<boundary_line>& bound_vec, const std::vector<stair>& stair_vec, const std::vector<landing>& land_vec, float zoom, float camerax, float cameray)
{
	ofstream output;
	output.open(filename, ios::out);

	output_line_to_file("N", output);
	for (auto it = node_vec.begin(); it != node_vec.end(); ++it)
	{
		output_line_to_file(&(*it), output);
	}

	output_line_to_file("A", output);
	for (auto it = arc_vec.begin(); it != arc_vec.end(); ++it)
	{
		output_line_to_file(&(*it), output);
	}

	output_line_to_file("B", output);
	for (auto it = bound_vec.begin(); it != bound_vec.end(); ++it)
	{
		output_line_to_file(&(*it), output);
	}

	output_line_to_file("S", output);
	for (auto it = stair_vec.begin(); it != stair_vec.end(); ++it)
	{
		output_line_to_file(&(*it), output);
	}

	output_line_to_file("L", output);
	for (auto it = land_vec.begin(); it != land_vec.end(); ++it)
	{
		output_line_to_file(&(*it), output);
	}

	output_line_to_file("C", output);
	output_line_to_file(zoom, camerax, cameray, output);

	output.close();

	return true;
}

//load
std::optional<int> FileIO::string_to_int(std::string* s)
{
	try
	{
		if (s->size() == 0) return {};

		std::string::size_type sz;
		int i = std::stoi(*s, &sz);
		return i;
	}
	catch (...)
	{
		return {};
	}
}
std::optional<float> FileIO::string_to_float(std::string* s)
{
	try 
	{
		if (s->size() == 0) return {};

		std::string::size_type sz;
		return std::stof(*s, &sz);
	}
	catch(...)
	{
		return {};
	}
}
std::optional<bool> FileIO::string_to_bool(std::string* s)
{
	if (s->empty() || s->size() > 1) return {};

	if (s->at(0) == '0') return false;
	if (s->at(0) == '1') return true;

	return {};
}

FileIO::Node_construct FileIO::text_to_node(std::string* s)
{
	Node_construct node;

	int blocknum = 0;
	string block;

	bool failed = false;
	std::optional<int> intopt;
	std::optional<float> floatopt;
	std::optional<bool> boolopt;

	for (int i = 0; i < s->length()+1; ++i)
	{
		char c;
		if (i == s->length())
		{
			c = ',';
		}
		else
		{
			c = s->at(i);
		}
		
		if (c == ',')
		{
			switch (blocknum)
			{
			case 0:
				STRING_TO_VAR(float, node.xPos);
				break;
			case 1:
				STRING_TO_VAR(float, node.yPos);
				break;
			case 2:
				STRING_TO_VAR(int, node.id);
				break;
			case 3:
				STRING_TO_VAR(int, node.floor);
				break;
			case 4:
				STRING_TO_VAR(int, node.stairid);
				break;
			case 5:
				STRING_TO_VAR(int, node.landingid);
				break;
			}

			if (failed) break;

			++blocknum;
			block.clear();
		}
		else
		{
			block += c;
		}
	}

	node.failed = failed;

	return node;
}
FileIO::Arc_construct FileIO::text_to_arc(std::string* s)
{
	Arc_construct arc;

	int blocknum = 0;
	string block;
	bool failed = false;
	std::optional<int> intopt;
	std::optional<float> floatopt;
	std::optional<bool> boolopt;

	for (int i = 0; i < s->length(); ++i)
	{
		char c;
		if (i == s->length())
		{
			c = ',';
		}
		else
		{
			c = s->at(i);
		}

		if (c == ',')
		{
			switch (blocknum)
			{
			case 0:
				STRING_TO_VAR(int, arc.start_node);
				break;
			case 1:
				STRING_TO_VAR(int, arc.end_node);
				break;
			case 2:
				STRING_TO_VAR(float, arc.start_x);
				break;
			case 3:
				STRING_TO_VAR(float, arc.start_y);
				break;
			case 4:
				STRING_TO_VAR(float, arc.end_x);
				break;
			case 5:
				STRING_TO_VAR(float, arc.end_y);
				break;
			case 6:
				STRING_TO_VAR(int, arc.id);
				break;
			}

			if (failed) break;

			++blocknum;
			block.clear();
		}
		else
		{
			block += c;
		}
	}

	arc.failed = failed;

	return arc;
}
FileIO::BoundaryLine_construct FileIO::text_to_bound(std::string* s)
{
	BoundaryLine_construct bound;

	int blocknum = 0;
	string block;
	bool failed = false;
	std::optional<int> intopt;
	std::optional<float> floatopt;
	std::optional<bool> boolopt;

	for (int i = 0; i < s->length(); ++i)
	{
		char c;
		if (i == s->length())
		{
			c = ',';
		}
		else
		{
			c = s->at(i);
		}
		if (c == ',')
		{
			switch (blocknum)
			{
			case 0:
				STRING_TO_VAR(float, bound.startX);
				break;
			case 1:
				STRING_TO_VAR(float, bound.startY);
				break;
			case 2:
				STRING_TO_VAR(float, bound.endX);
				break;
			case 3:
				STRING_TO_VAR(float, bound.endY);
				break;
			case 4:
				STRING_TO_VAR(int, bound.id);
				break;
			case 5:
				STRING_TO_VAR(int, bound.floor);
				break;
			}

			if (failed) break;

			++blocknum;
			block.clear();
		}
		else
		{
			block += c;
		}
	}

	bound.failed = failed;

	return bound;
}
FileIO::Stair_construct FileIO::text_to_stair(std::string* s)
{
	Stair_construct st;

	int blocknum = 0;
	string block;
	bool failed = false;
	std::optional<int> intopt;
	std::optional<float> floatopt;
	std::optional<bool> boolopt;

	for (int i = 0; i < s->length(); ++i)
	{
		char c;
		if (i == s->length())
		{
			c = ',';
		}
		else
		{
			c = s->at(i);
		}
		if (c == ',')
		{
			switch (blocknum)
			{
			case 0:
				STRING_TO_VAR(int, st.id);
				break;
			case 1:
				STRING_TO_VAR(float, st.xposition);
				break;
			case 2:
				STRING_TO_VAR(float, st.yposition);
				break;
			case 3:
				STRING_TO_VAR(float, st.direction);
				break;
			case 4:
				STRING_TO_VAR(int, st.lanes);
				break;
			case 5:
				STRING_TO_VAR(int, st.riser_num);
				break;
			case 6:
				STRING_TO_VAR(float, st.length);
				break;
			case 7:
				STRING_TO_VAR(float, st.width);
				break;
			case 8:
				STRING_TO_VAR(float, st.height);
				break;
			case 9:
				STRING_TO_VAR(int, st.floor_num);
				break;
			}

			if (failed) break;

			++blocknum;
			block.clear();
		}
		else
		{
			block += c;
		}
	}

	st.failed = failed;

	return st;
}
FileIO::Landing_construct FileIO::text_to_land(std::string* s)
{
	Landing_construct land;

	int blocknum = 0;
	string block;
	bool failed = false;
	std::optional<int> intopt;
	std::optional<float> floatopt;
	std::optional<bool> boolopt;

	for (int i = 0; i < s->length(); ++i)
	{
		char c;
		if (i == s->length())
		{
			c = ',';
		}
		else
		{
			c = s->at(i);
		}
		if (c == ',')
		{
			switch (blocknum)
			{
			case 0:
				STRING_TO_VAR(int, land.id);
				break;
			case 1:
				STRING_TO_VAR(int, land.down_stair_id);
				break;
			case 2: 
				STRING_TO_VAR(int, land.up_stair_id);
				break;
			case 3:
				STRING_TO_VAR(float, land.wall_start_x);
				break;
			case 4:
				STRING_TO_VAR(float, land.wall_start_y);
				break;
			case 5:
				STRING_TO_VAR(float, land.wall_end_x);
				break;
			case 6:
				STRING_TO_VAR(float, land.wall_end_y);
				break;
			}

			if (failed) break;
			++blocknum;
			block.clear();
		}
		else
		{
			block += c;
		}
	}

	land.failed = failed;

	return land;
}
void FileIO::text_to_camera(std::string* s, float* zoom, float* camerax, float* cameray)
{
	int blocknum = 0;
	string block;

	bool failed = false;
	std::optional<int> intopt;
	std::optional<float> floatopt;
	std::optional<bool> boolopt;

	for (int i = 0; i < s->length(); ++i)
	{
		char c;
		if (i == s->length())
		{
			c = ',';
		}
		else
		{
			c = s->at(i);
		}
		if (c == ',')
		{
			switch (blocknum)
			{
			case 0:
				STRING_TO_VAR(float, *zoom);
				break;
			case 1:
				STRING_TO_VAR(float, *camerax);
				break;
			case 2:
				STRING_TO_VAR(float, *cameray);
				break;
			}
			++blocknum;
			block.clear();
		}
		else
		{
			block += c;
		}
	}
}

bool FileIO::read_file(const char* filename, vector<Node_construct>* nodes, vector<Arc_construct>* arcs, vector<BoundaryLine_construct>* bounds, std::vector<Stair_construct>* stairs, std::vector<Landing_construct>* landings, float* zoom, float* camerax, float* cameray)
{
	ifstream input(filename);

	if (!input.good()) return false;

	nodes->clear();
	arcs->clear();
	bounds->clear();

	int mode = FILE_NODE;
	bool read_this_line = true;

	string line;
	while (getline(input, line))
	{
		read_this_line = true;

		//work out what mode we are in
		if (line.at(0) == 'N') { mode = FILE_NODE; read_this_line = false; }
		else if (line.at(0) == 'A') { mode = FILE_ARC; read_this_line = false; }
		else if (line.at(0) == 'B') { mode = FILE_BOUND; read_this_line = false; }
		else if (line.at(0) == 'S') { mode = FILE_STAIR; read_this_line = false; }
		else if (line.at(0) == 'L') { mode = FILE_LAND; read_this_line = false; }
		else if (line.at(0) == 'C') { mode = FILE_CAMERA; read_this_line = false; }

		if (read_this_line)
		{
			switch (mode)
			{
			case FILE_NODE:
				nodes->push_back(text_to_node(&line));
				break;
			case FILE_ARC:
				arcs->push_back(text_to_arc(&line));
				break;
			case FILE_BOUND:
				bounds->push_back(text_to_bound(&line));
				break;
			case FILE_STAIR:
				stairs->push_back(text_to_stair(&line));
				break;
			case FILE_LAND:
				landings->push_back(text_to_land(&line));
				break;
			case FILE_CAMERA:
				text_to_camera(&line, zoom, camerax, cameray);
				break;
			}
		}

		line.clear();
	}

	return true;
}

// Data output
bool FileIO::data_output_node_adj_matrix(const char* filename, const std::vector<int>& nodes, const std::vector<bool>& adj)
{
	int num_nodes = nodes.size();
	if (adj.size() != num_nodes * num_nodes)
		return false;

	std::ofstream data_ofs;
	data_ofs.open(filename, std::ofstream::out | std::ofstream::trunc);
	{
		if (!data_ofs.is_open()) return false;

		// top line
		data_ofs << ",";
		for (const int& id : nodes)
		{
			data_ofs << id << ",";
		}

		data_ofs << endl;

		// left line + adjacency
		for (int row = 0; row < num_nodes; ++row)
		{
			data_ofs << nodes[row] << ",";

			for (int col = 0; col < num_nodes; ++col)
			{
				data_ofs << adj[row*num_nodes + col] << ",";
			}

			data_ofs << endl;
		}
	} data_ofs.close();
	
	return true;
}
bool FileIO::data_output_node_adj_list(const char* filename, const std::map<int, std::vector<int>>& node_adj)
{
	std::ofstream data_ofs;
	data_ofs.open(filename, std::ofstream::out | std::ofstream::trunc);
	{
		if (!data_ofs.is_open()) return false;

		for (const auto& n_adj : node_adj)
		{
			data_ofs << n_adj.first << ",";

			for (const int& adj : n_adj.second)
			{
				data_ofs << adj << ",";
			}

			data_ofs << endl;
		}
	} data_ofs.close();

	return true;
}

bool FileIO::data_output_cnode_carc_info(const char* filename, const std::vector<CSNode>(&csnodes)[iNUM_ORIENT])
{
	std::ofstream data_ofs;

	data_ofs.open(filename, std::ofstream::out | std::ofstream::trunc);
	{
		if (!data_ofs.is_open()) return false;

		data_ofs << "orient,vector_pos,floor_num,x,y,landing_id,stair_id,arcs_trans,arcs_rot,arcs_trans_rot" << endl;

		for (const auto& n_v : csnodes)
		{
			for (const auto& n : n_v)
			{
				int trans = 0;
				int rot = 0;
				int trans_rot = 0;

				for (const auto& a : n._arcs)
				{
					if (a.same_layer)
						++trans;
					else if (a.turn_on_spot)
						++rot;
					else
						++trans_rot;
				}

				data_ofs << n.get_vec_pos().orientation
					<< "," << n.get_vec_pos().vec_pos
					<< "," << n.get_floor_num()
					<< "," << n.get_position().x
					<< "," << n.get_position().y
					<< "," << n.landing_id
					<< "," << n._stair_id
					<< "," << trans
					<< "," << rot
					<< "," << trans_rot
					<< "," << endl;
			}
		}

	} 
	data_ofs.close();

	return true;
}
bool FileIO::data_output_tnode_tarc_info(const char* filename, const std::vector<node>& tnodes, const std::vector<arc>& tarcs)
{
	std::ofstream data_ofs;

	data_ofs.open(filename, std::ofstream::out | std::ofstream::trunc);
	{
		if (!data_ofs.is_open()) return false;

		data_ofs << "id,floor_num,x,y,stair_id,landing_id,num_arcs" << endl;

		std::map<int, int> node_num_arcs;

		for (const auto& a : tarcs)
		{
			++node_num_arcs[a.tnode_ids.start];
			++node_num_arcs[a.tnode_ids.end];
		}

		for (const auto& n : tnodes)
		{
			data_ofs << n.id
				<< "," << n.floor_num
				<< "," << n.position.x
				<< "," << n.position.y
				<< "," << n.stair_id
				<< "," << n.landing_id
				<< "," << node_num_arcs[n.id]
				<< "," << endl;
		}
	}
	data_ofs.close();

	return true;
}

bool FileIO::data_output_object_info(const char* filename, bool first, int step, int sub_step, int comp_time_ms, float sim_time, const std::vector<GenObject>& objs)
{
	std::ofstream data_ofs;

	data_ofs.open(filename,  first ? (std::ofstream::out | std::ofstream::trunc) : (std::ofstream::out | std::ofstream::app));
	{
		if (!data_ofs.is_open()) return false;

		if (first)
			data_ofs << "step,sub_step,comp_time_ms,sim_time,id,prefab_id,floor,stair_id,stair_dir,cnode_orient,cnode_pos,x,y,orient,num_vert,num_attch_points,num_occ_nodes,length,width,max_width,drive,accel,ang_speed,max_lin_speed,vel_x,vel_y,vel_theta,num_vos,itinerary_size,active,stopped,just_stopped,moved,moved_back,not_move_cost,time_block_cor,time_not_block_cor,wait" << endl;

		for (const auto& o : objs)
		{
			data_ofs   << step
				<< "," << sub_step
				<< "," << comp_time_ms
				<< "," << sim_time
				<< "," << o.get_object_id()
				<< "," << o.get_object_prefab_id()
				
				<< "," << o.get_floor()
				<< "," << o.stair_id
				<< "," << stair_arc_dir_str(o.stair_dir)
				<< "," << o.get_cnode_id().orientation
				<< "," << o.get_cnode_id().vec_pos
				<< "," << o._position.x
				<< "," << o._position.y
				<< "," << o._orientation
				
				<< "," << o.get_verticies_relative().size()
				<< "," << o._attachment_points.size()
				<< "," << o.get_occupation_tnodes().size()
				<< "," << o.get_length()
				<< "," << o.get_width()
				<< "," << o.get_max_width()
			
				<< "," << o.get_drive()
				<< "," << o.get_max_acceleration()
				<< "," << o.get_max_angular_speed()
				<< "," << o.get_max_linear_speed(o.stair_dir, o.stair_id)
				<< "," << o.velocity_current.first.x
				<< "," << o.velocity_current.first.y
				<< "," << o.velocity_current.second
				
				<< "," << o.get_vos().size()
				<< "," << o.itinerary.size()
				<< "," << o.active
				<< "," << o.stopped
				<< "," << o.just_stopped
				<< "," << o.moved
				<< "," << o.moved_backwards
				
				<< "," << o.not_move_cost
				<< "," << o.time_blocking_cor
				<< "," << o.time_not_blocking_cor
				<< "," << o.wait

				<< "," << endl;
		}
	}
	data_ofs.close();

	return true;
}

FileIO::FileIO(void)
{
}

FileIO::~FileIO(void)
{
}
