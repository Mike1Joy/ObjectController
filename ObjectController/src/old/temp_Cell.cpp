#include "temp_Cell.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>

//setters
void Cell::addTime(float time)
{
	_times.push_back(time);
}

void Cell::reset_num_ent(void) { _num_ent = 0; }
void Cell::pp_num_ent(void) { ++_num_ent; }

void Cell::calcMinMaxAvg(std::vector<float>* vec, float* min, float* avg)
{
	//take vector<float> and output min, max and avg of all positive numbers. If empty or all negative, gives -1.0f for all values

	if (vec->empty())
	{
		*min = -1.0f;
		*avg = -1.0f;
		return;
	}

	float _min;
	float _avg;

	int num = 0;
	for (std::vector<float>::iterator it = vec->begin(); it != vec->end(); ++it)
	{
		if (*it >= 0.0f)
		{
			if (num == 0)
			{
				_min = *it;
				_avg = *it;
			}
			else
			{
				if (*it < _min) _min = *it;
				_avg += *it;
			}

			++num;
		}
	}

	if (num == 0)
	{
		*min = -1.0f;
		*avg = -1.0f;
		return;
	}

	*min = _min;
	*avg = _avg / (num + 0.0f);
}
void Cell::setMinMaxAvgs()
{
	if (_times.empty())
	{
		_data.min_time = -1.0f;
		_data.avg_time = -1.0f;
	}
	else
	{
		calcMinMaxAvg(&_times, &_data.min_time, &_data.avg_time);
	}
}
void Cell::reset(void)
{
	_times.clear();
}
void Cell::resetALL(void)
{
	reset();
	_data.avg_time = -1.0f;
	_data.min_time = -1.0f;
}
void Cell::resetALL_pot(void)
{
	reset();
	_data.avg_time = -1.0f;
	_data.min_time = -1.0f;
}
void Cell::makeIsEmpty(bool empty)
{
	_empty = empty;
}

//getters
bool Cell::isEmpty(void)
{
	return _empty;
}
float Cell::getTime(int pos)
{
	return _times.at(pos);
}
cell_data Cell::getCellData()
{
	return _data;
}

//constructors
Cell::Cell(void)
{
	_empty = true;
	cell_data _data;
	_num_ent = 0;
	_num_invalid = 0;
}
Cell::~Cell(void) {}