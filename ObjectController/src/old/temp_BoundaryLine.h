#pragma once
class BoundaryLine
{
	int _seqNo;
	float _startX;
	float _startY;
	float _endX;
	float _endY;
	float _angle;
	float _length;

	bool _isDeleted;

	BoundaryLine(void);

public:
	float getStartX(void);
	float getStartY(void);
	float getEndX(void);
	float getEndY(void);
	int getSeqNo(void);
	float get_angle(void);
	float get_length(void);

	bool isDeleted(void);
	void erase(void);

	BoundaryLine(float, float, float, float, bool);
	~BoundaryLine(void);
};