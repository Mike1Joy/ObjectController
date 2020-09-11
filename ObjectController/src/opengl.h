#pragma once

#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>
#define _USE_MATH_DEFINES
#include <vector>
#include <math.h>
#include <sstream>
#include <string>
#include <cstdio>
#include <algorithm>

#include "opengl_const.h"
#include "ObjectSpace.h"

//rainbow
static int rain_colour = RED;
static int rain_start = 0;
static int rain_end = 255;
static int rain_sensitivity = 5;
static int key_int = 10;
static int rain_mode = MIN_TIME;
static float max_pot = 0.0f;

// right click menu
static int menu;
static bool show_nodes = true;
static bool show_arcs = true;
static bool show_bound = true;
static bool show_cells = true;
static bool show_cspace = false;
static bool show_ldcspace = false;
static bool show_sim_data = true;
static bool show_vos = true;
static bool show_poly = true;
static bool show_ap = true;
static bool show_cor = true;

// camera control
static float zoom = 30.0f;
static float camerax = -1.0f;
static float cameray = -2.16f;
static float ratio = 1.0f;
static int window_height = 768;
static int window_width = 1024;
static float scale = zoom / window_width;
static float view_left = camerax;
static float view_right = (camerax + zoom);
static float view_bottom = ratio * cameray;
static float view_top = ratio * (cameray + zoom);
static bool flipped_view = true;

static int floor_num = 1;

// View port
static int mainWindow;

// bottom screen menu and generating cspace + object + potential
static bool cspace_generating = false;
static bool cspace_generated = false;
static bool object_generating = false;
static bool adding_person = false;
static bool object_free_space = false;
static bool object_free_space_generating = false;
static bool potential_generating = false;
static bool potential_generated = false;
static bool simulate = false;
static bool simulate_pause = false;
static bool simulate_step = false;
static bool selecting_goals = false;
static int orient = 0;
static std::vector<int> goals;
static float grad_b = 0.3f;
static float grad_g = 0.5f;
static float grad_r = 0.7f;
static float inf_color = 0.3f;
static int mode = GEOMETRY;
static bool TCP_cont = false;

//grid
static bool show_grid = true;
static int increment = 50; //cm
static int grid_first_x = floor(view_left / (increment / 100.0f));
static int grid_last_x = floor(view_right / (increment / 100.0f));
static int grid_first_y = floor(view_bottom / (increment / 100.0f));
static int grid_last_y = floor(view_top / (increment / 100.0f));

// mouse move
static int deltaX = 0;
static int deltaY = 0;
static int xOrigin = -1;
static int yOrigin = -1;
static float tempcamerax = camerax;
static float tempcameray = cameray;
static bool mouseDown = false;
static int mousex = -1;
static int mousey = -1;

// Edit mode
static char edit_mode = EDIT_NODE;
static struct selection
{
	const int* node;
	const int* arc;
	const int* bound;

	void deselect()
	{
		m_id = 0;
		node = nullptr;
		arc = nullptr;
		bound = nullptr;
	}

	void select_node(const int* id)
	{
		if (id == nullptr)
		{
			deselect();
			return;
		}
		m_id = *id;
		node = &m_id;
		arc = nullptr;
		bound = nullptr;
	}
	void select_arc(const int* id)
	{
		if (id == nullptr)
		{
			deselect();
			return;
		}
		m_id = *id;
		node = nullptr;
		arc = &m_id;
		bound = nullptr;
	}
	void select_bound(const int* id)
	{
		if (id == nullptr)
		{
			deselect();
			return;
		}
		m_id = *id;
		node = nullptr;
		arc = nullptr;
		bound = &m_id;
	}

	operator bool()
	{
		return (node || arc || bound);
	}
	bool equals_node(int id)
	{
		if (!node)
		{
			return false;
		}
		return m_id == id;
	}
	bool equals_arc(int id)
	{
		if (!arc)
		{
			return false;
		}
		return m_id == id;
	}
	bool equals_bound(int id)
	{
		if (!bound)
		{
			return false;
		}
		return m_id == id;
	}

private: int m_id = 0;
};
static selection select;
static bool auto_connect = false;
static bool cleared = false;

// Add mode
static bool add = false;
static bool adding = false;
static float firstx = 0.0f;
static float firsty = 0.0f;
static const node* firstn = nullptr;

//for speed control
static bool speed_control = true;
static bool wait = false;
static float delay_mult = 1.0f;

//for Cspace generation
static Orient orientation = 0;
static int arc_connection = 0;
static int prefab = 1;
static int cur_obj = 1;

// simulation
static data_for_TCP::all_sim_data gl_sim_data;

////////////////////////////////////////////

//rainbow
float get_rainbow(int, int);
float get_rainbow(float prop, int colour);

// right click menu
void processMenuEvents(int);
void createGLUTMenus();

// pix <--> pos
float x_pix_to_pos(int);
float y_pix_to_pos(int);
int x_pos_to_pix(float);
int y_pos_to_pix(float);

// View port
void changeViewPort(int, int);

// Mouse control
void mouseButton(int, int, int, int);
void mouseMove(int, int);

// key press
void processNormalKeys(unsigned char, int, int);
void pressKey(int, int, int);

// add geometry
void add_node(int id, float x_position, float y_position, bool is_landing);
void add_arc(int start_stair, int end_stair, int start_node_id, int end_node_id, float start_x, float start_y, float end_x, float end_y, int, int, int id);
void add_bound(float start_x, float start_y, float end_x, float end_y, int id);
void add_stair(float x, float y);

// add objects
void add_object(int object_id_, int object_prefab_id_, int start_node_id_, int start_floor_num_, int start_orientation_, float start_x, float start_y);
void fill_objects();
void add_person(float xpos, float ypos, int floor);

// Draw (single)
void drawRegPoly(float xpos, float ypos, float zpos, float radius, float edges, float red, float green, float blue);
void drawTestDot(float x, float y);
void drawRegPoly(float xpos, float ypos, float zpos, float radius, float edges, float red, float green, float blue, float alpha);
void drawLine(float layer, float startx, float starty, float endx, float endy, float width, float red, float green, float blue);
void drawIrregPoly(float transx, float transy, float transz, float rot, std::vector<vector2> vertices, float red, float green, float blue, bool outline, bool fill);
void drawFillPoly(const std::vector<x_y<float>>& vertices, float layer, float r, float g, float b, float a);
void drawNode(float xpos, float ypos, bool isSelected, float pot, bool occupied_object, bool occupied_person, bool stair, bool land);
void drawCSNode(float xpos, float ypos, bool valid, int stair, int, float land_dist);
void drawCSNodeSim(float xpos, float ypos, float pot, float max_pot, bool in_edges);
void drawCSArc(float xpos, float ypos, unsigned char stair_type);
void drawArc(float startx, float starty, float endx, float endy, bool isSelected, bool bstair, unsigned char stair_dir);
void drawBoundary(float startx, float starty, float endx, float endy, bool isSelected);
void drawStair(float x_position_, float y_position_, float direction_, int lanes_, int riser_num_, float hlength_, float hwidth_, float height_, bool bottom, start_end<x_y<float>> left_wall, start_end<x_y<float>> right_wall); // half length and half width
void drawLanding(landing& l);
void drawCell(float xsize, float ysize, float x, float y, int rain, bool empty);
void drawRegPoly_nofill(float xpos, float ypos, float zpos, float radius, float edges, float red, float green, float blue, float width);
void drawXYTheta(float x, float y, float theta, float layer, float length, float radius, float r, float g, float b);
void drawBezier(bezier& bez);
void drawVO(const velocity_obstacle& vo, x_y<float> pos, vector2 velocity);

// Draw (vector)
void drawNodeVector();
void drawArc1Vector();
void drawBoundVector();
void drawStairVector();
void drawLandingVector();
void drawCells();
void drawCSNodeVector();
void drawLDCSNodeVector();
void drawSimData();

// Draw Geo
void drawGeo();
void drawCSpace();
void drawLDCSpace();

// Draw Object
void drawPrevConfig(const onion_config& config);
void drawConfig(GenObject& obj);
void drawObj();

// Onion Skinning
static std::vector<onion_config> previous_configs;
static bool save_pre_configs = false;
static bool show_pre_configs = false;
void reset_all_pre_configs();
void save_config(const GenObject& obj, float time, const polygon& poly, const std::vector<attachment_point>& att_points);
float get_prev_config_depth(float prev_time);
static std::map<int, bool> hide_objs;

// Draw grid
void updateGrid();
void drawGridX(int counter);
void drawGridY(int counter);

// Write text
void writeTextScreen(int x_pos, int y_pos, void* font, const char* string, float red, float green, float blue);
void writeTextGeo(float x_pos, float y_pos, void* font, char* string, float red, float green, float blue);
void writeTextGeoc(float x_pos, float y_pos, void* font, char c, float red, float green, float blue);
void write_edit_mode();

//delay 
void delayed(int delay);
static int num_arcs = 0;
static int num_nodes = 0;
static int num_bounds = 0;
static int num_stairs = 0;
static int num_objects = 0;
static int num_people = 0;
void hide_geo_if_changed(int);
void delayed_update(int update_freq_ms);

// RENDER
void render();

// initialise
int Initialize_GLUT(int argc, char* argv[], int&, int&, float, bool TCP_control, std::string title);
