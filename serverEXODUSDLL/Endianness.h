// Author/editor/coder: Simo
#pragma once
class CEndianness
{
public:
	static void Init();
	static short (*BigShort)(short s);
	static short (*LittleShort)(short s);
	static int (*BigInt)(int i);
	static int (*LittleInt)(int i);
	static float (*BigFloat)(float f);
	static float (*LittleFloat)(float f);

private:
	CEndianness(void);
	~CEndianness(void);

	static short ShortSwap(short s);
	static short ShortNoSwap(short s);
	static int IntSwap(int i);
	static int IntNoSwap(int i);
	static float FloatSwap(float f);
	static float FloatNoSwap(float f);

	static bool m_bIsInit;
};

