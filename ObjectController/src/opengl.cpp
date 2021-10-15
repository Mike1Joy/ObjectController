#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <sstream>
#include <string>
#include <cstdio>
#include <algorithm>

#include "ObjectSpace.h"
#include "opengl.h"
#include "Log.h"

//rainbow
float get_rainbow(int position, int color)
{
	if (rain_end <= rain_start)
	{
		if (position < (rain_start + rain_end) / 2)
		{
			if (rain_colour == RED)
			{
				return red[256][color];
			}
			else
			{
				return rainbow[256][color];
			}
		}
		else
		{
			if (rain_colour == RED)
			{
				return red[257][color];
			}
			else
			{
				return rainbow[257][color];
			}
		}
	}
	int pos = (position - rain_start) * 255 / (rain_end - rain_start);
	if (pos < 0)
	{
		pos = 256;
	}
	else if (pos > 255)
	{
		pos = 257;
	}
	if (rain_colour == RED)
	{
		return red[pos][color];
	}
	else
	{
		return rainbow[pos][color];
	}
}
float get_rainbow(float prop, int colour)
{
	return get_rainbow((int)(prop * 255), colour);
}

// Right click menu
void processMenuEvents(int option) {
	switch (option) {
	case SHOW_HIDE_ALL:
		if (show_nodes || show_arcs || show_sim_data || show_vos || show_grid)
		{
			log_opengl.print("Cleaning view");
			show_nodes = false;
			show_arcs = false;
			show_sim_data = false;
			show_vos = false;
			show_grid = false;
		}
		else
		{
			log_opengl.print("Showing all");
			show_nodes = true;
			show_arcs = true;
			show_sim_data = true;
			show_vos = true;
			show_grid = true;
		}
		break;
	case SHOW_HIDE_NODES:
		show_nodes = !show_nodes;
		log_opengl.print(show_nodes ? "Showing nodes" : "Nodes hidden");
		break;
	case SHOW_HIDE_ARCS:
		show_arcs = !show_arcs;
		log_opengl.print(show_arcs ? "Showing arcs" : "Arcs hidden");
		break;
	case SHOW_HIDE_BOUND:
		show_bound = !show_bound;
		log_opengl.print(show_bound ? "Showing boundary lines" : "Boundary lines hidden");
		break;
	case SHOW_HIDE_CSPACE:
		show_cspace = !show_cspace;
		max_pot = 0.0f;
		show_ldcspace = false;
		log_opengl.print(show_cspace ? "Showing C-Space" : "Showing T-Space");
		break;
	case SHOW_HIDE_LDCSPACE:
		show_ldcspace = !show_ldcspace;
		max_pot = 0.0f;
		show_cspace = false;
		log_opengl.print(show_ldcspace ? "Showing Lower Dimensional C-Space" : "Showing T-Space");
		break;
	case SHOW_HIDE_SIM_DATA:
		show_sim_data = !show_sim_data;
		log_opengl.print(show_sim_data ? "Showing Sim Data" : "Hiding Sim Data");
		break;
	case SHOW_HIDE_VOS:
		show_vos = !show_vos;
		log_opengl.print(show_vos ? "Showing VOs" : "Hiding VOs");
		break;
	case FLIP_VIEW:
		flipped_view = !flipped_view;
		changeViewPort(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
		log_opengl.print(flipped_view ? "View flipped (origin in top left corner)" : "View normal (origin in bottom left corner)");
		break;
	case SHOW_HIDE_PRE_CONFIGS:
		show_pre_configs = !show_pre_configs;
		log_opengl.print(show_pre_configs ? "Showing Onion Skinning" : "Hiding Onion Skinning");
		break;
	case START_STOP_ONION_SKIN:
		if (save_pre_configs)
		{
			save_pre_configs = false;
			reset_all_pre_configs();
			log_opengl.print("Onion Skinning stopped");
		}
		else
		{
			save_pre_configs = true;
			log_opengl.print("Onion Skinning started");
		}
		break;
	case SHOW_HIDE_POLY:
		show_poly = !show_poly;
		log_opengl.print(show_poly ? "Showing Polygons" : "Hiding Polygons");
		break;
	case SHOW_HIDE_AP:
		show_ap = !show_ap;
		log_opengl.print(show_ap ? "Showing Attachment Points" : "Hiding Attachment Points");
		break;
	case SHOW_HIDE_COR:
		show_cor = !show_cor;
		log_opengl.print(show_cor ? "Showing CoRs" : "Hiding CoRs");
		break;
	case SHOW_HIDE_SEL_OBJ:
		hide_objs[cur_obj];
		hide_objs[cur_obj] = !hide_objs[cur_obj];
		log_opengl.print(hide_objs[cur_obj] ? "Hiding object %d" : "Showing object %d", cur_obj);
		break;
	case SHOW_ALL_OBJ:
		show_poly = true;
		show_ap = true;
		show_cor = true;
		hide_objs.clear();
		log_opengl.print("Showing all objects");
		break;
	}
}
void createGLUTMenus() {
	// create the menu, tell glut that "processMenuEvents" will handle the events
	menu = glutCreateMenu(processMenuEvents);

	//add entries to our menu
	glutAddMenuEntry("View:", -1);
	glutAddMenuEntry("  C-Space", SHOW_HIDE_CSPACE);
	glutAddMenuEntry("  LD C-Space", SHOW_HIDE_LDCSPACE);
	glutAddMenuEntry("  Flip View", FLIP_VIEW);
	glutAddMenuEntry("Show/Hide Geo:", SHOW_HIDE_ALL);
	glutAddMenuEntry("  Nodes", SHOW_HIDE_NODES);
	glutAddMenuEntry("  Arcs", SHOW_HIDE_ARCS);
	glutAddMenuEntry("  Boundry", SHOW_HIDE_BOUND);
	glutAddMenuEntry("Show/Hide Obj:", SHOW_ALL_OBJ);
	glutAddMenuEntry("  Sim Data", SHOW_HIDE_SIM_DATA);
	glutAddMenuEntry("  VOs", SHOW_HIDE_VOS);
	glutAddMenuEntry("  Polygon", SHOW_HIDE_POLY);
	glutAddMenuEntry("  Att. Points", SHOW_HIDE_AP);
	glutAddMenuEntry("  CoR", SHOW_HIDE_COR);
	glutAddMenuEntry("  Selected Object", SHOW_HIDE_SEL_OBJ);
	glutAddMenuEntry("Onion Skin:", -1);
	glutAddMenuEntry("  Start/Stop", START_STOP_ONION_SKIN);
	glutAddMenuEntry("  Show/Hide", SHOW_HIDE_PRE_CONFIGS);

	// attach the menu to the right button
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// pix <--> pos
float x_pix_to_pos(int x_pos)
{
	return camerax + x_pos * scale;
}
float y_pix_to_pos(int y_pos)
{
	if (flipped_view)
	{
		y_pos = glutGet(GLUT_WINDOW_HEIGHT) - y_pos;
	}

	return cameray * ratio + y_pos * scale;
}
int x_pos_to_pix(float x_pos)
{
	return floor((x_pos - camerax) / scale);
}
int y_pos_to_pix(float y_pos)
{
	if (flipped_view)
		return glutGet(GLUT_WINDOW_HEIGHT) - floor(((y_pos) - cameray * ratio) / scale);
	else
		return floor((y_pos - cameray * ratio) / scale);
}

// View port
void changeViewPort(int w, int h)
{
	if (w < 1024)
	{
		w = 1024;
		glutReshapeWindow(w, h);
	}
	if (h < 64)
	{
		h = 64;
		glutReshapeWindow(w, h);
	}

	if (w == 0) w = 1;
	ratio = 1.0* h / w;

	scale = zoom / w;

	view_left = camerax;
	view_right = (camerax + zoom);
	view_bottom = ratio * cameray;
	view_top = ratio * (cameray + zoom);

	updateGrid();

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective
	if (flipped_view)
		glOrtho(view_left, view_right, view_top, view_bottom, 999, -999);
	else
		glOrtho(view_left, view_right, view_bottom, view_top, 999, -999);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

// Mouse control
void mouseButton(int button, int state, int x, int y)
{
	if (flipped_view)
	{
		y = glutGet(GLUT_WINDOW_HEIGHT) - y;
	}

	if (mode == GEOMETRY)
	{
		// only start motion if the left button is pressed
		if (!add)
		{
			if (button == GLUT_LEFT_BUTTON) {
				// when the button is released
				if (state == GLUT_UP) {
					tempcamerax = camerax;
					tempcameray = cameray;
					xOrigin = -1;
					yOrigin = -1;
					mouseDown = false;
				}
				else {// state = GLUT_DOWN
					tempcamerax = camerax;
					tempcameray = cameray;
					xOrigin = x;
					if (flipped_view)
						yOrigin = glutGet(GLUT_WINDOW_HEIGHT) - y;
					else
						yOrigin = y;
					mouseDown = true;

					//entity selection
					switch (edit_mode) {
					case EDIT_NODE:
						select.select_node(s_object_space.select_tnode(x*scale + view_left, view_top - y * scale, 0.2f, select.node, floor_num));
						break;
					case EDIT_ARC:
						select.select_arc(s_object_space.select_tarc(x*scale + view_left, view_top - y * scale, 0.2f, select.arc, floor_num));
						break;
					case EDIT_BOUND:
						select.select_bound(s_object_space.select_bound(x*scale + view_left, view_top - y * scale, 0.2f, select.bound, floor_num));
						break;
					}
				}
			}
		}
		else if (state == GLUT_DOWN)
		{
			float xx = show_grid ? floor((x*scale + view_left) / (increment / 100.0f) + 0.5f)*(increment / 100.0f) : x * scale + view_left;
			float yy = show_grid ? floor((view_top - y * scale) / (increment / 100.0f) + 0.5f)*(increment / 100.0f) : view_top - y * scale;
			int id_ = s_object_space.get_next_node_id();
			switch (edit_mode)
			{
			case EDIT_NODE:
				add_node(id_,xx,yy,false);
				if (auto_connect && show_grid)
				{
					for (int i = -1; i <= 1; ++i)
					{
						for (int j = -1; j <= 1; ++j)
						{
							if (i != 0 || j != 0)
							{
								const node* n = s_object_space.get_tnode(xx + i*0.5f, yy + j*0.5f, 0.01f, floor_num);
								if (n) add_arc(-1, n->stair_id, id_, n->id, xx, yy, n->position.x, n->position.y, floor_num, n->floor_num, s_object_space.get_next_arc_id());
							}
						}
					}
				}
				cspace_generated = false;
				break;
			case EDIT_ARC:
				firstn = s_object_space.get_tnode(x*scale + view_left, view_top - y * scale, 0.2f, floor_num);
				if (firstn)
				{
					mousex = -1;
					mousey = -1;
					adding = true;
					firstx = firstn->position.x;
					firsty = firstn->position.y;
				}
				break;
			case EDIT_BOUND:
				firstx = xx;
				firsty = yy;
				mousex = -1;
				mousey = -1;
				adding = true;
				break;
			case EDIT_STAIR:
				add_stair(xx, yy);
				cspace_generated = false;
				break;
			}
		}
		else if (state == GLUT_UP && mousex != -1 && adding) {
			adding = false;

			switch (edit_mode)
			{
			case EDIT_ARC:
				if (firstn)
				{
					const node* lastn = s_object_space.get_tnode(x*scale + view_left, view_top - y * scale, 0.2f, floor_num);
					if (lastn)
					{
						if (firstn->id != lastn->id)
						{
							add_arc(firstn->stair_id, lastn->stair_id, firstn->id, lastn->id, firstn->position.x, firstn->position.y, lastn->position.x, lastn->position.y, firstn->floor_num, lastn->floor_num, s_object_space.get_next_arc_id());
							cspace_generated = false;
						}
					}
					firstn = nullptr;
				}
				break;
			case EDIT_BOUND:
				float lastx = show_grid ? floor((x*scale + view_left) / (increment / 100.0f) + 0.5f)*(increment / 100.0f) : x * scale + view_left;
				float lasty = show_grid ? floor((view_top - y * scale) / (increment / 100.0f) + 0.5f)*(increment / 100.0f) : view_top - y * scale;
				add_bound(firstx, firsty, lastx, lasty, s_object_space.get_next_bound_id());
				break;
			}
		}
	}
	else if (mode == OBJECT)
	{
		if (!object_generating && !adding_person) {
			if (button == GLUT_LEFT_BUTTON) {
				// when the button is released
				if (state == GLUT_UP) {
					tempcamerax = camerax;
					tempcameray = cameray;
					xOrigin = -1;
					yOrigin = -1;
					mouseDown = false;
				}
				else {// state = GLUT_DOWN
					tempcamerax = camerax;
					tempcameray = cameray;
					xOrigin = x;
					if (flipped_view)
						yOrigin = glutGet(GLUT_WINDOW_HEIGHT) - y;
					else
						yOrigin = y;
					mouseDown = true;
				}
			}
		}
		else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			if (object_generating)
			{
				const node* n = s_object_space.get_tnode(x*scale + view_left, view_top - y * scale, 0.26f, floor_num);
				if (n)
					add_object(s_object_space.get_next_object_id(), prefab, n->id, n->floor_num, (int)orientation, n->position.x, n->position.y);
			}
			else if (adding_person)
			{
				add_person(x*scale + view_left, view_top - y * scale, floor_num);
			}
		}
	}
	else if (mode == SIMULATE)
	{
		if (!potential_generating && !object_generating && !selecting_goals && !adding_person)
		{
			if (button == GLUT_LEFT_BUTTON) {
				// when the button is released
				if (state == GLUT_UP) {
					tempcamerax = camerax;
					tempcameray = cameray;
					xOrigin = -1;
					yOrigin = -1;
					mouseDown = false;
				}
				else {// state = GLUT_DOWN
					tempcamerax = camerax;
					tempcameray = cameray;
					xOrigin = x;
					if (flipped_view)
						yOrigin = glutGet(GLUT_WINDOW_HEIGHT) - y;
					else
						yOrigin = y;
					mouseDown = true;
				}
			}
		}
		else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			if (selecting_goals)
			{
				select.select_node(s_object_space.select_tnode(x*scale + view_left, view_top - y * scale, 0.25f, select.node, floor_num));
				if (select.node)
				{
					log_opengl.print("Goal selected: node = %d", *select.node);
					s_object_space.go_to_target(cur_obj, *select.node);
				}
			}
			else if (adding_person)
			{
				add_person(x * scale + view_left, view_top - y * scale, floor_num);
			}
		}
	}
	else // default
	{
		// pan
		if (button == GLUT_LEFT_BUTTON) {
			// when the button is released
			if (state == GLUT_UP) {
				tempcamerax = camerax;
				tempcameray = cameray;
				xOrigin = -1;
				yOrigin = -1;
				mouseDown = false;
			}
			else {// state = GLUT_DOWN
				tempcamerax = camerax;
				tempcameray = cameray;
				xOrigin = x;
				if (flipped_view)
					yOrigin = glutGet(GLUT_WINDOW_HEIGHT) - y;
				else
					yOrigin = y;
				mouseDown = true;
			}
		}
	}
}
void mouseMove(int x, int y)
{
	if (adding) { mousex = x; mousey = y; }

	// this will only be true when the left button is down
	if (mouseDown) {
		// update deltaAngle
		deltaX = -(x - xOrigin);
		if (flipped_view)
			deltaY = (yOrigin - y);
		else
			deltaY = (y - yOrigin);

		// update camera's direction
		camerax = tempcamerax + deltaX * scale;
		cameray = tempcameray + deltaY * zoom / glutGet(GLUT_WINDOW_HEIGHT);

		changeViewPort(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
	}
}

// key press
void processNormalKeys(unsigned char key, int x, int y)
{
	//for all modes
	switch (key) {
	case '+': zoom /= 1.1f; changeViewPort(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT)); break;
	case '-': zoom *= 1.1f; changeViewPort(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT)); break;
	}

	//for specific mode
	switch (mode) {
	case SIMULATE:
		switch (key) {
		case 'l':
			s_object_space.save_state();
			break;
		case 'L':
			s_object_space.retrieve_state();
			break;
		case 'R':
			s_object_space.remove_object_task(cur_obj);
			break;
		case 'm':
			if (TCP_cont)
			{
				mode = TCP_CONTROL;
				break;
			}
			if (true) {
				mode = GEOMETRY;
				//REDO: cspace->reset_all_potentials();
				//REDO: cspace->reset_all_cells();
				potential_generated = false;
				potential_generating = false;
				selecting_goals = false;
				goals.clear();
				show_grid = true;
				increment = 50;
			}
			break;
		case 'c':
			speed_control = !speed_control;
			break;
		case 'g':
			selecting_goals = !selecting_goals;
			log_opengl.print("selecting_goals = %d", selecting_goals);
			break;
		case 'p':
			simulate = !simulate;
			break;
		case 'P':
			simulate_step = true;
			break;
		case 'O':
			++cur_obj;
			if (cur_obj >= s_object_space.get_next_object_id())
				cur_obj = 0;
			log_opengl.print("Current Object: %d", cur_obj);
			break;
		case 'o':
			orientation += 1;
			if (orientation == 0)
				++arc_connection;
			if (arc_connection == 2)
				arc_connection = -1;
			log_opengl.print("orientation: %d", (int)orientation);
			log_opengl.print("arc_connection: %d", arc_connection);
			break;
		case '2':
			if (rain_start > rain_sensitivity)
			{
				rain_start -= rain_sensitivity;
				rain_end -= rain_sensitivity;
			}
			else
			{
				rain_end -= rain_start;
				rain_start = 0;
			}
			break;
		case '3':
			if (rain_end < 255 - rain_sensitivity)
			{
				rain_start += rain_sensitivity;
				rain_end += rain_sensitivity;
			}
			else
			{
				rain_start += (255 - rain_end);
				rain_end = 255;
			}
			break;
		case '1':
			if (rain_end - rain_start >= rain_sensitivity)
			{
				rain_start += rain_sensitivity;
				rain_end -= rain_sensitivity;
			}
			else
			{
				rain_end = (rain_start + rain_end) / 2.0f;
				rain_start = rain_end;
			}
			break;
		case '4':
			if (rain_end < 255 - rain_sensitivity)
			{
				rain_end += rain_sensitivity;
			}
			else
			{
				rain_end = 255;
			}
			if (rain_start > rain_sensitivity)
			{
				rain_start -= rain_sensitivity;
			}
			else
			{
				rain_start = 0;
			}
			break;
		case '5':
			rain_start = 0;
			rain_end = 255;
			//printf(" start: %d\n end: %d\n\n", rain_start, rain_end);
			break;
		case '7':
			if (key_int < 100)
				++key_int;
			break;
		case '6':
			if (key_int > 1)
				--key_int;
			break;
		case '0':
			++rain_mode;
			if (rain_mode == 7)
				rain_mode = MIN_TIME;
			break;
		case 'r':
			++rain_colour;
			if (rain_colour == 2)
				rain_colour = 0;
			break;
		case 's':
			adding_person = !adding_person;
			adding_person ? log_opengl.print("adding people...") : log_opengl.print("stopped adding people...");
			object_generating = false;
			break;
		}
		break;
	case OBJECT:
		switch (key) {
		case 'm':
			if (TCP_cont)
			{
				mode = SIMULATE;
				break;
			}
			if (object_free_space) {
				mode = SIMULATE;
				adding_person = false;
			}
			break;
		case 's':
			adding_person = !adding_person;
			adding_person ? log_opengl.print("adding people...") : log_opengl.print("stopped adding people...");
			object_generating = false;
			break;
		case 'o':
			if (!object_generating)
			{
				object_generating = true;
				adding_person = false;
			}
			else
			{
				orientation += 1;
				if (orientation == 0)
					++arc_connection;
				if (arc_connection == 2)
					arc_connection = -1;
				log_opengl.print("orientation: %d", (int)orientation);
				log_opengl.print("arc_connection: %d", arc_connection);
			}
			break;
		case 'p':
			prefab++;
			if (prefab == s_object_space.get_next_prefab_id())
				prefab = 1;
			log_opengl.print("prefab: %d", prefab);
			break;
		case 'f':
			if (!object_generating)
			{
				object_free_space_generating = true;
			}
			break;
		case 'F':
			fill_objects();
			break;
		}
		break;
	case GEOMETRY:
		switch (key) {
		case 'd' :
			if (select.node)
			{
				log_opengl.print("selected node %d", *select.node);
				s_object_space.print_details_tnode(*select.node);
			}
			else if (select.arc)
			{
				log_opengl.print("selected arc %d", *select.arc);
				s_object_space.print_details_tarc(*select.arc);
			}
			else if (select.bound)
			{
				log_opengl.print("selected bound %d", *select.bound);
				s_object_space.print_details_bound(*select.bound);
			}
			else
			{
				log_opengl.print("cannot show details: nothing selected");
			}
			break;
		case 'a':
			select.deselect();
			auto_connect = false;
			add = !add;
			adding = false;
			break;
		case 'A':
			autoconnect_nodes();
			break;
		case 'g':
			if (increment == 10)
			{
				auto_connect = false;
				show_grid = false;
				increment = 1000;
			}
			else if (!show_grid)
			{
				show_grid = true;
				increment = 100;
			}
			else if (increment == 100)
			{
				show_grid = true;
				increment = 50;
			}
			else if (increment == 50)
			{
				auto_connect = false;
				increment = 25;
			}
			else if (increment == 25)
			{
				auto_connect = false;
				increment = 10;
			}
			changeViewPort(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
			break;
		case '1':
			if (edit_mode == EDIT_NODE && add) { auto_connect = !auto_connect; }
			else { auto_connect = false; edit_mode = EDIT_NODE; select.deselect(); adding = false; }
			break;
		case '2':
			edit_mode = EDIT_ARC; select.deselect(); adding = false; auto_connect = false; break;
		case '3':
			edit_mode = EDIT_BOUND; select.deselect(); adding = false; auto_connect = false; break;
		case '4':
			edit_mode = EDIT_STAIR; select.deselect(); adding = false; auto_connect = false; break;
		case 127: //delete key
			if (!add)
			{
				switch (edit_mode) {
				case EDIT_NODE:
					if (select.node)
					{
						s_object_space.TCP_remove_node(select.node);
						cspace_generated = false;
						select.deselect();
					}
					break;
				case EDIT_ARC:
					if (select.arc)
					{
						s_object_space.TCP_remove_arc(select.arc);
						cspace_generated = false;
						select.deselect();
					}
					break;
				case EDIT_BOUND:
					if (select.bound)
					{
						s_object_space.TCP_remove_bound(select.bound);
						cspace_generated = false;
						select.deselect();
					}
					break;
				}
			}
			break;
			//test cspace
		case 'c':
			cspace_generating = true;
			break;
		case 'm':
			if (TCP_cont)
			{
				mode = OBJECT;
				break;
			}
			if (!cspace_generated || cspace_generating) break;
			auto_connect = false;
			select.deselect();
			mode = OBJECT;
			show_arcs = false;
			show_grid = false;
			increment = 60;
			break;
		case 's':
			s_object_space.save("geo.csv", zoom, camerax, cameray);
			break;
		case 'l':
			s_object_space.load("geo.csv", &zoom, &camerax, &cameray);
			changeViewPort(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
			break;
		case 'r':
			cleared = true;
			s_object_space.TCP_clear_geometry_vec();
			camerax = -1.0f;
			cameray = -2.16f;
			changeViewPort(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
			break;
		case 'o':
			orientation += 1;
			if (orientation == 0)
				++arc_connection;
			if (arc_connection == 2)
				arc_connection = -1;
			log_opengl.print("orientation: %d", (int)orientation);
			log_opengl.print("arc_connection: %d", arc_connection);
			break;
		case 'p':
			prefab++;
			if (prefab >= s_object_space.get_next_prefab_id())
				prefab = 0;
			log_opengl.print("prefab: %d", prefab);
			break;
		}
		break;
	default:
		switch (key) {
		case 'g':
			if (increment == 10)
			{
				auto_connect = false;
				show_grid = false;
				increment = 1000;
			}
			else if (!show_grid)
			{
				show_grid = true;
				increment = 100;
			}
			else if (increment == 100)
			{
				show_grid = true;
				increment = 50;
			}
			else if (increment == 50)
			{
				auto_connect = false;
				increment = 25;
			}
			else if (increment == 25)
			{
				auto_connect = false;
				increment = 10;
			}
			changeViewPort(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
			break;
		case 'm':
			mode = GEOMETRY;
			break;
		}
		break;
	}

}
void pressKey(int key, int xx, int yy) {
	// reposition window
	switch (key) {
	case GLUT_KEY_LEFT:
		camerax -= 0.01*zoom;
		break;
	case GLUT_KEY_RIGHT:
		camerax += 0.01*zoom;
		break;
	case GLUT_KEY_UP:
		cameray += 0.01*zoom;
		break;
	case GLUT_KEY_DOWN:
		cameray -= 0.01*zoom;
		break;
	case GLUT_KEY_PAGE_UP:
		++floor_num;
		log_opengl.print("Floor: %d", floor_num);
		break;
	case GLUT_KEY_PAGE_DOWN:
		--floor_num;
		log_opengl.print("Floor: %d", floor_num);
		break;
	}
	changeViewPort(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}

// add geometry
void add_node(int id, float x_position, float y_position, bool is_landing)
{
	s_object_space.TCP_add_node(node(id, x_position, y_position, -1, -1, floor_num));
	++num_nodes;
}
void autoconnect_nodes()
{
	num_arcs += s_object_space.GL_autoconnect_nodes(0.5f);
}
void add_arc(int start_stair, int end_stair, int start_node_id, int end_node_id, float start_x, float start_y, float end_x, float end_y, int start_floor, int end_floor, int id)
{
	s_object_space.TCP_add_arc(arc(start_stair, end_stair, start_node_id, end_node_id, start_x, start_y, end_x, end_y, start_floor, end_floor, id));
	++num_arcs;
}
void add_bound(float start_x, float start_y, float end_x, float end_y, int id)
{
	s_object_space.TCP_add_bound(boundary_line(start_x, start_y, end_x, end_y, floor_num, id));
	++num_bounds;
}
void add_stair(float x, float y)
{
	float direction = fLAYER_GAP * (int)orientation;
	s_object_space.TCP_add_stair(stair(s_object_space.get_next_stair_id(), x, y, direction, 3, 15, 3.6f, 1.4f, 3.0f, floor_num), true);
	++num_stairs;

#if 1
	// auto add landings between stairs 1 & 2, 3 & 4, 5 & 6, ...
	if (num_stairs % 2 == 0)
	{
		int id = s_object_space.get_next_landing_id();
		stair* s1 = s_object_space.get_stairpt_from_id(num_stairs - 2);
		stair* s2 = s_object_space.get_stairpt_from_id(num_stairs - 1);
		if (!s1 || !s2) return;

		landing land(id, s1, s2, 1.4f);

		s_object_space.TCP_add_landing(land);

		s_object_space.auto_all_landing_nodes();
	}
#endif
}

// add objects
void add_object(int object_id_, int object_prefab_id_, int start_node_id_, int start_floor_num_, int start_orientation_, float start_x, float start_y)
{
	object_generating = false;

	s_object_space.TCP_add_object(object_instance(object_id_, object_prefab_id_, start_node_id_, start_floor_num_, start_orientation_, start_x, start_y));

	num_objects++;
}
void fill_objects()
{
	s_object_space.GL_fill_objects_people(false);
}
void add_person(float xpos, float ypos, int floor)
{
	const node* n = s_object_space.get_tnode(xpos, ypos, 0.2f, floor_num);
	if (n)
	{
		if (n->occupied_by_person)
		{
			log_opengl.print("remove person");
			if (!n->occupier_ids.empty())
				s_object_space.TCP_remove_person(*n->occupier_ids.begin());
		}
		else
		{
			log_opengl.print("add person");
			s_object_space.TCP_add_person(n->id);
		}
		
	}
	else
	{
		log_opengl.print("person not added: tnode not found");
	}
}

// Draw (single)
void drawRegPoly(float xpos, float ypos, float zpos, float radius, float edges, float red, float green, float blue)
{
	//draw
	glColor3f(red, green, blue);
	glTranslatef(xpos, ypos, zpos);

	glBegin(GL_POLYGON);
	for (double i = 0; i < 2 * M_PI; i += M_PI / edges)
	{
		glVertex3f(cos(i) * radius, sin(i) * radius, 0.0);
	}
	glEnd();

	//reset
	glColor3f(1.0f, 1.0f, 1.0f);
	glTranslatef(-xpos, -ypos, -zpos);
}
void drawTestDot(float x, float y)
{
	drawRegPoly(x, y,-9.0f, 0.1f, 8.0f, 0.0f, 0.0f, 0.0f);
}
void drawRegPoly(float xpos, float ypos, float zpos, float radius, float edges, float red, float green, float blue, float alpha)
{
	//draw
	glColor4f(red, green, blue, alpha);
	glTranslatef(xpos, ypos, zpos);

	glBegin(GL_POLYGON);
	for (double i = 0; i < 2 * M_PI; i += M_PI / edges)
	{
		glVertex3f(cos(i) * radius, sin(i) * radius, 0.0);
	}
	glEnd();

	//reset
	glColor3f(1.0f, 1.0f, 1.0f);
	glTranslatef(-xpos, -ypos, -zpos);
}
void drawLine(float layer, float startx, float starty, float endx, float endy, float width, float red, float green, float blue)
{
	glColor3f(red, green, blue);
	glLineWidth(width);
	glBegin(GL_LINES);
	glVertex3f(startx, starty, layer);
	glVertex3f(endx, endy, layer);
	glEnd();

	glColor3f(1.0f, 1.0f, 1.0f);
}
void drawIrregPoly(float transx, float transy, float transz, float rot, std::vector<vector2> vertices, float red, float green, float blue, bool outline, bool fill)
{
	if (!fill && !outline) return;

	glTranslatef(transx, transy, transz);
	glRotatef(rot, 0, 0, 1);

	if (fill)
	{
		glColor3f(red, green, blue);
		glBegin(GL_POLYGON);
		for (vector2& v : vertices)
		{
			glVertex3f(v.x, v.y, 0.0f);
		}
		glEnd();
		glColor3f(1.0f, 1.0f, 1.0f);
	}

	if (outline)
	{
		glColor3f(red, green, blue);
		for (int i = 0; i < vertices.size(); ++i)
		{
			int j = (i + 1 == vertices.size()) ? 0 : i + 1;
			drawLine(0.0f, vertices[i].x, vertices[i].y, vertices[j].x, vertices[j].y, 2.0f, 0.0f, 0.0f, 0.0f);
		}
		glColor3f(1.0f, 1.0f, 1.0f);
	}

	glRotatef(-rot, 0, 0, 1);
	glTranslatef(-transx, -transy, -transz);
}
void drawFillPoly(const std::vector<x_y<float>>& vertices, float layer, float r, float g, float b, float a)
{
	if (vertices.size() < 3) return;

	//draw
	glColor4f(r, g, b, a);

	glBegin(GL_POLYGON);
	for (const auto& v : vertices)
	{
		glVertex3f(v.x, v.y, layer);
	}
	glEnd();

	//reset
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}
void drawNode(float xpos, float ypos, bool isSelected, float pot, bool occupied_object, bool occupied_person, bool stair, bool land)
{
	if (show_nodes)
	{
		float radius = stair ? 0.08f : 0.2f;
		float layer = stair ? STAIRS_BOT - 0.01f : NODES;
		float r = land ? 0.7f : 0.0f;
		float g = land ? 0.0f : 0.7f;

		if (isSelected && edit_mode == EDIT_NODE)
		{
			drawRegPoly(xpos, ypos, layer, radius, 8, 0.7, 0.1, 0.1);
		}
		else if (occupied_person)
		{
			drawRegPoly(xpos, ypos, layer, radius, 8, 0.0f, 0.0f, 0.7f);
		}
		else if (occupied_object)
		{
			drawRegPoly(xpos, ypos, layer, radius, 8, 0.7f, 0.7f, 0.7f);
		}
		else
		{
			drawRegPoly(xpos, ypos, layer, radius, 8, r, g, 0.0f);
		}
	}
}
void drawCSNode(float xpos, float ypos, bool valid, int stair, int stair_edge, float land_dist)
{
	float rad = (stair == -1) ? ((stair_edge == -1) ? 0.1f : 0.08f) : 0.06f;

	float r = 0.0f;
	float g = 0.0f;
	float b = 0.0f;

	if (valid)
	{
		if (land_dist == -1.0f)
			g = 0.7f;
		else
		{
			r = 1.0f;
			g = land_dist;
			b = g;
		}
	}

	drawRegPoly(xpos, ypos, NODES, rad, 8, r, g, b);
}
void drawCSNodeSim(float xpos, float ypos, float pot, float max_pot, bool in_edges)
{
	if (pot == -1.0f)
	{
		drawRegPoly(xpos, ypos, NODES, 0.1, 8, 0.5f, 0.5f, 0.5f);
	}
	else if (in_edges || pot == -2.0f)
	{
		drawRegPoly(xpos, ypos, NODES, 0.1, 8, 0.0f, 0.0f, 0.0f);
	}
	else
	{
		int pos = (pot / max_pot) * 255;
		drawRegPoly(xpos, ypos, NODES, 0.1, 8, get_rainbow(pos, 0), get_rainbow(pos, 1), get_rainbow(pos, 2));
	}
}
void drawCSArc(float xpos, float ypos, unsigned char stair_dir)
{
	float green = stair_dir == STAIR_ARC_DOWN || stair_dir == STAIR_ARC_UP || stair_dir == STAIR_ARC_SIDEDOWN || stair_dir == STAIR_ARC_SIDEUP ? 0.8f : 0.0f;
	float blue = stair_dir == STAIR_ARC_SIDE || stair_dir == STAIR_ARC_SIDEDOWN || stair_dir == STAIR_ARC_SIDEUP ? 0.8f : 0.0f;
	drawRegPoly(xpos, ypos, NODES - 0.1, 0.05, 8, 0.0f, green, blue);
}
void drawArc(float startx, float starty, float endx, float endy, bool isSelected, bool bstair, unsigned char stair_dir)
{
	float width = bstair ? 1.5f : 2.0f;
	float layer = bstair ? STAIRS_TOP + 0.02 : ARCS;
	float green = stair_dir == STAIR_ARC_DOWN || stair_dir == STAIR_ARC_UP || stair_dir == STAIR_ARC_SIDEDOWN || stair_dir == STAIR_ARC_SIDEUP ? 0.8f : 0.0f;
	float blue = stair_dir == STAIR_ARC_SIDE || stair_dir == STAIR_ARC_SIDEDOWN || stair_dir == STAIR_ARC_SIDEUP ? 0.8f : 0.0f;

	if (isSelected && edit_mode == EDIT_ARC)
	{
		drawLine(layer, startx, starty, endx, endy, width, 0.9f, 0.0f, blue);
	}
	else
	{
		drawLine(layer, startx, starty, endx, endy, width, 0.0f, green, blue);
	}
}
void drawBoundary(float startx, float starty, float endx, float endy, bool isSelected)
{
	if (isSelected && edit_mode == EDIT_BOUND) { drawLine(BOUNDLINES, startx, starty, endx, endy, 7.5f, 0.9f, 0.0f, 0.0f); }
	else { drawLine(BOUNDLINES, startx, starty, endx, endy, 7.5f, 0.0f, 0.0f, 0.0f); }
}
void drawStair(float x_position_, float y_position_, float direction_, int lanes_, int riser_num_, float hlength_, float hwidth_, float height_, bool bottom, start_end<x_y<float>> left_wall, start_end<x_y<float>> right_wall)
{
	float z = bottom ? STAIRS_BOT : STAIRS_TOP;
	glTranslatef(0.0f, 0.0f, z);
	drawLine(-0.1f, left_wall.start.x, left_wall.start.y, left_wall.end.x, left_wall.end.y, 2.0f, 0.0f, 0.0f, 0.0f);
	drawLine(-0.1f, right_wall.start.x, right_wall.start.y, right_wall.end.x, right_wall.end.y, 2.0f, 0.0f, 0.0f, 0.0f);

	glTranslatef(x_position_, y_position_, 0.0f);
	glRotatef(direction_, 0, 0, 1);

	// fill
	glBegin(GL_POLYGON);
		glColor4f(0.5f, 0.5f, 0.5f, bottom ? 0.75f : 0.0f); glVertex3f(-hwidth_ , -hlength_, 0.0f); // top left
		glColor4f(0.5f, 0.5f, 0.5f, bottom ? 0.0f : 0.75f); glVertex3f(-hwidth_ ,  hlength_, 0.0f); // bottom left
		glColor4f(0.5f, 0.5f, 0.5f, bottom ? 0.0f : 0.75f); glVertex3f( hwidth_ ,  hlength_, 0.0f); // bottom right
		glColor4f(0.5f, 0.5f, 0.5f, bottom ? 0.75f : 0.0f); glVertex3f( hwidth_ , -hlength_, 0.0f); // top right
	glEnd();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	// outline
	//drawLine(-0.1f, -hwidth_ , -hlength_, -hwidth_ ,  hlength_, 2.0f, 0.0f, 0.0f, 0.0f); // left
	drawLine(-0.1f, -hwidth_ ,  hlength_,  hwidth_ ,  hlength_, 1.0f, 0.0f, 0.0f, 0.0f); // bottom
	//drawLine(-0.1f,  hwidth_ ,  hlength_,  hwidth_ , -hlength_, 2.0f, 0.0f, 0.0f, 0.0f); // right
	drawLine(-0.1f,  hwidth_ , -hlength_, -hwidth_ , -hlength_, 7.0f, 0.0f, 0.9f, 0.0f); // top

	// lanes
	float lspacing = hwidth_*2.0f / lanes_;
	for (int i = 1; i < lanes_; ++i)
	{
		drawLine(-0.2f, i*lspacing - hwidth_ , -hlength_, i*lspacing - hwidth_ , hlength_, 1.0f, 0.0f, 0.0f, 0.0f);
	}

	// risers
	float rspacing = hlength_*2.0f / (riser_num_-1);
	for (int i = 1; i < riser_num_; ++i)
	{
		drawLine(-0.2f, -hwidth_ , i*rspacing - hlength_, hwidth_ , i*rspacing - hlength_, 1.0f, 0.0f, 0.0f, 0.0f);
	}

	glColor3f(1.0f, 1.0f, 1.0f);
	glRotatef(-direction_, 0, 0, 1);
	glTranslatef(-x_position_, -y_position_, -z);

}
void drawLanding(landing& l)
{
	auto draw_landing_line = [](start_end<x_y<float>> line)
	{
		drawLine(BOUNDLINES, line.start.x, line.start.y, line.end.x, line.end.y, 2.0f, 1.0f, 0.0f, 0.0f);
	};

	draw_landing_line(l.opposite_wall);
	draw_landing_line(l.down_stair_entrance);
	draw_landing_line(l.up_stair_entrance);
	draw_landing_line({ l.up_stair_entrance.end,  l.up_stair_entrance.end + l.up_normal });
	draw_landing_line({ l.down_stair_entrance.end,  l.down_stair_entrance.end + l.down_normal });

	if (l.prefab_bezier.find(prefab) != l.prefab_bezier.end())
	{
		drawBezier(l.prefab_bezier[prefab]);
	}
}
void drawCell(float xsize, float ysize, float x, float y, int rain, bool empty)
{
	bool draw = false;

	float layer = 0.0f;

	if (rain >= 0 && rain < 256)
	{
		glColor3f(get_rainbow(rain, 0), get_rainbow(rain, 1), get_rainbow(rain, 2));
		draw = true;
		layer = CELLS_2;
	}

	if (draw)
	{
		glTranslatef(x, y, layer);

		glBegin(GL_POLYGON);
		glVertex3f(-xsize, -ysize, 0.0);
		glVertex3f(-xsize, ysize, 0.0);
		glVertex3f(xsize, ysize, 0.0);
		glVertex3f(xsize, -ysize, 0.0);
		glEnd();

		glTranslatef(-x, -y, -layer);

		glColor3f(1.0f, 1.0f, 1.0f);
	}
}
void drawRegPoly_nofill(float xpos, float ypos, float zpos, float radius, float edges, float red, float green, float blue, float width)
{
	//draw
	glTranslatef(xpos, ypos, zpos);

	if (radius < 0.1f)
	{
		radius = 0.1f;
	}

	x_y<float> vec1;
	x_y<float> vec2(radius, 0.0f);
	for (float i = 0; i < 2 * M_PI; i += M_PI / edges)
	{
		vec1 = vec2;
		vec2 = x_y<float>(cos(i + M_PI / edges) * radius, sin(i + M_PI / edges) * radius);
		drawLine(zpos, vec1.x, vec1.y, vec2.x, vec2.y, width, red, green, blue);
	}
	drawLine(zpos, vec2.x, vec2.y, radius, 0.0f, width, red, green, blue);
	//reset
	glTranslatef(-xpos, -ypos, -zpos);
}
void drawXYTheta(float x, float y, float theta, float layer, float length, float radius, float r, float g, float b)
{
	// x,y
	drawRegPoly(x, y, layer, radius, 8, r, g, b);

	if (length == 0.0f) return;

	// theta
	float next_x = x + (length * sin(DEG2RAD * theta));
	float next_y = y + (length * cos(DEG2RAD * theta));
	drawLine(layer, x, y, next_x, next_y, 2.0f, r, g, b);
}
void drawBezier(bezier& bez)
{
	drawXYTheta(bez.u0.x, bez.u0.y, 0.0f, TOP_LAYER, 0.0f, 0.05, 1.0f, 0.0f, 0.0f);
	drawXYTheta(bez.u1.x, bez.u1.y, 0.0f, TOP_LAYER, 0.0f, 0.05, 0.75f, 0.25f, 0.0f);
	drawXYTheta(bez.m.x , bez.m.y , 0.0f, TOP_LAYER, 0.0f, 0.05, 0.5f, 0.5f, 0.0f);
	drawXYTheta(bez.d1.x, bez.d1.y, 0.0f, TOP_LAYER, 0.0f, 0.05, 0.25f, 0.75f, 0.0f);
	drawXYTheta(bez.d0.x, bez.d0.y, 0.0f, TOP_LAYER, 0.0f, 0.05, 0.0f, 1.0f, 0.0f);
	
	for (config& c : bez.curve_up)
	{
		drawXYTheta(c.x, c.y, c.theta, TOP_LAYER - 0.01, 0.05f, 0.02, 0.75f, 0.0f, 0.0f);
	}
	for (config& c : bez.curve_down)
	{
		drawXYTheta(c.x, c.y, c.theta, TOP_LAYER - 0.01, 0.05f, 0.02, 0.0f, 0.75f, 0.0f);
	}
}
void drawVO(const velocity_obstacle& vo, x_y<float> pos, vector2 velocity, vector2 des_vel)
{
	glTranslatef(pos.x, pos.y, 0.0f);
		// vo left line (blue)
		drawLine(TOP_LAYER, vo.left_line.start.x, vo.left_line.start.y, vo.left_line.end.x, vo.left_line.end.y, 2.0f, 0.0f, 0.0f, 1.0f); 
		drawRegPoly(vo.left_line.start.x, vo.left_line.start.y, TOP_LAYER, 0.1f, 8, 0.0f, 0.0f, 1.0f);
		// vo right line (red)
		drawLine(TOP_LAYER, vo.right_line.start.x, vo.right_line.start.y, vo.right_line.end.x, vo.right_line.end.y, 2.0f, 1.0f, 0.0f, 0.0f); 
		drawRegPoly(vo.right_line.start.x, vo.right_line.start.y, TOP_LAYER, 0.1f, 8, 1.0f, 0.0f, 0.0f);
		// min velocity to not care
		drawRegPoly_nofill(0.0f, 0.0f, TOP_LAYER, sqrt(vo.ok_mag_sq), 16, 0.5f, 0.5f, 0.5f, 2.0f); 
		// velocity (red if in vo, black if not)
		drawLine(TOP_LAYER, 0.0f, 0.0f, velocity.x, velocity.y, 2.0f, vo.velocity_in_vo(velocity) ? 1.0f : 0.0f, 0.0f, 0.0f);
		// desired velocity (blue)
		drawLine(TOP_LAYER, 0.0f, 0.0f, des_vel.x, des_vel.y, 2.0f, 0.0f, 0.0f, 1.0f);
	glTranslatef(-pos.x, -pos.y, 0.0f);
}

// Draw (vector)
void drawNodeVector()
{
	for (int i = 0; i < s_object_space.get_num_tnodes(); ++i)
	{
		node n = s_object_space.get_tnode(i);
		if (n.floor_num == floor_num || (n.stair_id != -1 && n.floor_num + 1 == floor_num))
			drawNode(n.position.x, n.position.y, select.equals_node(n.id), 0.0f, !n.occupier_ids.empty(), n.occupied_by_person, n.stair_id != -1, n.landing_id != -1);
	}
}
void drawArc1Vector()
{
	for (int i = 0; i < s_object_space.get_num_tarcs(); ++i)
	{
		arc a = s_object_space.get_tarc(i);
		if (
			(a.floor_num.start == floor_num && a.floor_num.end == floor_num)
			|| ((a.stair_ids.start != -1 && a.stair_ids.end != -1) && a.floor_num.start + 1 == floor_num)
			|| (!a.stair_ids.start != -1 && a.floor_num.start == floor_num)
			|| (!a.stair_ids.end != -1 && a.floor_num.end == floor_num)
			)
		{
			drawArc(a.position.start.x, a.position.start.y, a.position.end.x, a.position.end.y, select.equals_arc(a.id), a.stair_ids.start != -1 || a.stair_ids.end != -1, NOT_STAIR_ARC);
		}
	}
}
void drawBoundVector()
{
	for (int i = 0; i < s_object_space.get_num_bounds(); ++i)
	{
		boundary_line b = s_object_space.get_bound_line(i);
		if (b.floor_num == floor_num)
			drawBoundary(b.position.start.x, b.position.start.y, b.position.end.x, b.position.end.y, select.equals_bound(b.id));
	}
}
void drawStairVector()
{
	for (int i = 0; i < s_object_space.get_num_stairs(); ++i)
	{
		stair s = s_object_space.get_stair(i);
		if (s.floor_num == floor_num || s.floor_num + 1 == floor_num)
		{
			drawStair(s.position.x, s.position.y, s.direction, s.lanes, s.riser_num, s.length / 2.0f, s.width / 2.0f, s.height, s.floor_num == floor_num, s.left_wall, s.right_wall);
		}
	}
}
void drawLandingVector()
{
	for (int i = 0; i < s_object_space.get_num_landings(); ++i)
	{
		landing l = s_object_space.get_landing(i);
		if (l.floor == floor_num)
		{
			drawLanding(l);
		}
	}
}
void drawCells()
{
	/*REDO:
	float xsize = (cspace->get_cellSize().first) / 2.0f;
	float ysize = (cspace->get_cellSize().second) / 2.0f;
	float firstx = cspace->get_firstCellPos().first;
	float firsty = cspace->get_firstCellPos().second;

	cell_data* max_data = cspace->get_max_values();

	pair<int, int> numcells = cspace->get_numCells();
	for (int x = 0; x < numcells.first; ++x)
	{
	for (int y = 0; y < numcells.second; ++y)
	{
	Cell* cell = cspace->get_cell(x, y);
	if (!cell->isEmpty())
	{
	cell_data* data = cell->getCellData(display_haz);
	switch (rain_mode)
	{
	case MIN_TIME:
	drawCell(xsize, ysize, x*xsize * 2 + firstx, y*ysize * 2 + firsty, (data->min_time / max_data->min_time) * 255, cell->isEmpty());
	break;
	case AVG_TIME:
	drawCell(xsize, ysize, x*xsize * 2 + firstx, y*ysize * 2 + firsty, (data->avg_time / max_data->avg_time) * 255, cell->isEmpty());
	break;
	}
	}
	}
	}
	*/
}
void drawCSNodeVector()
{
	for (std::vector<CSNode>::iterator it_n = s_object_space.nodes_begin(orientation); it_n != s_object_space.nodes_end(orientation); ++it_n)
	{
		if (it_n->get_floor_num() == floor_num)
		{
			x_y<float> pos = it_n->get_position();

			if (pos.x > view_left - 0.1f && pos.x < view_right + 0.1f && pos.y > view_bottom - 0.1f && pos.y < view_top + 0.1f)
			{
				if (show_nodes)
				{
					if (mode == SIMULATE)
					{
						float pot = it_n->_potentials[cur_obj];
						if (pot > max_pot) max_pot = pot;
						drawCSNodeSim(pos.x, pos.y, pot, max_pot, !it_n->_potential_fixed[cur_obj]);
					}
					else
					{
						if (it_n->_valid.empty())
						{
							drawCSNode(pos.x, pos.y, false, it_n->_stair_id, it_n->_stair_ids[prefab], it_n->landing_id == -1 ? -1 : it_n->_landing_dist_curve[prefab]);
						}
						else
						{
							drawCSNode(pos.x, pos.y, it_n->_valid.at(prefab), it_n->_stair_id, it_n->_stair_ids[prefab], it_n->landing_id == -1 ? -1 : it_n->_landing_dist_curve[prefab]);
						}
					}
				}

				if (show_arcs)
				{
					for (std::vector<CSArc>::iterator it_a = it_n->_arcs.begin(); it_a != it_n->_arcs.end(); ++it_a)
					{
						if (it_a->cnode_to_vec_pos.orientation == (orientation + arc_connection))
						{
							if (it_a->turn_on_spot)
							{
								drawCSArc(it_a->pos_to.x, it_a->pos_to.y, it_a->stair_dir);
							}
							else
							{
								drawArc(pos.x, pos.y, it_a->pos_to.x, it_a->pos_to.y, false, false, it_a->stair_dir);
							}
						}
					}
				}
			}
		}
	}
}
void drawLDCSNodeVector()
{
	for (std::vector<CSNode>::iterator it_n = s_object_space.nodes_begin(); it_n != s_object_space.nodes_end(); ++it_n)
	{
		if (it_n->get_floor_num() == floor_num)
		{
			x_y<float> pos = it_n->get_position();

			if (pos.x > view_left - 0.1f && pos.x < view_right + 0.1f && pos.y > view_bottom - 0.1f && pos.y < view_top + 0.1f)
			{
				if (show_nodes)
				{
					if (mode == SIMULATE)
					{
						float pot = it_n->_potentials[cur_obj];
						if (pot > max_pot) max_pot = pot;
						drawCSNodeSim(pos.x, pos.y, pot, max_pot, false);

					}
					else
					{
						if (it_n->_valid.empty())
						{
							drawCSNode(pos.x, pos.y, false, it_n->_stair_id, it_n->_stair_ids[prefab], it_n->landing_id == -1 ? -1 : it_n->_landing_dist_curve[prefab]);
						}
						else
						{
							drawCSNode(pos.x, pos.y, it_n->_valid.at(prefab), it_n->_stair_id, it_n->_stair_ids[prefab], it_n->landing_id == -1 ? -1 : it_n->_landing_dist_curve[prefab]);
						}
					}
				}

				if (show_arcs)
				{
					for (std::vector<CSArc>::iterator it_a = it_n->_arcs.begin(); it_a != it_n->_arcs.end(); ++it_a)
					{
						if (it_a->cnode_to_vec_pos.orientation == (orientation + arc_connection))
						{
							if (it_a->turn_on_spot)
							{
								drawCSArc(it_a->pos_to.x, it_a->pos_to.y, it_a->stair_dir);
							}
							else
							{
								drawArc(pos.x, pos.y, it_a->pos_to.x, it_a->pos_to.y, false, false, it_a->stair_dir);
							}
						}
					}
				}
			}
		}
	}
}
void drawSimData()
{
	// configs (green)
	for (data_for_TCP::object_config& config : gl_sim_data.configs)
	{
		if (config.floor == floor_num)
		{
			drawXYTheta(config.x, config.y, config.angle - 90.0f, TOP_LAYER - 0.3f, 1.0f, 0.075f, 0.0f, 1.0f, 0.0f);
		}
	}

	// occupied nodes (blue)
	for (data_for_TCP::occupied_nodes& on : gl_sim_data.occ_nodes)
	{
		for (int& id : on.node_ids)
		{
			x_y<float> pos = s_object_space.get_tnode_xypos(id);
			drawRegPoly(pos.x, pos.y, TOP_LAYER - 0.2f, 0.1f, 8, 0.0f, 0.0f, 1.0f);
		}
	}

	// pvo costs (white -> red)
	std::vector<data_for_TCP::pvo>& pvo_draw = gl_sim_data.vos.empty() ? s_object_space.pvos_store : gl_sim_data.vos;
	for (data_for_TCP::pvo& vo : pvo_draw)
	{
		float max_cost = 0.0f;
		for (auto& noc : vo.node_obj_cost)
		{
			float cost = 0.0f;
			for (auto& oc : noc.second)
			{
				cost += oc.second;
			}

			if (cost > max_cost)
				max_cost = cost;
		}
		for (auto& noc : vo.node_obj_cost)
		{
			x_y<float> pos = s_object_space.get_tnode_xypos(noc.first);

			float cost = 0.0f;
			for (auto& oc : noc.second)
			{
				cost += oc.second;
			}
			float prop = cost / max_cost;

			drawRegPoly(pos.x, pos.y, TOP_LAYER - 0.1, 0.15f, 8, get_rainbow(prop, 0), get_rainbow(prop, 1), get_rainbow(prop, 2), 1.0f);
		}
	}
	
}

// Draw Geo
void drawGeo()
{
	int window_height = glutGet(GLUT_WINDOW_HEIGHT);

	if (mode == GEOMETRY) drawRegPoly(0.0f, 0.0f, BACKGROUND, 0.08f, 8, 0.8f, 0.8f, 0.8f);

	if (show_cells && mode == SIMULATE)
	{
		drawCells();
	}

	if (show_arcs)
	{
		drawArc1Vector();
	}

	if (show_nodes)
	{
		drawNodeVector();
	}

	if (show_bound)
	{
		drawBoundVector();
		drawStairVector();
		drawLandingVector();
	}

	if (adding && mousex != -1)
	{
		switch (edit_mode) {
		case EDIT_ARC:
			drawArc(firstx, firsty, mousex*scale + view_left, view_top - mousey * scale, true, false, NOT_STAIR_ARC);
			break;
		case EDIT_BOUND:
			drawBoundary(firstx, firsty, mousex*scale + view_left, view_top - mousey * scale, true);
			break;
		}
	}
}
void drawCSpace()
{
	int window_height = glutGet(GLUT_WINDOW_HEIGHT);

	if (mode == GEOMETRY) drawRegPoly(0.0f, 0.0f, BACKGROUND, 0.08f, 8, 0.8f, 0.8f, 0.8f);

	drawCSNodeVector();

	if (show_bound)
	{
		drawBoundVector();
		drawStairVector();
		drawLandingVector();
	}

}
void drawLDCSpace()
{
	int window_height = glutGet(GLUT_WINDOW_HEIGHT);

	if (mode == GEOMETRY) drawRegPoly(0.0f, 0.0f, BACKGROUND, 0.08f, 8, 0.8f, 0.8f, 0.8f);

	drawLDCSNodeVector();

	if (show_bound)
	{
		drawBoundVector();
		drawStairVector();
		drawLandingVector();
	}
}

// Draw Object
void drawPrevConfig(const onion_config& conf)
{
	if (hide_objs[conf.id]) return;

	float layer = get_prev_config_depth(conf.time);

	// outline
	if (show_poly)
	{
		for (std::vector<x_y<float>>::const_iterator it = conf.poly.verticies.begin(); it != conf.poly.verticies.end(); ++it)
		{
			std::vector<x_y<float>>::const_iterator next_it = it + 1;
			if (next_it == conf.poly.verticies.end()) next_it = conf.poly.verticies.begin();

			drawLine(layer, it->x, it->y, next_it->x, next_it->y, 2.0f, 0.5f, 0.5f, 0.5f);
		}
	}

	// attachment points
	if (show_ap)
	{
		for (const attachment_point& ap : conf.att_points)
		{
			if (ap.inside)
			{
				if (ap.occupied_circle != -1)
				{
					const capsule& cap = ap.occupation_circles;
					drawRegPoly(cap.points.begin()->x, cap.points.begin()->y, layer, cap.radius, 8, 0.7, 0.0, 0.0);
				}
			}
			else
			{
				for (int i = 0; i < ap.occupation_circles.points.size(); ++i)
				{
					if (ap.occupation_circles.invalid_points.find(i) == ap.occupation_circles.invalid_points.end()) // i is not invalid
					{
						if (ap.occupied_circle == i)
						{
							auto& point = ap.occupation_circles.points[i];
							drawRegPoly(point.x, point.y, layer, ap.occupation_circles.radius, 8, 0.0, 0.0, 0.7);
						}
						//drawRegPoly_nofill(point.x, point.y, OBJECT_3, ap.occupation_circles.radius, 8, 0.2, 0.2, 0.2, 1.0f);
					}
				}
			}
		}
	}

	// fill
	if (show_poly)
	{
		drawFillPoly(conf.poly.verticies, layer, 0.75f, 0.75f, 0.75f, 0.75f);
	}

	// CoR and direction
	if (show_cor)
	{
		drawXYTheta(conf.position.x, conf.position.y, conf.angle, layer - 0.00001f, 0.3f, 0.1f, 0.0f, 0.0f, 0.0f);
	}

}
void drawConfig(GenObject& obj)
{
	polygon poly = obj.get_polygon();
	std::vector<attachment_point> att_point = obj._attachment_points;
	relative_to_object(obj._orientation, obj._position, poly, att_point);

	if (save_pre_configs && obj.just_moved)
	{
		save_config(obj, s_object_space.sim_time, poly, att_point);
	}

	if (hide_objs[obj.get_object_id()]) return;

	if (!(obj.get_floor() == floor_num || (obj.stair_id != -1 && obj.get_floor() + 1 == floor_num))) return;

	// outline
	if (show_poly)
	{
		for (std::vector<x_y<float>>::iterator it = poly.verticies.begin(); it != poly.verticies.end(); ++it)
		{
			std::vector<x_y<float>>::iterator next_it = it + 1;
			if (next_it == poly.verticies.end()) next_it = poly.verticies.begin();

			drawLine(OBJECT_1, it->x, it->y, next_it->x, next_it->y, 2.0f, 0.2f, 0.2f, 0.2f);
		}
	}

	
	// attachment points
	if (show_ap)
	{
		for (std::vector<attachment_point>::iterator it = att_point.begin(); it != att_point.end(); ++it)
		{
			if (it->inside)
			{
				if (it->occupied_circle != -1)
				{
					capsule& cap = it->occupation_circles;
					drawRegPoly(cap.points.begin()->x, cap.points.begin()->y, OBJECT_1, cap.radius, 8, 0.7, 0.0, 0.0);
				}
			}
			else
			{
				int i = 0;
				for (std::vector<x_y<float>>::iterator it2 = it->occupation_circles.points.begin(); it2 != it->occupation_circles.points.end(); it2++)
				{
					if (it->occupation_circles.invalid_points.find(i) == it->occupation_circles.invalid_points.end()) // i is not invalid
					{
						if (it->occupied_circle == i)
						{
							drawRegPoly(it2->x, it2->y, OBJECT_1, it->occupation_circles.radius, 8, 0.0, 0.0, 0.7);
						}
						//drawRegPoly_nofill(it2->x, it2->y, OBJECT_3, it->occupation_circles.radius, 8, 0.2, 0.2, 0.2, 1.0f);
					}
					++i;
				}
			}
		}
	}

	// CoR and direction
	if (show_cor)
	{
		drawXYTheta(obj._position.x, obj._position.y, obj._orientation, OBJECT_1 - 0.1f, 0.3f, 0.1f, 0.0f, 0.0f, 0.0f);
	}

	// vos
	if (show_vos)
	{
		for (const velocity_obstacle& vo : obj.get_vos())
		{
			drawVO(vo, obj._position, obj.velocity_current.first, obj.velocity_desired);
		}
	}

	// fill
	if (show_poly)
	{
		drawFillPoly(poly.verticies, OBJECT_0, 0.5f, 0.5f, 0.5f, 0.5f);
	}
}
void drawObj()
{
	std::vector<GenObject> objs = s_object_space.get_objects();
	for (GenObject& obj : objs)
	{
		drawConfig(obj);
	}

	if (show_pre_configs && !previous_configs.empty())
	{
		for (auto conf = previous_configs.rbegin(); conf != previous_configs.rend(); ++conf)
		{
			if (conf->draw_me && (conf->floors.first == floor_num || conf->floors.second == floor_num))
				drawPrevConfig(*conf);
		}
	}
}

// Onion Skinning
void reset_all_pre_configs()
{
	previous_configs.clear();
}
void save_config(const GenObject& obj, float time, const polygon& poly, const std::vector<attachment_point>& att_points)
{
	if (!previous_configs.empty())
	{
		for (auto pc = previous_configs.rbegin(); pc != previous_configs.rend(); ++pc)
		{
			if (pc->id == obj.get_object_id())
			{
				pc->draw_me = true;
				break;
			}
		}
	}

	std::pair<int, int> floors = { obj.get_floor(), obj.stair_id == -1 ? obj.get_floor() : obj.get_floor() + 1 };
	previous_configs.push_back({ obj.get_object_id(), time, obj._position, obj._orientation, floors, obj.stair_id, obj.velocity_current, poly, att_points });
}
float get_prev_config_depth(float prev_time)
{
	return OBJECT_prev - (prev_time / s_object_space.sim_time);
}

// Draw grid
void updateGrid()
{
	grid_first_x = ceil(view_left / (increment / 100.0f));
	grid_last_x = floor(view_right / (increment / 100.0f));
	grid_first_y = ceil(view_bottom / (increment / 100.0f));
	grid_last_y = floor(view_top / (increment / 100.0f));
}
void drawGridX(int counter)
{
	if ((grid_last_x - grid_first_x) < counter) return;

	float shade = 0.8;
	drawLine(BACKGROUND, (grid_first_x + counter)*(increment / 100.0f), view_top, (grid_first_x + counter)*(increment / 100.0f), view_bottom, 0.1, shade, shade, shade);

	counter++;
	drawGridX(counter);
}
void drawGridY(int counter)
{
	if ((grid_last_y - grid_first_y) < counter) return;

	float shade = 0.8;
	drawLine(BACKGROUND, view_left, (grid_first_y + counter)*(increment / 100.0f), view_right, (grid_first_y + counter)*(increment / 100.0f), 0.1, shade, shade, shade);

	counter++;
	drawGridY(counter);
}

// Write text
void writeTextScreen(int x_pos, int y_pos, void* font, const char* string, float red, float green, float blue)
{
	glColor3f(red, green, blue);

	const char *c;
	glPushMatrix();
	glLoadIdentity();

	glRasterPos3f(x_pix_to_pos(x_pos), y_pix_to_pos(y_pos), TOP_LAYER);

	for (c = string; *c != '\0'; c++)
	{
		glutBitmapCharacter(font, *c);
	}
	glPopMatrix();

	glColor3f(1.0f, 1.0f, 1.0f);
}
void writeTextGeo(float x_pos, float y_pos, void* font, char* string, float red, float green, float blue)
{
	glColor3f(red, green, blue);
	char *c;
	glRasterPos3f(x_pos, y_pos, TOP_LAYER);

	for (c = string; *c != '\0'; c++)
	{
		glutBitmapCharacter(font, *c);
	}
	glColor3f(1.0f, 1.0f, 1.0f);
}
void writeTextGeoc(float x_pos, float y_pos, void* font, char c, float red, float green, float blue)
{
	glColor3f(red, green, blue);
	glRasterPos3f(x_pos, y_pos, TOP_LAYER);

	glutBitmapCharacter(font, c);

	glColor3f(1.0f, 1.0f, 1.0f);
}
void write_edit_mode()
{
	int height = 30;

	glPushMatrix();
	glLoadIdentity();
	glRasterPos3f(x_pix_to_pos(0), y_pix_to_pos(0), UI_1);
	glColor4f(0.6078f, 0.8549f, 0.9137f, 0.8f);
	glBegin(GL_POLYGON);
	glVertex3f(x_pix_to_pos(0), y_pix_to_pos(0), 0.0);
	glVertex3f(x_pix_to_pos(1024), y_pix_to_pos(0), 0.0);
	glVertex3f(x_pix_to_pos(1024), y_pix_to_pos(64), 0.0);
	glVertex3f(x_pix_to_pos(0), y_pix_to_pos(64), 0.0);
	glEnd();
	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	glPopMatrix();

	drawLine(UI_2, x_pix_to_pos(0), y_pix_to_pos(0), x_pix_to_pos(1024), y_pix_to_pos(0), 0.2f, 0.0, 0.0, 0.0);
	drawLine(UI_2, x_pix_to_pos(1024), y_pix_to_pos(0), x_pix_to_pos(1024), y_pix_to_pos(64), 0.2f, 0.0, 0.0, 0.0);
	drawLine(UI_2, x_pix_to_pos(1024), y_pix_to_pos(64), x_pix_to_pos(0), y_pix_to_pos(64), 0.2f, 0.0, 0.0, 0.0);
	drawLine(UI_2, x_pix_to_pos(0), y_pix_to_pos(64), x_pix_to_pos(0), y_pix_to_pos(0), 0.2f, 0.0, 0.0, 0.0);

	switch (mode) {
	case GEOMETRY:
		writeTextScreen(25, height, GLUT_BITMAP_HELVETICA_18, "GEOMETRY:", 0, 0, 0);
		writeTextScreen(55, height - 15, GLUT_BITMAP_HELVETICA_12, "<-\`m\'->", cspace_generated && !cspace_generating ? 0.0 : 0.7, cspace_generated && !cspace_generating ? 0.0 : 0.7, cspace_generated && !cspace_generating ? 0.0 : 0.7);

		writeTextScreen(175, height, GLUT_BITMAP_HELVETICA_18, "Edit", add ? 0.7 : 0.0, add ? 0.7 : 0.0, add ? 0.7 : 0.0);
		writeTextScreen(225, height, GLUT_BITMAP_HELVETICA_18, "Add", add ? 0.0 : 0.7, add ? 0.0 : 0.7, add ? 0.0 : 0.7);
		writeTextScreen(195, height - 15, GLUT_BITMAP_HELVETICA_12, "<-\`a\'->", 0, 0, 0);

		writeTextScreen(300, height, GLUT_BITMAP_HELVETICA_18, "Nodes", edit_mode == EDIT_NODE ? 0.0 : 0.7, edit_mode == EDIT_NODE ? 0.0 : 0.7, edit_mode == EDIT_NODE ? 0.0 : 0.7);
		writeTextScreen(322, height - 15, GLUT_BITMAP_HELVETICA_12, "\`1\'", 0, 0, 0);
		if (auto_connect) writeTextScreen(290, height + 20, GLUT_BITMAP_HELVETICA_12, "auto connect", 0, 0, 0);


		writeTextScreen(375, height, GLUT_BITMAP_HELVETICA_18, "Arcs", edit_mode == EDIT_ARC ? 0.0 : 0.7, edit_mode == EDIT_ARC ? 0.0 : 0.7, edit_mode == EDIT_ARC ? 0.0 : 0.7);
		writeTextScreen(387, height - 15, GLUT_BITMAP_HELVETICA_12, "\`2\'", 0, 0, 0);

		writeTextScreen(435, height, GLUT_BITMAP_HELVETICA_18, "Boundary", edit_mode == EDIT_BOUND ? 0.0 : 0.7, edit_mode == EDIT_BOUND ? 0.0 : 0.7, edit_mode == EDIT_BOUND ? 0.0 : 0.7);
		writeTextScreen(467, height - 15, GLUT_BITMAP_HELVETICA_12, "\`3\'", 0, 0, 0);

		writeTextScreen(540, height, GLUT_BITMAP_HELVETICA_18, (show_grid ? (increment == 100 ? "Grid: 1.0m" : (increment == 50 ? "Grid: 0.5m" : (increment == 25 ? "Grid: 0.25m" : "Grid: 0.1m"))) : "Grid: OFF"), show_grid ? 0.0 : 0.7, show_grid ? 0.0 : 0.7, show_grid ? 0.0 : 0.7);
		writeTextScreen(578, height - 15, GLUT_BITMAP_HELVETICA_12, "\`g\'", 0, 0, 0);

		writeTextScreen(660, height, GLUT_BITMAP_HELVETICA_18, cspace_generating ? "C-Space: generating..." : cspace_generated ? "C-Space: generated" : "C-Space: NONE", cspace_generating ? 0.7 : cspace_generated ? 0.0 : 0.8, cspace_generating ? 0.7 : cspace_generated ? 0.8 : 0.0, cspace_generating ? 0.7 : cspace_generated ? 0.0 : 0.0);
		writeTextScreen(725, height - 15, GLUT_BITMAP_HELVETICA_12, "\`c\'", 0, 0, 0);

		writeTextScreen(855, height, GLUT_BITMAP_HELVETICA_18, "Save", 0.0, 0.0, 0.0);
		writeTextScreen(875, height - 15, GLUT_BITMAP_HELVETICA_12, "\`s\'", 0, 0, 0);

		writeTextScreen(910, height, GLUT_BITMAP_HELVETICA_18, "Load", 0.0, 0.0, 0.0);
		writeTextScreen(927, height - 15, GLUT_BITMAP_HELVETICA_12, "\`l\'", 0, 0, 0);

		writeTextScreen(965, height, GLUT_BITMAP_HELVETICA_18, "Clear", 0.0, 0.0, 0.0);
		writeTextScreen(982, height - 15, GLUT_BITMAP_HELVETICA_12, "\`r\'", 0, 0, 0);

		break;

	case OBJECT:
		writeTextScreen(25, height, GLUT_BITMAP_HELVETICA_18, "OBJECT:", 0, 0, 0);
		writeTextScreen(40, height - 15, GLUT_BITMAP_HELVETICA_12, "<-\`m\'->", object_free_space && !object_generating ? 0.0 : 0.7, object_free_space && !object_generating ? 0.0 : 0.7, object_free_space && !object_generating ? 0.0 : 0.7);

		writeTextScreen(145, height, GLUT_BITMAP_HELVETICA_18, "Generate Object", 0.0, 0.8, 0.0);
		writeTextScreen(210, height - 15, GLUT_BITMAP_HELVETICA_12, "\`o\'", object_generating ? 0.7 : 0.0, object_generating ? 0.7 : 0.0, object_generating ? 0.7 : 0.0);

		writeTextScreen(325, height, GLUT_BITMAP_HELVETICA_18, object_free_space ? "Free Space: generated" : object_free_space_generating ? "Free Space: generating..." : "Free Space: NONE", object_free_space ? 0.0 : object_free_space_generating ? 0.7 : 0.8, object_free_space ? 0.8 : object_free_space_generating ? 0.7 : 0.0, object_free_space_generating ? 0.7 : 0.0);
		writeTextScreen(405, height - 15, GLUT_BITMAP_HELVETICA_12, "\`f\'", 0.0, 0.0, 0.0);

		if (object_generating)
		{
			writeTextScreen(525, height, GLUT_BITMAP_HELVETICA_18, "Select node to add object", 0, 0, 0);
			writeTextScreen(610, height - 15, GLUT_BITMAP_HELVETICA_12, "<click>", 0, 0, 0);

			writeTextScreen(765, height, GLUT_BITMAP_HELVETICA_18, "Change Orientation", 0, 0, 0);
			writeTextScreen(840, height - 15, GLUT_BITMAP_HELVETICA_12, "\`o\'", 0, 0, 0);
		}

		break;

	case SIMULATE:
		writeTextScreen(25, height, GLUT_BITMAP_HELVETICA_18, "SIMULATE:", 0, 0, 0);
		writeTextScreen(50, height - 15, GLUT_BITMAP_HELVETICA_12, "<-\`m\'->", cspace_generated && !cspace_generating ? 0.0 : 0.7, cspace_generated && !cspace_generating ? 0.0 : 0.7, cspace_generated && !cspace_generating ? 0.0 : 0.7);

		writeTextScreen(145, height, GLUT_BITMAP_HELVETICA_18, potential_generated ? "Goal: selected" : potential_generating ? "Goal: generating" : "Goal: NONE", potential_generated ? 0.0 : potential_generating ? 0.7 : 0.8, potential_generated ? 0.8 : potential_generating ? 0.7 : 0.0, potential_generated ? 0.0 : potential_generating ? 0.7 : 0.0);
		writeTextScreen(190, height - 15, GLUT_BITMAP_HELVETICA_12, "\`g\'", potential_generating ? 0.7 : 0.0, potential_generating ? 0.7 : 0.0, potential_generating ? 0.7 : 0.0);

		writeTextScreen(290, height, GLUT_BITMAP_HELVETICA_18, simulate ? "Pause simulation" : "Play simulation", potential_generated ? 0.0 : 0.7, potential_generated ? 0.0 : 0.7, potential_generated ? 0.0 : 0.7);
		writeTextScreen(350, height - 15, GLUT_BITMAP_HELVETICA_12, "\`p\'", potential_generated ? 0.0 : 0.7, potential_generated ? 0.0 : 0.7, potential_generated ? 0.0 : 0.7);

		writeTextScreen(625, height, GLUT_BITMAP_HELVETICA_18, "Place object", 0, 0, 0);
		writeTextScreen(670, height - 15, GLUT_BITMAP_HELVETICA_12, "\`o\'", 0, 0, 0);

		if (selecting_goals)
		{
			writeTextScreen(785, height, GLUT_BITMAP_HELVETICA_18, "Select node to add goal", 0, 0, 0);
			writeTextScreen(870, height - 15, GLUT_BITMAP_HELVETICA_12, "<click>", 0, 0, 0);
		}
		else if (object_generating)
		{
			writeTextScreen(785, height, GLUT_BITMAP_HELVETICA_18, "Select node to add object", 0, 0, 0);
			writeTextScreen(875, height - 15, GLUT_BITMAP_HELVETICA_12, "<click>", 0, 0, 0);
		}
		else
		{
			writeTextScreen(785, height, GLUT_BITMAP_HELVETICA_18, "Speed Control", speed_control ? 0.0 : 0.7, speed_control ? 0.0 : 0.7, speed_control ? 0.0 : 0.7);
			writeTextScreen(840, height - 15, GLUT_BITMAP_HELVETICA_12, "\`c\'", 0, 0, 0);
		}
		break;
	case TCP_CONTROL:
		writeTextScreen(25, height, GLUT_BITMAP_HELVETICA_18, "TCP_CONTROL", 0, 0, 0);
		break;
	}
	// logs
	int height1 = 80;
	if (log_cmd.on)
	{
		writeTextScreen(10, height1, GLUT_BITMAP_HELVETICA_12, "CMD: ", 0.0f, 0.0f, 0.0f);
		writeTextScreen(70, height1, GLUT_BITMAP_HELVETICA_12, log_cmd.get_last(), 0.0f, 0.0f, 0.0f);
		height1 += 20;
	}
	if (log_opengl.on)
	{
		writeTextScreen(10, height1, GLUT_BITMAP_HELVETICA_12, "OPENGL: ", 0.0f, 0.0f, 0.0f);
		writeTextScreen(70, height1, GLUT_BITMAP_HELVETICA_12, log_opengl.get_last(), 0.0f, 0.0f, 0.0f);
		height1 += 20;
	}
	if (log_TCP.on)
	{
		writeTextScreen(10, height1, GLUT_BITMAP_HELVETICA_12, "TCP: ", 0.0f, 0.0f, 0.0f);
		writeTextScreen(70, height1, GLUT_BITMAP_HELVETICA_12, log_TCP.get_last(), 0.0f, 0.0f, 0.0f);
		height1 += 20;
	}
	if (log_main.on)
	{
		writeTextScreen(10, height1, GLUT_BITMAP_HELVETICA_12, "MAIN: ", 0.0f, 0.0f, 0.0f);
		writeTextScreen(70, height1, GLUT_BITMAP_HELVETICA_12, log_main.get_last(), 0.0f, 0.0f, 0.0f);
		height1 += 20;
	}
}

//delay
void delayed(int delay) // for "delay" miliseconds
{
	if (!wait && speed_control) {//done when first called
		wait = true;
		glutTimerFunc(delay, delayed, 0);
	}
	else //done after delay msec
	{
		wait = false;
	}
}
void reset_show_nodes(int b)
{
	show_nodes = b;
}
void reset_show_arcs(int b)
{
	show_arcs = b;
}
void reset_show_bound(int b)
{
	show_bound = b;
}
void hide_geo_if_changed(int ms_delay)
{
	int new_bounds = s_object_space.get_num_bounds();
	int new_nodes = s_object_space.get_num_tnodes();
	int new_arcs = s_object_space.get_num_tarcs();

	if (num_bounds != new_bounds)
	{
		num_bounds = new_bounds;
		glutTimerFunc(ms_delay, reset_show_bound, show_bound);
		show_bound = false;
	}

	if (num_nodes != new_nodes)
	{
		num_nodes = new_nodes;
		glutTimerFunc(ms_delay, reset_show_nodes, show_nodes);
		show_nodes = false;
	}

	if (num_arcs != new_arcs)
	{
		num_arcs = new_arcs;
		glutTimerFunc(ms_delay, reset_show_arcs, show_arcs);
		show_arcs = false;
	}
}
unsigned int last_update;
void delayed_update(int update_freq_ms)
{
	int this_update = glutGet(GLUT_ELAPSED_TIME);
	
	if (last_update + update_freq_ms < this_update)
	{
		last_update = this_update;
	}
}

// SIM
int GLSIM_step = 0;
DWORD GLSIM_comptime_start = 0;
float GLSIM_simtime = 0.0f;

// RENDER
int render_print = 0;
const int render_print_freq = 10000;
DWORD last_clock_check = 0;
bool first = true;
void render()
{
	delayed_update(500);
	hide_geo_if_changed(150);

	++render_print;
	DWORD this_check = GetTickCount();
	if (this_check - last_clock_check > render_print_freq)
	{
		float fps = (render_print*1000.0f)/(float)(this_check - last_clock_check);
		render_print = 0;
		last_clock_check = this_check;
		//log_opengl.print("Average FPS: %f", fps);
	}

	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	if (show_grid) { drawGridX(0); drawGridY(0); }

	if (show_cspace)
	{
		drawCSpace();
	}
	else if (show_ldcspace)
	{
		drawLDCSpace();
	}
	else
	{
		drawGeo();
	}

	drawObj();

	if (show_sim_data)
		drawSimData();

	write_edit_mode();

	glutSwapBuffers();

	//generate cspace
	if (cspace_generating)
	{
		s_object_space.generate_cspace(false);

		cspace_generating = false;
		cspace_generated = true;
		object_generating = false;
		object_free_space = false;
	}
	//generate object
	if (object_generating && select.node)
	{
		select.deselect();
		object_generating = false;
	}

	//generate freespace
	if (object_free_space_generating)
	{
		s_object_space.generate_freespace();

		object_free_space = true;
		object_free_space_generating = false;
	}

	//generate potential
	if (potential_generating && !goals.empty())
	{
		potential_generating = false;
		potential_generated = true;
		show_nodes = false;
		show_arcs = false;
		show_bound = true;
		show_cells = true;
	}
	
	if ((simulate || simulate_step) && !simulate_pause && !(wait && speed_control))
	{
		if (simulate_step)
		{
			simulate_step = false;

			log_opengl.print("Simulation stepped forward");
		}
		else // if simulate
		{
			if (speed_control)
			{
				delayed(iSIM_STEP_MSEC);
			}
			if (first)
			{
				GLSIM_step = 0;
				GLSIM_comptime_start = GetTickCount();
				GLSIM_simtime = 0.0f;
			}

			gl_sim_data = s_object_space.main_sim_step_ALL(fSIM_STEP_SEC, first);
			log_opengl.print("Simulation stepped forward");
			log_opengl.print(2, "%d configs", (int)gl_sim_data.configs.size());
			log_opengl.print(2, "%d occ_nodes", (int)gl_sim_data.occ_nodes.size());
			log_opengl.print(2, "%d vos", (int)gl_sim_data.vos.size());

			s_object_space.output_sim_data("data_sim.csv", GLSIM_step, 0, (GetTickCount() - GLSIM_comptime_start), GLSIM_simtime, first);
			
			first = false;

			GLSIM_step++;
			GLSIM_simtime += fSIM_STEP_SEC;
		}
	}

	if (cleared)
	{
		cleared = false;
		cspace_generating = false;
		cspace_generated = false;
		object_generating = false;
		object_free_space = false;
		potential_generating = false;
		potential_generated = false;
		simulate = false;
		simulate_step = false;
		orient = 0;
		goals.clear();
		selecting_goals = false;
		select.deselect();
	}
}

int Initialize_GLUT(int argc, char* argv[], int& other_window_size_x, int& other_window_size_y, float prop_of_screen, bool TCP_control, std::string title)
{
	// Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	
	window_width = floor(glutGet(GLUT_SCREEN_WIDTH) * prop_of_screen);
	int window_x_pos = glutGet(GLUT_SCREEN_WIDTH) - window_width - 8;
	window_height = glutGet(GLUT_SCREEN_HEIGHT) - 72;

	other_window_size_x = window_x_pos;
	other_window_size_y = window_height;
	 
	//window_height = glutGet(GLUT_SCREEN_HEIGHT);
	glutInitWindowPosition(window_x_pos, 0);
	glutInitWindowSize(window_width, window_height);
	mainWindow = glutCreateWindow(title.c_str());
	createGLUTMenus();
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

	//enable alpha
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glClearColor(0.0, 0.0, 0.0, 0.0);

	//depth
	glEnable(GL_DEPTH_TEST);

	// register callbacks
	glutReshapeFunc(changeViewPort);
	glutDisplayFunc(render);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(pressKey);
	glutIdleFunc(render);
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);

	// Initializes the entry points in the OpenGL driver so we can call all the functions in the API
	GLenum err = glewInit();
	if (GLEW_OK != err) { fprintf(stderr, "GLEW error"); return -1; }
	log_opengl.print("Initializing OpenGL window");
	char c1[100];
	sprintf(c1, "Using freeglut with glew-%s", glewGetString(GLEW_VERSION));
	char c2[100];
	sprintf(c2, "OpenGL version supported: %s", glGetString(GL_VERSION));
	log_opengl.print(2,c1);
	log_opengl.print(2,c2);

	last_clock_check = GetTickCount();
	last_update = glutGet(GLUT_ELAPSED_TIME);

	TCP_cont = TCP_control;
	if (TCP_cont)
	{
		mode = TCP_CONTROL;
		show_sim_data = false;
	}
	return 0;
}