// Author/editor/coder: Simo
#pragma once
#ifndef STR_H
#define STR_H
#include "IComparable.h"
#include "ArrayList.h"
#include <cstring>
#include <cstdio>
#include <Shlwapi.h>
#include <Windows.h>
#pragma comment( lib, "Shlwapi.lib" )
#define NULL 0
// CString is a reserved name apparently
class CStr : public CIComparable<CStr>
{
public:
	CStr(void);
	CStr(CStr * pOriginal);
	CStr(char * pszValue);
	CStr(const char * pszValue);
	CStr(char * pszValue, int iOffset, int iCount);
	CStr(const char * pszValue, int iOffset, int iCount);
	CStr(int iValue);
	CStr(float fValue);
	CStr(float fValue, int iDecimalPoints);
	CStr(char cValue);
	~CStr(void);

	int Length();
	bool IsEmpty();
	void Keep();
	const char * GetArray();
	CStr * Temp();
	
	CStr * operator+(CStr * rhs);
	CStr * operator+(const char * rhs);
	CStr * operator+(int rhs);
	CStr * operator+(float rhs);
	CStr * operator+(char rhs);
	CStr * operator=(CStr * rhs);
	CStr * operator=(const char * rhs);
	void operator+=(CStr * rhs);
	void operator+=(const char * rhs);
	void operator+=(int rhs);
	void operator+=(float rhs);
	void operator+=(char rhs);
	bool operator==(CStr * rhs);
	bool operator==(const char * rhs);
	char operator[](int index);
	CStr * operator()(int iValue);
	CStr * operator()(float fValue);

	CStr * Trim();
	CStr * Substring(int iBeginIndex);
	CStr * Substring(int iBeginIndex, int iEndIndex);
	// Doesn't support regular expression yet
	// Currently limited to 1 character
	//CStr ** Split(CStr * pRegex, int iLimit);
	CArrayList<CStr> * Split(char ch, int iLimit);
	int IndexOf(char ch);
	int IndexOf(char ch, int iFromIndex);
	void ToLowerCase();
	CStr * FindExtension();

	bool IsInt();
	bool IsFloat();
	int AsInt();
	float AsFloat();

	int CompareTo(CIComparable<CStr> * pOther);
	bool Equals(CIComparable<CStr> * pOther);
	bool Equals(char * pStr);
	bool Equals(const char * pStr);
private:
	char * m_pszValue;
	int m_iCount;
	bool m_bIsTemp;
	
	static const int MAX_INT_LENGTH = 20; // probably 12, but make it safe
	static const int MAX_FLOAT_LENGTH = 20; // probably less, but make it safe
};

#endif