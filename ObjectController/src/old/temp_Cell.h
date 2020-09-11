#pragma once
#include <vector>

struct cell_data {
	float min_time;
	float avg_time;

	cell_data(void)
	{
		min_time = -1.0f;
		avg_time = -1.0f;
	}
};

class Cell
{
	bool _empty;

	std::vector<float> _times;
	cell_data _data;
	int _num_ent;
	int _num_invalid;

	void calcMinMaxAvg(std::vector<float>*, float*, float*);

public:
	//setters
	void addTime(float);

	void reset_num_ent(void);
	void pp_num_ent(void);

	void setMinMaxAvgs();
	void reset(void);
	void resetALL(void);
	void resetALL_pot(void);
	void makeIsEmpty(bool);

	//getters
	bool isEmpty(void);
	float getTime(int);
	cell_data getCellData();

	//constructors
	Cell(void);
	~Cell(void);
};