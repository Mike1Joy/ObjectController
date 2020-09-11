// Author/editor/coder: Simo
#pragma once
#ifndef ICOMPARABLE_H
#define ICOMPARABLE_H
#define NULL 0
template <class T>
class CIComparable
{
public:
	// Has to return positive, negative, or 0 if equal
	virtual int CompareTo(CIComparable<T> * pOther) = 0;
	virtual ~CIComparable(void){}
};


#endif