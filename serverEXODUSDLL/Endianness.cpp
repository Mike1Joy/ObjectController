// Author/editor/coder: Simo
#include "Endianness.h"


bool CEndianness::m_bIsInit = false;
CEndianness::CEndianness(void)
{
}


CEndianness::~CEndianness(void)
{
}
short (*CEndianness::BigShort)(short s);
short (*CEndianness::LittleShort)(short s);
int (*CEndianness::BigInt)(int i);
int (*CEndianness::LittleInt)(int i);
float (*CEndianness::BigFloat)(float f);
float (*CEndianness::LittleFloat)(float f);

void 
CEndianness::Init()
{
	if(m_bIsInit)
	{
		return;
	}
	char SwapTest[2] = { 1, 0 };
	if(*(short *) SwapTest == 1)
	{
		// little endian
		BigShort = ShortSwap;
		LittleShort = ShortNoSwap;
		BigInt = IntSwap;
		LittleInt = IntNoSwap;
		BigFloat = FloatSwap;
		LittleFloat = FloatNoSwap;
	}
	else
	{
		// big endian
		BigShort = ShortNoSwap;
		LittleShort = ShortSwap;
		BigInt = IntNoSwap;
		LittleInt = IntSwap;
		BigFloat = FloatNoSwap;
		LittleFloat = FloatSwap;
	}
	m_bIsInit = true;
}
short 
CEndianness::ShortSwap(short s)
{
	unsigned char b1, b2;
	
	b1 = s & 255;
	b2 = (s >> 8) & 255;

	return (b1 << 8) + b2;
}
short 
CEndianness::ShortNoSwap(short s)
{
	return s;
}
int 
CEndianness::IntSwap(int i)
{
	unsigned char b1, b2, b3, b4;

	b1 = i & 255;
	b2 = (i >> 8) & 255;
	b3 = (i >> 16) & 255;
	b4 = (i >> 24) & 255;

	return ((int)b1 << 24) + ((int)b2 << 16) + ((int)b3 << 8) + b4;
}
int 
CEndianness::IntNoSwap(int i)
{
	return i;
}
float 
CEndianness::FloatSwap(float f)
{
	union
	{
		float f;
		unsigned char b[4];
	} dat1, dat2;

	dat1.f = f;
	dat2.b[0] = dat1.b[3];
	dat2.b[1] = dat1.b[2];
	dat2.b[2] = dat1.b[1];
	dat2.b[3] = dat1.b[0];

	return dat2.f;
}
float 
CEndianness::FloatNoSwap(float f)
{
	return f;
}