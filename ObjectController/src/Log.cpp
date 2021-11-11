#include "Log.h"

int Log::s_indent = 1;
bool Log::s_printing = false;

Log::Log(Log_colour colour, const char* label) : m_log_colour(colour), on(true), m_label(label), to_file(false)
{
	std::string s(label);
	m_indent = s.size();
	if (m_indent > s_indent)
	{
		s_indent = m_indent;
	}
	
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
	GetConsoleScreenBufferInfo(hConsole, &csbiInfo);
	wOldColorAttrs = csbiInfo.wAttributes;

	clear_str_buff(0);

}

void Log::print_label(int indent)
{
	std::cout << "[" << m_label << "]: ";
	for (int i = 0; i < s_indent - m_indent; ++i)
	{
		std::cout << " ";
	}
	std::cout << ">";
	for (int i = 0; i < indent; ++i)
	{
		std::cout << " ";
	}
}

char* Log::get_last()
{
	return m_last_str;
}

void Log::print(int indent)
{
	if (!on) return;

	while (s_printing) {}
	s_printing = true;
	clear_str_buff(0);

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
	GetConsoleScreenBufferInfo(hConsole, &csbiInfo);
	WORD wOldColorAttrs = csbiInfo.wAttributes;

	SetConsoleTextAttribute(hConsole, m_log_colour);

	print_label(indent);

	SetConsoleTextAttribute(hConsole, wOldColorAttrs);
	s_printing = false;
}
void Log::print(int indent, const char * message)
{
	if (!on) return;
	while (s_printing) {}
	s_printing = true;
	clear_str_buff(0);
	sprintf(m_last_str, message);

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, m_log_colour);

	print_label(indent);

	printf(message);
	if (!(std::strrchr(message, '\n')))
		std::cout << std::endl;

	SetConsoleTextAttribute(hConsole, wOldColorAttrs);
	s_printing = false;
}
void Log::print(int indent, const char * message, int t)
{
	if (!on) return;
	while (s_printing) {}
	s_printing = true;
	clear_str_buff(0);
	sprintf(m_last_str, message,t);

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, m_log_colour);

	print_label(indent);

	printf(message,t);
	if (!(std::strrchr(message, '\n')))
		std::cout << std::endl;

	SetConsoleTextAttribute(hConsole, wOldColorAttrs);
	s_printing = false;
}
void Log::print(int indent, const char * message, float t)
{
	if (!on) return;
	while (s_printing) {}
	s_printing = true;
	clear_str_buff(0);
	sprintf(m_last_str, message, t);

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, m_log_colour);

	print_label(indent);

	printf(message, t);
	if (!(std::strrchr(message, '\n')))
		std::cout << std::endl;

	SetConsoleTextAttribute(hConsole, wOldColorAttrs);
	s_printing = false;
}
void Log::print(int indent, const char * message, bool t)
{
	if (!on) return;
	while (s_printing) {}
	s_printing = true;
	clear_str_buff(0);
	sprintf(m_last_str, message, t);

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, m_log_colour);

	print_label(indent);

	printf(message, t);
	if (!(std::strrchr(message, '\n')))
		std::cout << std::endl;

	SetConsoleTextAttribute(hConsole, wOldColorAttrs);
	s_printing = false;
}

void Log::print()
{
	print(0);
}
void Log::print(const char * message)
{
	print(0, message);
}
void Log::print(const char * message, int t)
{
	print(0, message, t);
}
void Log::print(const char * message, float t)
{
	print(0, message, t);
}
void Log::print(const char * message, bool t)
{
	print(0, message, t);
}