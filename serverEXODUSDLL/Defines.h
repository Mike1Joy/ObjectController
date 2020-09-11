// Author/editor/coder: Simo
#ifndef DEFINES_H
#define DEFINES_H
#define NULL 0
//#define EXODUS_SERVER_DLL_EXPORTS // Defined or not defined in the compilation settings!
#ifdef EXODUS_SERVER_DLL_EXPORTS
#define EXODUS_SERVER_DLL_API __declspec(dllexport) 
#else
#define EXODUS_SERVER_DLL_API __declspec(dllimport) 
#endif

#endif