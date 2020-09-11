#pragma once
#include <iostream>
#include <fstream>
#include <variant>
#include <optional>

#include "DataStructs.h"
#include "CSNode.h"
#include "GenObject.h"

using namespace ObjCont;

class FileIO
{
public:
	FileIO(void);
	~FileIO(void);

	// GEO
	struct Node_construct
	{
		bool failed;

		float xPos;
		float yPos;
		int id;
		int floor;
		int landingid;
		int stairid;
	};

	struct Arc_construct
	{
		bool failed;

		int start_node;
		int end_node;
		float start_x;
		float start_y;
		float end_x;
		float end_y;
		int id;
	};

	struct BoundaryLine_construct
	{
		bool failed;

		float startX;
		float startY;
		float endX;
		float endY;
		int id;
		int floor;
	};

	struct Stair_construct
	{
		bool failed;

		int id;
		float xposition;
		float yposition;
		float direction;
		int lanes;
		int riser_num;
		float length;
		float width;
		float height;
		int floor_num;
	};

	struct Landing_construct
	{
		bool failed;

		int id;
		int down_stair_id;
		int up_stair_id;
		float wall_start_x;
		float wall_start_y;
		float wall_end_x;
		float wall_end_y;
	};

	// INIT
	enum TYPE { INT, FLOAT, BOOL };

	struct prefab_init
	{
		static const std::string csv_code;

		std::vector<std::variant<int, float, bool>> content;
		std::vector<TYPE> types =
		{
			INT,
			FLOAT,
			FLOAT,
			FLOAT,
			FLOAT,
			FLOAT,
			FLOAT,
			FLOAT,
			FLOAT,
			FLOAT,
			FLOAT,
			FLOAT,
			FLOAT,
			FLOAT,
			FLOAT,
			BOOL,
			BOOL,
			BOOL,
			BOOL,
			FLOAT,
			BOOL,
			FLOAT
		};

		int prefab_id;
		float max_linear_speed;
		float max_angular_speed;
		float max_stair_dec_speed;
		float max_stair_acc_speed;
		float max_acceleration;
		float holo0;
		float holo1;
		float holo2;
		float holo3;
		float holo4;
		float holo5;
		float holo6;
		float holo7;
		float holo8;
		bool turn_on_spot;
		bool stair_up;
		bool stair_down;
		bool stair_side;
		float landing_entrance;
		bool landing_inside;
		float integral_drive;

		bool fill_values()
		{
			if (content.size() != types.size()) return false;

			if (fill_if<int>(prefab_id, content[0])) return false;
			if (fill_if<float>(max_linear_speed, content[1])) return false;
			if (fill_if<float>(max_angular_speed, content[2])) return false;
			if (fill_if<float>(max_stair_dec_speed, content[3])) return false;
			if (fill_if<float>(max_stair_acc_speed, content[4])) return false;
			if (fill_if<float>(max_acceleration, content[5])) return false;
			if (fill_if<float>(holo0, content[6])) return false;
			if (fill_if<float>(holo1, content[7])) return false;
			if (fill_if<float>(holo2, content[8])) return false;
			if (fill_if<float>(holo3, content[9])) return false;
			if (fill_if<float>(holo4, content[10])) return false;
			if (fill_if<float>(holo5, content[11])) return false;
			if (fill_if<float>(holo6, content[12])) return false;
			if (fill_if<float>(holo7, content[13])) return false;
			if (fill_if<float>(holo8, content[14])) return false;
			if (fill_if<bool>(turn_on_spot, content[15])) return false;
			if (fill_if<bool>(stair_up, content[16])) return false;
			if (fill_if<bool>(stair_down, content[17])) return false;
			if (fill_if<bool>(stair_side, content[18])) return false;
			if (fill_if<float>(landing_entrance, content[19])) return false;
			if (fill_if<bool>(landing_inside, content[20])) return false;
			if (fill_if<float>(integral_drive, content[21])) return false;

			return true;
		}
	};
	
	struct vertex_init
	{
		static const std::string csv_code;

		std::vector<std::variant<int, float, bool>> content;
		std::vector<TYPE> types =
		{
			INT,
			FLOAT,
			FLOAT
		};

		int prefab_id;
		float x;
		float y;

		bool fill_values()
		{
			if (content.size() != types.size()) return false;

			if (fill_if<int>(prefab_id, content[0])) return false;
			if (fill_if<float>(x, content[1])) return false;
			if (fill_if<float>(y, content[2])) return false;

			return true;
		}
	};

	struct ext_attach_init
	{
		static const std::string csv_code;

		std::vector<std::variant<int, float, bool>> content;
		std::vector<TYPE> types =
		{
			INT,
			INT,
			FLOAT,
			FLOAT,
			FLOAT,
			FLOAT,
			INT,
			FLOAT,
			INT,
			FLOAT,
			INT,
			BOOL
		};

		int prefab_id;
		int point_id;
		float start_x;
		float start_y;
		float end_x;
		float end_y;
		int num_points;
		float radius;
		int default_occ;
		float attachment_time;
		int required_attendants;
		bool is_driver;

		bool fill_values()
		{
			if (content.size() != types.size()) return false;

			if (fill_if<int>(prefab_id, content[0])) return false;
			if (fill_if<int>(point_id, content[1])) return false;
			if (fill_if<float>(start_x, content[2])) return false;
			if (fill_if<float>(start_y, content[3])) return false;
			if (fill_if<float>(end_x, content[4])) return false;
			if (fill_if<float>(end_y, content[5])) return false;
			if (fill_if<int>(num_points, content[6])) return false;
			if (fill_if<float>(radius, content[7])) return false;
			if (fill_if<int>(default_occ, content[8])) return false;
			if (fill_if<float>(attachment_time, content[9])) return false;
			if (fill_if<int>(required_attendants, content[10])) return false;
			if (fill_if<bool>(is_driver, content[11])) return false;

			return true;
		}

	};

	struct int_attach_init
	{
		static const std::string csv_code;

		std::vector<std::variant<int, float, bool>> content;
		std::vector<TYPE> types =
		{
			INT,
			INT,
			FLOAT,
			FLOAT,
			FLOAT,
			FLOAT,
			INT,
			BOOL
		};

		int prefab_id;
		int point_id;
		float x;
		float y;
		float radius;
		float attachment_time;
		int required_attendants;
		bool is_driver;

		bool fill_values()
		{
			if (content.size() != types.size()) return false;

			if (fill_if<int>(prefab_id, content[0])) return false;
			if (fill_if<int>(point_id, content[1])) return false;
			if (fill_if<float>(x, content[2])) return false;
			if (fill_if<float>(y, content[3])) return false;
			if (fill_if<float>(radius, content[4])) return false;
			if (fill_if<float>(attachment_time, content[5])) return false;
			if (fill_if<int>(required_attendants, content[6])) return false;
			if (fill_if<bool>(is_driver, content[7])) return false;

			return true;
		}
	};

	struct options_init
	{
		static const std::string csv_code;

		std::vector<std::variant<int, float, bool>> content;
		std::vector<TYPE> types =
		{
			BOOL,
			BOOL,
			FLOAT,
			INT,
			INT,
			FLOAT,
			FLOAT,
			FLOAT,
			BOOL,
			BOOL,
			BOOL,
			BOOL,
			BOOL,
			BOOL
		};

		bool vo_hybrid;
		bool vo_generalised;
		float time_to_collision;
		int num_runs_per_sim;
		int tcp_PortID;
		float vo_add_dist_cost;
		float vo_l_r_priority;
		float min_time_scale_stopped_ped;
		bool data_sim;
		bool data_adj_matrix;
		bool data_adj_list;
		bool data_cnode_arc_info;
		bool data_tnode_arc_info;
		bool data_object_info;

		bool fill_values()
		{
			if (content.size() != types.size()) return false;

			if (fill_if<bool>(vo_hybrid, content[0])) return false;
			if (fill_if<bool>(vo_generalised, content[1])) return false;
			if (fill_if<float>(time_to_collision, content[2])) return false;
			if (fill_if<int>(num_runs_per_sim, content[3])) return false;
			if (fill_if<int>(tcp_PortID, content[4])) return false;

			if (fill_if<float>(vo_add_dist_cost, content[5])) return false;
			if (fill_if<float>(vo_l_r_priority, content[6])) return false;
			if (fill_if<float>(min_time_scale_stopped_ped, content[7])) return false;

			if (fill_if<bool>(data_sim, content[8])) return false;
			if (fill_if<bool>(data_adj_matrix, content[9])) return false;
			if (fill_if<bool>(data_adj_list, content[10])) return false;
			if (fill_if<bool>(data_cnode_arc_info, content[11])) return false;
			if (fill_if<bool>(data_tnode_arc_info, content[12])) return false;
			if (fill_if<bool>(data_object_info, content[13])) return false;

			return true;
		}
	};

	struct mode_init
	{
		static const std::string csv_code;

		std::vector<std::variant<int, float, bool>> content;
		std::vector<TYPE> types =
		{
			BOOL,
			BOOL,
			BOOL,
			BOOL,
			BOOL
		};

		bool opengl;
		bool tcp;
		bool main_log;
		bool opengl_log;
		bool tcp_log;

		bool fill_values()
		{
			if (content.size() != types.size()) return false;

			if (fill_if<bool>(opengl, content[0])) return false;
			if (fill_if<bool>(tcp, content[1])) return false;
			if (fill_if<bool>(main_log, content[2])) return false;
			if (fill_if<bool>(opengl_log, content[3])) return false;
			if (fill_if<bool>(tcp_log, content[4])) return false;

			return true;
		}
	};

	struct init
	{
		bool failed = true;
		std::vector<prefab_init> prefabs;
		std::vector<vertex_init> vertices;
		std::vector<ext_attach_init> ext_points;
		std::vector<int_attach_init> int_points;
		options_init options;
		mode_init mode;

		std::vector<std::string> pre_prints;
		std::vector<std::string> post_prints;

		std::vector<std::string> errors;
	};

	// get init
	static init get_init();
	static std::vector<std::string> get_blocks(const std::string& line);

	// string to type
	static std::optional<int> string_to_int(std::string*);
	static std::optional<float> string_to_float(std::string*);
	static std::optional<bool> string_to_bool(std::string*);

	// save geo
	static void output_line_to_file(const node*, std::ofstream&);
	static void output_line_to_file(const arc*, std::ofstream&);
	static void output_line_to_file(const boundary_line*, std::ofstream&);
	static void output_line_to_file(const stair*, std::ofstream&);
	static void output_line_to_file(const landing*, std::ofstream&);
	static void output_line_to_file(const char*, std::ofstream&);
	static void output_line_to_file(float, float, float, std::ofstream&);

	static bool save_geometry(const char*, const std::vector<node>&, const std::vector<arc>&, const std::vector<boundary_line>&, const std::vector<stair>&, const std::vector<landing>&, float, float, float);

	// load geo
	static FileIO::Node_construct text_to_node(std::string*);
	static FileIO::Arc_construct text_to_arc(std::string*);
	static FileIO::BoundaryLine_construct text_to_bound(std::string*);
	static FileIO::Stair_construct text_to_stair(std::string*);
	static FileIO::Landing_construct text_to_land(std::string*);
	static void text_to_camera(std::string*, float*, float*, float*);

	static bool read_file(const char*, std::vector<Node_construct>*, std::vector<Arc_construct>*, std::vector<BoundaryLine_construct>*, std::vector<Stair_construct>*, std::vector<Landing_construct>*, float*, float*, float*);

	// Output data
	static bool data_output_node_adj_matrix(const char* filename, const std::vector<int>& nodes, const std::vector<bool>& adj);
	static bool data_output_node_adj_list(const char* filename, const std::map<int, std::vector<int>>& node_adj);
	static bool data_output_cnode_carc_info(const char* filename, const std::vector<CSNode>(&csnodes)[iNUM_ORIENT]);
	static bool data_output_tnode_tarc_info(const char* filename, const std::vector<node>& tnodes, const std::vector<arc>& tarcs);
	static bool data_output_object_info(const char* filename, bool first, int step, int sub_step, int comp_time_ms, float time, const std::vector<GenObject>& objs);
};

