#pragma once
#include <iostream>
#include <map>

#include "DataStructs.h"
#include "ObjectSpace.h"
#include "Log.h"

static bool CMD_loop = true;
static char cmd_str_buff[20];
static int cmd_int_buff;
static float cmd_float_buff;

static bool bmain = log_main.on;
static bool bTCP = log_TCP.on;
static bool bopengl = log_opengl.on;

extern bool opengl_freeze;

void cmd_fill_str_buff()
{
	std::cin >> cmd_str_buff;
}
bool test_str_buff(const char * txt)
{
	return strcmp(cmd_str_buff, txt) == 0;
}
void cmd_fill_int_buff()
{
	std::cin >> cmd_int_buff;
}
void cmd_fill_float_buff()
{
	std::cin >> cmd_float_buff;
}

void restore_logs()
{
	log_main.on = bmain;
	log_TCP.on = bTCP;
	log_opengl.on = bopengl;
}
void test_duplicates()
{
	log_cmd.print("testing for duplicates...");

	std::map<int, int> duplicates = s_object_space.test_duplicate_ids_tnodes();
	bool none = true;

	for (std::pair<int, int> d : duplicates)
	{
		if (d.second > 1)
		{
			none = false;
			log_cmd.print(0, "tnode id %d has duplicates:", d.first);
			log_cmd.print(2, "%d", d.second);
		}
	}
	if (none)
	{
		log_cmd.print(0, "no duplicate tnodes");
	}
	
	/////

	duplicates = s_object_space.test_duplicate_ids_tarcs();
	none = true;

	for (std::pair<int, int> d : duplicates)
	{
		if (d.second > 1)
		{
			none = false;
			log_cmd.print(0, "tarc id %d has duplicates:", d.first);
			log_cmd.print(2, "%d", d.second);
		}
	}
	if (none)
	{
		log_cmd.print(0, "no duplicate tarcs");
	}

	/////

	std::map<cnode_pos, int> duplicates2 = s_object_space.test_duplicate_ids_cnodes();
	none = true;

	for (std::pair<cnode_pos, int> d : duplicates2)
	{
		if (d.second > 1)
		{
			none = false;
			log_cmd.print(0, "cnode id %d has duplicates:", d.first.vec_pos);
			log_cmd.print(2, "%d", d.second);
		}
	}
	if (none)
	{
		log_cmd.print(0, "no duplicate cnodes");
	}

	log_cmd.print("end of duplicate test");
}

void cmd_loop()
{
	log_cmd.print("CMD input started:");
	log_cmd.print(2,"Press ENTER to enter a command");

	while (CMD_loop)
	{
		std::cin.ignore(10000, '\n');
		std::cin.get();

		bmain = log_main.on;
		bTCP = log_TCP.on;
		bopengl = log_opengl.on;

		log_main.on = false;
		log_TCP.on = false;
		log_opengl.on = false;

		log_cmd.print("Enter a command:");
		log_cmd.print();

		cmd_fill_str_buff();
		if (test_str_buff("add"))
		{
			cmd_fill_str_buff();
			if (test_str_buff("node"))
			{
				cmd_fill_float_buff();
				float x = cmd_float_buff;
				cmd_fill_float_buff();
				float y = cmd_float_buff;

				restore_logs();
				s_object_space.TCP_add_node(node(s_object_space.get_next_node_id(), x, y, -1, -1, 1));
			}
		}
		else if (test_str_buff("log"))
		{
			cmd_fill_str_buff();
			if (test_str_buff("tcp"))
			{
				cmd_fill_str_buff();
				if (test_str_buff("on"))
				{
					bTCP = true;
				}
				else if (test_str_buff("off"))
				{
					bTCP = false;
				}
			}
			else if (test_str_buff("opengl"))
			{
				cmd_fill_str_buff();
				if (test_str_buff("on"))
				{
					bopengl = true;
				}
				else if (test_str_buff("off"))
				{
					bopengl = false;
				}
			}
			else if (test_str_buff("main"))
			{
				cmd_fill_str_buff();
				if (test_str_buff("on"))
				{
					bmain = true;
				}
				else if (test_str_buff("off"))
				{
					bmain = false;
				}
			}
			else if (test_str_buff("all"))
			{
				cmd_fill_str_buff();
				if (test_str_buff("on"))
				{
					bTCP = true;
					bopengl = true;
					bmain = true;
				}
				else if (test_str_buff("off"))
				{
					bTCP = false;
					bopengl = false;
					bmain = false;
				}
			}
		}
		else if (test_str_buff("opengl"))
		{
			cmd_fill_str_buff();
			if (test_str_buff("freeze"))
			{
				
			}
			else if (test_str_buff("unfreeze"))
			{
				
			}
		}
		else if (test_str_buff("test"))
		{
			cmd_fill_str_buff();
			if (test_str_buff("dup"))
				test_duplicates();
			if (test_str_buff("arc"))
				s_object_space.correct_tarc_pos();
		}
		else if (test_str_buff("tcp"))
		{
			cmd_fill_str_buff();
			if (test_str_buff("unknown"))
			{
				log_TCP.on = true;
				print_types();
			}
		}
		else
		{
			log_cmd.print("Command not recognised");
		}

		std::cin.clear();


		for (int i = 0; i < 20; i++)
		{
			cmd_str_buff[i] = 0;
		}

		restore_logs();
	}

	log_cmd.print("CMD input ended");
}