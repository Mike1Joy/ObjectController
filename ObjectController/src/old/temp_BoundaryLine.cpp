#include "temp_BoundaryLine.h"
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>

float BoundaryLine::getStartX(void) { return _startX; }
float BoundaryLine::getStartY(void) { return _startY; }
float BoundaryLine::getEndX(void) { return _endX; }
float BoundaryLine::getEndY(void) { return _endY; }
int BoundaryLine::getSeqNo(void) { return _seqNo; }
float BoundaryLine::get_angle(void) { return _angle; }
float BoundaryLine::get_length(void) { return _length; }

bool BoundaryLine::isDeleted(void) { return _isDeleted; }
void BoundaryLine::erase(void) { _isDeleted = true; }

BoundaryLine::BoundaryLine(float startX, float startY, float endX, float endY, bool isDeleted)
{
	static int id = 0; _seqNo = id++;

	_startX = startX;
	_startY = startY;
	_endX = endX;
	_endY = endY;

	float xdiff = endX - startX;
	float ydiff = endY - startY;;

	_length = sqrt((xdiff*xdiff) + (ydiff*ydiff));

	_angle = ydiff == 0.0f ? 90.0f : atan(xdiff / ydiff)*(180 / M_PI);

	_isDeleted = isDeleted;

	//printf("bound const %f\n", _angle);
}

BoundaryLine::~BoundaryLine(void)
{
	//printf("bound dest %i\n", _seqNo);
}
