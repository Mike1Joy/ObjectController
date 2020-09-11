#pragma once

#include <iostream>
#include <windows.h>

enum Log_colour { CONS_WHITE = 15, CONS_RED = 12, CONS_GREEN = 10, CONS_YELLOW = 14 };

class Log
{
	Log_colour m_log_colour;
	static bool s_printing;
	static int s_indent;
	int m_indent;
	const char * m_label;
	WORD wOldColorAttrs;

	void clear_str_buff(int start)
	{
		for (int i = start; i < 100; ++i)
			m_last_str[i] = 0;
	}
	char m_last_str[100];
	
	Log();
	void print_label(int indent);

public:
	bool on;

	Log(Log_colour colour, const char* label);

	char* get_last();

	void print();
	void print(const char * message);
	void print(const char * message, int t);
	void print(const char * message, float t);
	void print(const char * message, bool t);

	void print(int indent);
	void print(int indent, const char * message);
	void print(int indent, const char * message, int t);
	void print(int indent, const char * message, float t);
	void print(int indent, const char * message, bool t);
};

extern Log log_main;
extern Log log_TCP;
extern Log log_opengl;
extern Log log_cmd;