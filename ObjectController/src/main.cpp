#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>
#define _USE_MATH_DEFINES
#include <map>
#include <set>
#include <vector>
#include <math.h>
#include <sstream>
#include <string>
#include <cstdio>
#include <algorithm>

#include "TCP_main_loop.h"
#include "opengl.h"
#include "ObjectSpace.h"
#include "Log.h"
#include "CMD.h"
#include "FileIO.h"

using namespace System;
using namespace System::Threading;

static bool TCP_loop = true;
static bool OpenGL_loop = true;

Log log_main(CONS_WHITE, "MAIN");
Log log_TCP(CONS_RED, "TCP");
Log log_opengl(CONS_GREEN, "OPENGL");
Log log_cmd(CONS_YELLOW, "CMD");

static bool default_obj = true;
static bool main_log = true;
static bool CMD_log = true;
static bool TCP_log = TCP_loop;
static bool OpenGL_log = OpenGL_loop;
static bool CMD_thread = false;

bool get_answer(const char * question, const std::string& t, const std::string& f)
{
	std::cout << "\n" <<  question << " (" << t << "/" << f << ")\n> ";
	std::string ans;
	std::cin >> ans;
	std::cout << "\n";
	if (ans.compare(t) == 0) return true;
	else if (ans.compare(f) == 0) return false;
	else
	{
		std::cout << "please enter \"" << t << "\" or \"" << f << "\"!\n";
		return get_answer(question, t, f);
	}
}

bool get_init()
{
	FileIO::init init = FileIO::get_init();

	if (!init.pre_prints.empty())
	{
		log_main.print("init.csv pre_prints:");
	
		for (std::string& s : init.pre_prints)
		{
			log_main.print(2, s.c_str());
		}
	}

	if (init.failed)
	{
		log_main.print("init failed: init.csv not found");
		return false;
	}

	log_main.print("init started from init.csv");

	//// PREFABS ////
	// params
	default_obj = false;
	for (FileIO::prefab_init& prefab : init.prefabs)
	{
		if (prefab.fill_values())
		{

			float holo[iNUM_HOLO] = {
				prefab.holo0,
				prefab.holo1,
				prefab.holo2,
				prefab.holo3,
				prefab.holo4,
				prefab.holo5,
				prefab.holo6,
				prefab.holo7,
				prefab.holo8
			};

			s_object_space.TCP_add_prefab(object_prefab(prefab.prefab_id, prefab.max_linear_speed, prefab.max_angular_speed, prefab.max_stair_dec_speed, prefab.max_stair_acc_speed, prefab.max_acceleration, holo, prefab.turn_on_spot, prefab.stair_up, prefab.stair_down, prefab.stair_side, prefab.landing_entrance, prefab.landing_inside, prefab.integral_drive));
		}
		else
		{
			log_main.print(2, "prefab not loaded - failed to fill values");
		}
	}

	// vertices
	for (FileIO::vertex_init& vertex : init.vertices)
	{
		if (vertex.fill_values())
		{
			
			s_object_space.TCP_add_prefab_vertex(vertex.prefab_id, vertex.x, vertex.y);
		}
		else
		{
			log_main.print("vertex not loaded - failed to fill values");
		}
	}

	// external attachment points
	for (FileIO::ext_attach_init& point : init.ext_points)
	{
		if (point.fill_values())
		{

			s_object_space.TCP_add_prefab_attach_point(point.prefab_id, point.point_id, { {point.start_x,point.start_y}, {point.end_x,point.end_y} },point.num_points,point.radius,point.default_occ,point.attachment_time,point.required_attendants,point.is_driver);

		}
		else
		{
			log_main.print("external attachment point not loaded - failed to fill values");
		}
	}

	// internal attachment points
	for (FileIO::int_attach_init& point : init.int_points)
	{
		if (point.fill_values())
		{
			s_object_space.TCP_add_prefab_attach_point(point.prefab_id, point.point_id, {point.x,point.y}, point.radius, point.attachment_time, point.required_attendants, point.is_driver);

		}
		else
		{
			log_main.print("internal attachment point not loaded - failed to fill values");
		}
	}

	//// CONFIG ////
	// options
	if (init.options.fill_values())
	{
		s_object_space.vo_hybrid = init.options.vo_hybrid;
		s_object_space.vo_generalised = init.options.vo_generalised;
		s_object_space.vo_time_to_collision = init.options.time_to_collision;
		s_object_space.vo_dist_to_collision = init.options.dist_to_collision;

		s_object_space.num_runs_per_sim = init.options.num_runs_per_sim;
		s_object_space.tcp_PortID = init.options.tcp_PortID;

		s_object_space.vo_add_dist_cost = init.options.vo_add_dist_cost;
		s_object_space.vo_l_r_priority = init.options.vo_l_r_priority;
		s_object_space.min_time_scale_stopped_ped = init.options.min_time_scale_stopped_ped;

		s_object_space.data_sim = init.options.data_sim;
		s_object_space.data_adj_matrix = init.options.data_adj_matrix;
		s_object_space.data_adj_list = init.options.data_adj_list;
		s_object_space.data_cnode_arc_info = init.options.data_cnode_arc_info;
		s_object_space.data_tnode_arc_info = init.options.data_tnode_arc_info;
		s_object_space.data_object_info = init.options.data_object_info;

		s_object_space.vo_des_vel_f = init.options.vo_des_vel_f;

		s_object_space.vo_scale_add = init.options.vo_scale_add;
		s_object_space.vo_scale_mult = init.options.vo_scale_mult;
		s_object_space.pvo_scale_add = init.options.pvo_scale_add;
		s_object_space.pvo_scale_mult = init.options.pvo_scale_mult;

		s_object_space.occnode_when_inactive = init.options.occnode_when_inactive;

		s_object_space.remove_on_target = init.options.remove_on_target;
	}
	else
	{
		log_main.print("options not loaded - failed to fill values");
	}
	
	// mode
	if (init.mode.fill_values())
	{
		TCP_loop = init.mode.tcp;
		OpenGL_loop = init.mode.opengl;

		log_main.on = init.mode.main_log;
		log_TCP.on = init.mode.tcp_log;
		log_TCP.to_file = init.mode.tcp_log_file;
		log_opengl.on = init.mode.opengl_log;
		tcp_log_min = init.mode.tcp_log_min;
	}
	else
	{
		log_main.print("mode not loaded - failed to fill values");
	}

	if (!init.post_prints.empty())
	{
		log_main.print("init.csv post_prints:");

		for (std::string& s : init.post_prints)
		{
			log_main.print(2, s.c_str());
		}
	}

	if (!init.errors.empty())
	{
		log_main.print("init.csv ERRORS:");

		for (std::string& s : init.errors)
		{
			log_main.print(2, s.c_str());
		}
	}

	return true;
}

void add_default_objects()
{
	// exodus's default "stretcher"
	float holonomicity1[iNUM_HOLO] = { 1.0f, 0.875f, 0.75f, 0.625f, 0.5f, 0.625f, 0.75f, 0.875f, 1.0f };
	s_object_space.TCP_add_prefab(object_prefab(1, 1.215f, 79.5208f, 0.535f, 0.25f, 0.81f, holonomicity1, true, true, true, true, 0.7f, true, 2.0f));
	s_object_space.TCP_add_prefab_vertex(1, 0.2f, 0.5f);
	s_object_space.TCP_add_prefab_vertex(1, -0.2f, 0.5f);
	s_object_space.TCP_add_prefab_vertex(1, -0.2f, -0.5f);
	s_object_space.TCP_add_prefab_vertex(1, 0.2f, -0.5f);
	start_end<x_y<float>> line0;
	line0.start.x = -0.2f;
	line0.end.x = 0.2f;
	line0.start.y = 0.69f;
	line0.end.y = 0.69f;
	s_object_space.TCP_add_prefab_attach_point(1, 0, line0, 5, 0.2f, 1.0f, 0, true);
	line0.start.x = -0.2f;
	line0.end.x = 0.2f;
	line0.start.y = -0.69f;
	line0.end.y = -0.69f;
	s_object_space.TCP_add_prefab_attach_point(1, 1, line0, 5, 0.2f, 1.0f, 0, true);
	s_object_space.TCP_add_prefab_attach_point(1, 2, x_y<float>(0.0f, 0.0f), 0.2f, 75.0f, 2, false);

	// evac chair
	float holonomicity2[iNUM_HOLO] = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f };
	s_object_space.TCP_add_prefab(object_prefab(2, 1.48f, 128.245f, 0.825f, 0.0f, 1.0f, holonomicity2, true, false, true, false, 0.35f, true, 2.0f));
	s_object_space.TCP_add_prefab_vertex(2, -0.26f, 0.175f);
	s_object_space.TCP_add_prefab_vertex(2, -0.2f, 0.375f);
	s_object_space.TCP_add_prefab_vertex(2, 0.2f, 0.375f);
	s_object_space.TCP_add_prefab_vertex(2, 0.26f, 0.175f);
	s_object_space.TCP_add_prefab_vertex(2, 0.26f, -0.595f);
	s_object_space.TCP_add_prefab_vertex(2, -0.26f, -0.595f);
	start_end<x_y<float>> line1;
	line1.start.x = -0.26f;
	line1.end.x = 0.26f;
	line1.start.y = -0.795f;
	line1.end.y = -0.795f;
	s_object_space.TCP_add_prefab_attach_point(2, 0, line1, 5, 0.25f, 0.0f, 0, true);
	s_object_space.TCP_add_prefab_attach_point(2, 1, { 0.0f,-0.3f }, 0.2f, 29.0f, 1, false);

	// rescue sheet
	float holonomicity3[iNUM_HOLO] = { 1.0f, 0.35f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.35f };
	s_object_space.TCP_add_prefab(object_prefab(3, 1.125f, 98.2881f, 0.670f, 0.0f, 0.75f, holonomicity3, true, false, true, true, 0.45f, false, 2.0f));
	s_object_space.TCP_add_prefab_vertex(3, -0.25f, 1.32f);
	s_object_space.TCP_add_prefab_vertex(3, 0.25f, 1.32f);
	s_object_space.TCP_add_prefab_vertex(3, 0.375f, 0.5f);
	s_object_space.TCP_add_prefab_vertex(3, 0.375f, 0.0f);
	s_object_space.TCP_add_prefab_vertex(3, 0.25f, -0.5f);
	s_object_space.TCP_add_prefab_vertex(3, -0.25f, -0.5f);
	s_object_space.TCP_add_prefab_vertex(3, -0.375f, 0.0f);
	s_object_space.TCP_add_prefab_vertex(3, -0.375f, 0.5f);
	start_end<x_y<float>> line2;
	line2.start.x = -0.44f;
	line2.end.x = -0.2f;
	line2.start.y = 1.32f;
	line2.end.y = 1.51f;
	start_end<x_y<float>> line3;
	line3.start.x = 0.44f;
	line3.end.x = 0.2f;
	line3.start.y = 1.32f;
	line3.end.y = 1.51f;
	s_object_space.TCP_add_prefab_attach_point(3, 0, line2, 3, 0.2f, 2, 53.0f, 0, true);
	s_object_space.TCP_add_prefab_attach_point(3, 1, line3, 3, 0.2f, 2, 53.0f, 0, true);

	// Stretcher
	float holonomicity4[iNUM_HOLO] = { 1.0f, 0.875f, 0.75f, 0.625f, 0.5f, 0.625f, 0.75f, 0.875f, 1.0f };
	s_object_space.TCP_add_prefab(object_prefab(4, 1.215f, 79.5208f, 0.535f, 0.25f, 0.81f, holonomicity4, true, true, true, true, 0.7f, true, 2.0f));
	s_object_space.TCP_add_prefab_vertex(4, 0.215f, 0.83f);
	s_object_space.TCP_add_prefab_vertex(4, -0.215f, 0.83f);
	s_object_space.TCP_add_prefab_vertex(4, -0.215f, -0.83f);
	s_object_space.TCP_add_prefab_vertex(4, 0.215f, -0.83f);
	start_end<x_y<float>> line4;
	line4.start.x = 0.414f;
	line4.end.x = 0.2f;
	line4.start.y = 0.63f;
	line4.end.y = 1.02f;
	s_object_space.TCP_add_prefab_attach_point(4, 0, line4, 5, 0.2f, 0, 1.0f, 0, true);
	line4.start.x = -0.414f;
	line4.end.x = -0.2f;
	line4.start.y = 0.63f;
	line4.end.y = 1.02f;
	s_object_space.TCP_add_prefab_attach_point(4, 1, line4, 5, 0.2f, 0, 1.0f, 0, true);
	line4.start.x = -0.414f;
	line4.end.x = -0.2f;
	line4.start.y = -0.63f;
	line4.end.y = -1.02f;
	s_object_space.TCP_add_prefab_attach_point(4, 2, line4, 5, 0.2f, 0, 1.0f, 0, true);
	line4.start.x = 0.414f;
	line4.end.x = 0.2f;
	line4.start.y = -0.63f;
	line4.end.y = -1.02f;
	s_object_space.TCP_add_prefab_attach_point(4, 3, line4, 5, 0.2f, 0, 1.0f, 0, true);
	s_object_space.TCP_add_prefab_attach_point(4, 4, x_y<float>(0.0f, 0.0f), 0.2f, 75.0f, 2, false);

	log_main.print("default objects added");
}

void print_version(std::string date, std::string time, std::string platform, std::string configuration)
{
	if (configuration.empty()) configuration = "Release";

	std::cout << std::endl << " ";

	char c = '#';
	int s = (int)date.size() + (int)time.size() + 1;
	int d = s - ((int)platform.size() + (int)configuration.size());

	for (int i = 0; i < s + 28; ++i)
	{
		std::cout << c;
	}
	std::cout << std::endl << " ";

	std::cout << c << c << " Latest build date: " << date << " at " << time << " " << c << c << std::endl << " ";
	std::cout << c << c << " Platform (config): " << platform << " (" << configuration << ") ";
	for (int i = 0; i < d; ++i)
	{
		std::cout << " ";
	}
	std::cout << c << c << std::endl << " ";

	for (int i = 0; i < s + 28; ++i)
	{
		std::cout << c;
	}
	std::cout << std::endl << std::endl;
}

int main(int argc, char* argv[])
{
// Platform and configuration setting
#ifdef _DEBUG
	std::string CONFIGURATION_STR = "Debug";
#else
	std::string CONFIGURATION_STR;
#endif
	
	std::string PLATFORM_STR = "unknown";
#ifdef _WIN64
	PLATFORM_STR = "x64";
#else
	#ifdef _WIN32
		PLATFORM_STR = "x86";
	#endif
#endif

	std::string console_title = "ObjectController (";
	if (!CONFIGURATION_STR.empty())
	{
		console_title.append(CONFIGURATION_STR);
		console_title.append(" ");
	}
	console_title.append(PLATFORM_STR);
	console_title.append(")");

	SetConsoleTitle(console_title.c_str());

	// print version
	print_version(__DATE__, __TIME__, PLATFORM_STR, CONFIGURATION_STR);

	// logs
	log_main.on = main_log;
	log_TCP.on = TCP_log;
	log_opengl.on = OpenGL_log;
	log_cmd.on = CMD_log;

	// setup params (if not forcing defaults)
	bool setup_done = get_init();

	if (!setup_done)
	{
		if (!get_answer("Use default settings?", "y", "n"))
		{
			default_obj = get_answer("[MAIN] Add default objects?", "y", "n");
			OpenGL_loop = get_answer("[OPENGL] Open OpenGL window?", "y", "n");
			TCP_loop = get_answer("[TCP] Start TCP link?", "y", "n");

			while (!TCP_loop && !OpenGL_loop)
			{
				printf("There must be at least an OpenGL window or a TCP link!\n");
				OpenGL_loop = get_answer("[OPENGL] Open OpenGL window?", "y", "n");
				TCP_loop = get_answer("[TCP] Start TCP link?", "y", "n");
			}

			if (!get_answer("[MAIN] logging?", "y", "n"))
			{
				log_main.on = false;
			}
			if (OpenGL_loop)
			{
				if (!get_answer("[OPENGL] logging?", "y", "n"))
				{
					log_opengl.on = false;
				}
			}
			if (TCP_loop)
			{
				if (!get_answer("[TCP] logging?", "y", "n"))
				{
					log_TCP.on = false;
				}
			}
		}
	}

	// default objects
	if (default_obj)
	{
		add_default_objects();
	}

	if (TCP_loop || OpenGL_loop)
	{
		// CMD loop thread
		Thread^ t_CMD_Loop = gcnew Thread(gcnew ThreadStart(cmd_loop));
		if (CMD_thread)
		{
			t_CMD_Loop->Start();
		}
		else
		{
			t_CMD_Loop->Abort();
		}

		// TCP loop thread
		Thread^ t_TCP_Loop = gcnew Thread(gcnew ThreadStart(TCP_main_loop));
		if (TCP_loop && OpenGL_loop)
		{
			t_TCP_Loop->Start();
		}
		else
		{
			t_TCP_Loop->Abort();
		}
		
		// OpenGL loop (this thread)
		if (OpenGL_loop)
		{
			int window_width = 200;
			int window_height = 200;

			std::string gl_title = "ObjectController - OpenGL (";
			if (!CONFIGURATION_STR.empty())
			{
				gl_title.append(CONFIGURATION_STR);
				gl_title.append(" ");
			}
			gl_title.append(PLATFORM_STR); 
			gl_title.append(")");

			if (Initialize_GLUT(argc, argv, window_width, window_height, 3.0f / 4.0f, TCP_loop, gl_title) == 0)
			{
				// resize console
				HWND wh = GetConsoleWindow();
				MoveWindow(wh, -7, 0, window_width + 23, window_height + 39, TRUE);

				log_main.print("setup complete");
				glutMainLoop();
				log_opengl.print("OpenGL window closed!");
			}
			else
			{
				log_opengl.print("Failed to init");
			}
			OpenGL_loop = false;
		} 
		else // if only TCP with no OpenGL - run TCP on thi s thread 
		{
			log_main.print("setup complete");
			TCP_main_loop();
		}

		// Join threads
		if (TCP_loop && OpenGL_loop)
		{
			t_TCP_Loop->Join();
			TCP_loop = false;
		}
		if (CMD_thread)
		{
			t_CMD_Loop->Join();
		}
	}
	else
	{
		log_main.print("No way to interact with application. Ensure at least one of OpenGL and TCP are active");
	}

	log_main.print("Close window to end application");
	while (true) {}
	// end of application
}
