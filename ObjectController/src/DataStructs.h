#pragma once

#include <algorithm>
#define _USE_MATH_DEFINES
#include <map>
#include <set>
#include <vector>
#include <math.h>
#include <functional>
#include <optional>
#include <variant>

namespace ObjCont
{
	// static constants
	static constexpr int iNUM_ORIENT = 16;
	static constexpr float fLAYER_GAP = 360.0f / iNUM_ORIENT;
	static constexpr int iNUM_HOLO = (iNUM_ORIENT / 2) + 1;
	static constexpr int iSIM_STEP_MSEC = 83;
	static constexpr float fSIM_STEP_SEC = (float)iSIM_STEP_MSEC / 1000.0f;

	//static constexpr float fMOVE_TO_LEFT = 2.0f; // > 1 move to left, < -1 move to right, else do not prefer either
	static constexpr float RAD2DEG = 180.0f / M_PI;
	static constexpr float DEG2RAD = M_PI / 180.0f;

	// Useful datatypes
	struct Angle
	{
		void mod360()
		{
			while (f >= 360.0f)
			{
				f -= 360.0f;
			}
			while (f < 0.0f)
			{
				f += 360.0f;
			}
		}

		float f;

		//constructors
		Angle(const float& angle)
		{
			f = angle;
			mod360();
		}
		Angle()
		{
			f = 0.0f;
		}

		//operator overload
		operator float() const 
		{
			return f;
		}

		Angle operator+ (const float& other) const
		{
			return f + other;
		}

		Angle operator- (const float& other) const
		{
			return f - other;
		}

		Angle operator- (const Angle& other) const
		{
			return f - other.f;
		}

		Angle operator* (const float& other) const
		{
			return f * other;
		}

		Angle operator/ (const float& other) const
		{
			if (other != 0)
				return f / other;
			return f;
		}

		void operator+= (const float& other)
		{
			f += other;
			mod360();
		}

		void operator-= (const float& other)
		{
			f -= other;
			mod360();
		}

		void operator*= (const float& other)
		{
			f *= other;
			mod360();
		}

		void operator/= (const float& other)
		{
			if (other != 0)
			{
				f /= other;
				mod360();
			}
		}

		bool operator< (const Angle& other) const
		{
			if (f < other.f)
			{
				return true;
			}
			return false;
		}

		Angle min_diff(const Angle& other) const
		{
			Angle dif1 = f - other.f;
			Angle dif2 = other.f - f;

			return dif1 < dif2 ? dif1 : dif2;
		}

		float minus(const Angle& other) const
		{
			Angle dif = f - other.f;
			float diff = dif.f;

			return diff <= 180.0f ? diff : diff - 360.0f;
		}

		float get_f()
		{
			return f;
		}
	};

	struct Orient
	{
		int i;

		void mod()
		{
			while (i >= iNUM_ORIENT)
			{
				i -= iNUM_ORIENT;
			}
			while (i < 0)
			{
				i += iNUM_ORIENT;
			}
		}

		//constructors
		Orient(const int& angle)
		{
			i = angle;
			mod();
		}
		Orient()
		{
			i = 0;
		}

		//operator overload
		operator int() const
		{
			return i;
		}

		Orient& operator++ ()
		{
			++i;
			void mod();
			return *this;
		}

		Orient& operator-- ()
		{
			--i;
			void mod();
			return *this;
		}

		Orient operator+ (const int& other) const
		{
			return i + other;
		}

		Orient operator- (const int& other) const
		{
			return i - other;
		}

		Orient operator* (const int& other) const
		{
			return i * other;
		}

		Orient operator/ (const int& other) const
		{
			if (other != 0)
				return i / other;
			return i;
		}

		void operator+= (const int& other)
		{
			i += other;
			mod();
		}

		void operator-= (const int& other)
		{
			i -= other;
			mod();
		}

		void operator*= (const int& other)
		{
			i *= other;
			mod();
		}

		void operator/= (const int& other)
		{
			if (other != 0)
			{
				i /= other;
				mod();
			}
		}

		Orient min_diff(const Orient& other) const
		{
			Orient dif1 = i - other.i;
			Orient dif2 = other.i - i;

			return dif1.i < dif2.i ? dif1 : dif2;
		}

		bool operator== (const Orient& other) const
		{
			if (i == other.i) return true;
			return false;
		}
		bool operator== (Orient& other) const
		{
			if (i == other.i) return true;
			return false;
		}
		bool operator== (const int& other) const
		{
			if (i == other) return true;
			return false;
		}

		bool operator< (const Orient& other) const
		{
			if (i < other.i)
				return true;
			return false;
		}
		bool operator< (Orient& other) const
		{
			if (i < other.i)
				return true;
			return false;
		}
		bool operator> (const Orient& other) const
		{
			if (i > other.i)
				return true;
			return false;
		}
		bool operator> (Orient& other) const
		{
			if (i > other.i)
				return true;
			return false;
		}
	};

	static int orient_to_holo(Orient o)
	{
		int holo = o;

		if (holo >= iNUM_HOLO)
		{
			holo = iNUM_ORIENT - holo;
		}

		return holo;
	}
	static Angle orient_to_angle(Orient o)
	{
		return ((float)(int)o) * fLAYER_GAP;
	}
	static Orient angle_to_orient(Angle a)
	{
		return (int)round(a.get_f() / fLAYER_GAP);
	}
	static Orient angle_to_orient(float a)
	{
		return (int)round(a / fLAYER_GAP);
	}
	static int anglediff_to_holo(Angle a)
	{
		float f = ((float)a) + (0.5f*fLAYER_GAP);
		f /= fLAYER_GAP;
		return (int)f;
	}
	static int to_holo(Angle a, Orient o)
	{
		float fang = a.get_f();
		Angle rotate = orient_to_angle(o);

		fang -= rotate.get_f();

		while (fang > 180.0f)
		{
			fang -= 360.0f;
		}
		while (fang <= -180.0f)
		{
			fang += 360.0f;
		}

		return floor(0.5f + (fang / fLAYER_GAP));
	}
	static float angle_diff(float from, float to) // both 0 <= from, to < 360
	{
		while (from > 360.0f)
		{
			from -= 360.0f;
		}
		while (from < 0.0f)
		{
			from += 360.0f;
		}
		while (to > 360.0f)
		{
			to -= 360.0f;
		}
		while (to < 0.0f)
		{
			to += 360.0f;
		}

		float big_ = from;
		float small_ = to;
		if (from < to)
		{
			big_ = to;
			small_ = from;
		}

		float d = big_ - small_;
		if (d > 180.0f)
		{
			d = 360.0f - d;
		}
		
		return d;
	}
	static Orient EX_ang_to_orient(float ang)
	{
		while (ang <= -M_PI)
		{
			ang += M_PI * 2;
		}
		while (ang > M_PI)
		{
			ang -= M_PI * 2;
		}

		ang *= -1;
		ang -= M_PI / 2.0f;

		while (ang < 0)
		{
			ang += M_PI * 2;
		}
		while (ang >= M_PI * 2)
		{
			ang -= M_PI * 2;
		}

		ang *= RAD2DEG;

		return angle_to_orient(ang);
	}
	static float orient_to_EX_angle(Orient o)
	{
		float ang = orient_to_angle(o);

		ang *= DEG2RAD;

		while (ang <= -M_PI)
		{
			ang += M_PI * 2;
		}
		while (ang > M_PI)
		{
			ang -= M_PI * 2;
		}

		ang += M_PI / 2.0f;
		ang *= -1;

		while (ang < 0)
		{
			ang += M_PI * 2;
		}
		while (ang >= M_PI * 2)
		{
			ang -= M_PI * 2;
		}

		return ang;
	}
	static float angle_to_EX_angle(Angle a)
	{
		float ang = a;

		ang *= DEG2RAD;

		while (ang <= -M_PI)
		{
			ang += M_PI * 2;
		}
		while (ang > M_PI)
		{
			ang -= M_PI * 2;
		}

		ang += M_PI / 2.0f;
		ang *= -1;

		while (ang < 0)
		{
			ang += M_PI * 2;
		}
		while (ang >= M_PI * 2)
		{
			ang -= M_PI * 2;
		}

		return ang;
	}

	template<typename T> bool fill_if(T& data, std::variant<int, float, bool>& var) // true if failed, for use with break / continue / return
	{
		T* got = std::get_if<T>(&var);
		if (got)
		{
			data = *got;
			return false;
		}
		return true;
	}

	template<typename T> void cap(T&& min, T& var, T&& max)
	{
		if (var < min) var = min;
		else if (var > max) var = max;
	}

	template<typename T> void cap(T& var, T&& max)
	{
		if (var > max) var = max;
	}

	template<typename T> void cap(T&& min, T& var)
	{
		if (var < min) var = min;
	}

	template<typename T>
		static bool in_vector(const std::vector<T>& vec, T member)
	{
		return std::find(vec.begin(), vec.end(), member) != vec.end();
	}

	template<typename T, typename LAMBDA>
		static bool in_vector_func(const std::vector<T>& vec, LAMBDA func)
	{
		return std::find_if(vec.begin(), vec.end(), func) != vec.end();
	}
#define IN_VECTOR_FUNC(vec,ty,condition)  in_vector_func(vec, [=](ty t) -> bool {return t condition;})

	template<typename T>
		static inline T MAX(T x, T y) { return x > y ? x : y; }
	template<typename T>
		static inline T MAX(T x, T y, T z) { return MAX(MAX(x, y), z); }
	template<typename T>
		static inline T MIN(T x, T y) { return x < y ? x : y; }
	template<typename T>
		static inline T MIN(T x, T y, T z) { return MIN(MIN(x, y), z); }

	template<typename T>
		struct x_y
	{
		T x, y;
		x_y(T x_, T y_) : x(x_), y(y_) {}

		x_y<T> operator+ (const x_y<T>& other) const
		{
			return x_y(x+other.x, y+other.y);
		}

		x_y<T> operator- (const x_y<T>& other) const
		{
			return x_y(x - other.x, y - other.y);
		}

		x_y<T> operator* (const T& other) const
		{
			return x_y(x*other, y*other);
		}

		x_y<T> operator/ (const T& other) const
		{
			return x_y(x/other, y/other);
		}

		void operator+= (const x_y<T>& other)
		{
			x += other.x;
			y += other.y;
		}

		void operator-= (const x_y<T>& other)
		{
			x -= other.x;
			y -= other.y;
		}

		void operator*= (const T& other)
		{
			x *= other;
			y *= other;
		}

		void operator/= (const T& other)
		{
			x /= other;
			y /= other;
		}

		x_y() {}
	};
	
	static float distsq_x_y(x_y<float> p1, x_y<float> p2)
	{
		float x = p1.x - p2.x;
		float y = p1.y - p2.y;
		return x * x + y * y;
	};

	template<typename T>
	struct start_end
	{
		T start, end;
		start_end(T start_, T end_) : start(start_), end(end_) {}
		start_end() {}

		start_end<T> operator+ (const T& trans) const
		{
			return { start + trans,end + trans };
		}

		void operator+= (const T& other)
		{
			start += other;
			end += other;
		}

		start_end<T> get_invert() const
		{
			return { end,start };
		}

		void invert_this()
		{
			T temp_start = start;
			start = end;
			end = temp_start;
		}
	};

	static int get_largest(float f0, float f1, float f2, float f3)
	{
		std::vector<float> v = { f0,f1,f2,f3 };
		std::sort(v.begin(), v.end());

		if (v.back() == f0) return 0;
		if (v.back() == f1) return 1;
		if (v.back() == f2) return 2;
		if (v.back() == f3) return 3;

		return 0;
	}

#define REMOVE_DUPLICATES(vec) vec.resize(std::distance(vec.begin(),unique(vec.begin(),vec.end())))

	struct vector2
	{
		float x;
		float y;

		vector2(const float& speed): x(speed), y(0.0f) {}

		vector2(const x_y<float>& pos) : x(pos.x), y(pos.y) {}

		vector2() : x(0.0f), y(0.0f){}

		vector2(float x_, float y_) : x(x_), y(y_) {}

		vector2 operator+ (const vector2& other) const
		{
			return vector2(x+other.x, y+other.y);
		}

		vector2 operator- (const vector2& other) const
		{
			return vector2(x - other.x, y - other.y);
		}

		vector2 operator* (const float& other) const
		{
			return vector2(x*other, y*other);
		}

		vector2 operator/ (const float& other) const
		{
			if (other != 0)
				return vector2(x/other, y/other);
			return *this;
		}

		void operator+= (const vector2& other)
		{
			*this = *this + other;
		}

		void operator-= (const vector2& other)
		{
			*this = *this - other;
		}

		void operator*= (const float& other)
		{
			*this = *this * other;
		}

		void operator/= (const float& other)
		{
			if (other != 0)
			{
				*this = *this / other;
			}
		}

		bool operator== (const vector2& other) const
		{
			if (x == other.x && y == other.y) return true;
			return false;
		}

		operator x_y<float>() const
		{
			return { x,y };
		}

		bool operator!= (const vector2& other) const
		{
			return !(*this == other);
		}
		float magnitude_squared() const
		{
			return x*x + y*y;
		}

		float magnitude()
		{
			return sqrt(magnitude_squared());
		}

		vector2 norm()
		{
			float mag = magnitude();
			if (mag != 0.0f)
				return operator/(mag);
			return vector2(1.0f, 0.0f);
		}

		bool norm_this()
		{
			float mag = magnitude();
			if (mag != 0.0f)
			{
				x /= mag;
				y /= mag;
				return true;
			}
			else
			{
				return false;
			}
		}

		bool is_zero() const
		{
			return x == 0.0f && y == 0.0f;
		}

		void resize(float s)
		{
			if (s == 0.0f)
			{
				x = 0.0f;
				y = 0.0f;
			}
			else
			{
				norm_this();
				x *= s;
				y *= s;
			}
		}

		float dot(const vector2& other) const
		{
			return x*other.x + y*other.y;
		}

		float distsq(const vector2& other) const
		{
			return (x - other.x)*(x - other.x) + (y - other.y)*(y - other.y);
		}

		float distsq(const x_y<float>& other) const
		{
			return (x - other.x)*(x - other.x) + (y - other.y)*(y - other.y);
		}

		float angle_to_other(const vector2& other) const
		{
			return RAD2DEG*atan2(other.x - x, other.y - y);
		}

	};

	static std::pair<x_y<float>,bool> intersection_inf_lines(start_end<x_y<float>> l1, start_end<x_y<float>> l2)
	{
		float& x1 = l1.start.x;
		float& y1 = l1.start.y;

		float& x2 = l1.end.x;
		float& y2 = l1.end.y;

		float& x3 = l2.start.x;
		float& y3 = l2.start.y;

		float& x4 = l2.end.x;
		float& y4 = l2.end.y;
		
		float det = (x1 - x2)*(y3 - y4) - (y1 - y2)*(x3 - x4);
		if (det == 0.0f) return { x_y<float>(),false };

		float a = x1 * y2 - y1 * x2;
		float b = x3 * y4 - y3 * x4;

		x_y<float> xy = { a*(x3 - x4) - b * (x1 - x2),a*(y3 - y4) - b * (y1 - y2) };
		xy /= det;

		return { xy,true };
	}

	struct cnode_pos
	{
		int vec_pos;
		Orient orientation;

		cnode_pos(int vec_pos_, Orient orientation_) : vec_pos(vec_pos_), orientation(orientation_){}
		cnode_pos() {}

		int abs_pos(int layer_size) const 
		{
			return (orientation.i)*layer_size + vec_pos;
		}
		void increment_layer(bool below)
		{
			if (below)
			{
				orientation -= 1;
			}
			else
			{
				orientation += 1;
			}
		}

		bool operator== (const cnode_pos& other) const
		{
			if (vec_pos == other.vec_pos && orientation == other.orientation) return true;
			return false;
		}
		bool operator== (cnode_pos& other) const
		{
			if (vec_pos == other.vec_pos && orientation == other.orientation) return true;
			return false;
		}

		bool operator< (const cnode_pos& other) const
		{
			if (orientation < other.orientation)
				return true;
			if (orientation == other.orientation && vec_pos < other.vec_pos)
				return true;
			return false;
		}
		bool operator< (cnode_pos& other) const
		{
			if (orientation < other.orientation)
				return true;
			if (orientation == other.orientation && vec_pos < other.vec_pos)
				return true;
			return false;
		}

		bool operator> (const cnode_pos& other) const
		{
			if (orientation > other.orientation)
				return true;
			if (orientation == other.orientation && vec_pos > other.vec_pos)
				return true;
			return false;
		}
		bool operator> (cnode_pos& other) const
		{
			if (orientation > other.orientation)
				return true;
			if (orientation == other.orientation && vec_pos > other.vec_pos)
				return true;
			return false;
		}

	};

	static float distance(x_y<float> start, x_y<float> end)
	{
		return sqrt((start.x - end.x)*(start.x - end.x) + (start.y - end.y)*(start.y - end.y));
	}

	// Construction Bridges

	struct node
	{
		int id;
		x_y<float> position;
		int landing_id;
		int stair_id;
		int floor_num;
		int cnode;
		std::vector<int> tnodes;
		bool occupied_by_person;
		std::set<int> occupier_ids;

		node(int id_, float x_position_, float y_position_, int landing_id_, int stair_id, int floor_num_) : id(id_), position(x_y<float>(x_position_, y_position_)), landing_id(landing_id_), stair_id(stair_id), floor_num(floor_num_), occupied_by_person(false) {}
	};

#if 0
	struct stair_arc // from stair to node
	{
		int node_id;
		x_y<float> node_position;

		int stair_id;
		x_y<float> stair_position;
		int lane_num;
		bool bottom;

		int floor_num;

		stair_arc(
			int node_id,
			x_y<float> node_position,
			int stair_id,
			x_y<float> stair_position,
			int lane_num,
			bool bottom,
			int floor_num
		) :
			node_id(node_id),
			node_position(node_position),
			stair_id(stair_id),
			stair_position(stair_position),
			lane_num(lane_num),
			bottom(bottom),
			floor_num(floor_num)
		{}
	};
#endif

	struct arc
	{
		int land_id;
		start_end<int> stair_ids;
		start_end<int> tnode_ids;
		start_end<int> cnode_ids;
		start_end<x_y<float>> position;
		start_end<int> floor_num;
		start_end<bool> is_cnode;
		x_y<bool> start_morethan_end;
		x_y<bool> start_lessthan_end;
		bool divided;
		bool nou; // new
		bool squared;
		bool fused;
		int id;
		bool all_info;

		arc(int start_stair_, int end_stair_, int start_node_id_, int end_node_id_, float start_x_, float start_y_, float end_x_, float end_y_, int start_floor_num_, int end_floor_num_, int id_) :
			land_id(-1),
			stair_ids({ start_stair_ ,end_stair_ }),
			tnode_ids(start_end<int>(start_node_id_, end_node_id_)),
			cnode_ids(start_end<int>(-1,-1)),
			position(start_end<x_y<float>>(x_y<float>(start_x_, start_y_), x_y<float>(end_x_, end_y_))),
			floor_num(start_end<int>(start_floor_num_, end_floor_num_)),
			is_cnode(start_end<bool>(false,false)),
			start_morethan_end(x_y<bool>(start_x_>end_x_, start_y_>end_y_)),
			start_lessthan_end(x_y<bool>(start_x_<end_x_, start_y_<end_y_)),
			divided(false),
			nou(false),
			squared(false),
			fused(false),
			id(id_),
			all_info(true)
		{}
		arc(int start_node_id_, int end_node_id_, int id_) :
			tnode_ids(start_end<int>(start_node_id_, end_node_id_)),
			cnode_ids(start_end<int>(-1, -1)),
			is_cnode(start_end<bool>(false, false)),
			divided(false),
			nou(false),
			squared(false),
			fused(false),
			id(id_),
			all_info(false)
		{}
	};

	struct stair
	{
		int id;
		x_y<float> position;
		float direction; // in deg from +ve y
		int lanes, riser_num, steps;
		float length, width, height;
		int floor_num; // of bottom floor
		start_end<x_y<float>> left_wall;
		start_end<x_y<float>> right_wall;
		bool all_info;
		int node_ids_start;
		float length_mult; // turn xy dist into xyz dist

		// for adding stair_arcs in opengl
		int next_lane_top;
		int next_lane_bot;

		x_y<float> get_node_pos(int lane, int step) // step 1 is bottom and lane 1 is left (from bottom)
		{
			float l = (width * 0.5f) - (width*(lane - 0.5f) / lanes);
			float r = (length*0.5f) - (length*(step - 0.5f) / steps);

			float ang = DEG2RAD * (90.0f - direction);
			float c = cos(ang);
			float s = sin(ang);

			float x = position.x - (l*s) - (r*c);
			float y = position.y - (l*c) + (r*s);

			return { x,y };
		}

		x_y<float> get_next_pos(bool bottom)
		{
			return get_node_pos(bottom ? next_lane_bot : next_lane_top, bottom ? 1 : steps);
		}

		int get_next_lane(bool bottom)
		{
			return bottom ? next_lane_bot : next_lane_top;
		}

		void increment_next_lane(bool bottom)
		{
			if (bottom)
			{
				++next_lane_bot;
				if (next_lane_bot > lanes)
				{
					next_lane_bot = 1;
				}
			}
			else
			{
				++next_lane_top;
				if (next_lane_top > lanes)
				{
					next_lane_top = 1;
				}
			}
		}

		std::pair<int,int> get_node_id(int step, float lane) // if (first == -1) didnt work; else if (second == -1) one node; else two nodes
		{
			if (node_ids_start == -1) return { -1,-1 };

			int out = node_ids_start + step * lanes;

			float fl_lane = floor(lane);
			if (floor(lane - 0.5f) == fl_lane) // not whole (something.5)
			{
				return { out + fl_lane, out + fl_lane + 1 };
			}
			else // whole number
			{
				return { out + fl_lane, -1 };
			}
			
		}

		int get_index(int s, int l)
		{
			if (s > steps || s <= 0) return -1;
			if (l > lanes || l <= 0) return -1;
			return (s - 1) * lanes + (l - 1);
		}

		std::pair<std::vector<node>,std::vector<arc>> get_internal_tnodes_tarcs(int& first_node_id, int& first_arc_id, bool plus)
		{
			node_ids_start = first_node_id;

			std::pair<std::vector<node>, std::vector<arc>> output;

			// generate nodes
			for (int s = 1; s <= steps; ++s)
			{
				for (int l = 1; l <= lanes; ++l)
				{
					x_y<float> pos = get_node_pos(l, s);
					output.first.emplace_back(first_node_id, pos.x, pos.y, -1, id, floor_num);
					plus ? ++first_node_id : --first_node_id;
				}
			}

			// generate arcs
			for (int s = 1; s <= steps; ++s)
			{
				for (int l = 1; l <= lanes; ++l)
				{
					node& n = output.first[get_index(s,l)];

					auto lam_add_arc = [&](int index)
					{
						if (index != -1)
						{
							node& n_other = output.first[index];
							output.second.emplace_back(id, id, n.id, n_other.id, n.position.x, n.position.y, n_other.position.x, n_other.position.y, floor_num, floor_num, first_arc_id);
							plus ? ++first_arc_id : --first_arc_id;
							n.tnodes.push_back(n_other.id);
							n_other.tnodes.push_back(n.id);
						}
					};

					lam_add_arc(get_index(s + 1, l    ));
					lam_add_arc(get_index(s + 1, l + 1));
					lam_add_arc(get_index(s    , l + 1));
					lam_add_arc(get_index(s + 1, l - 1));
				}
			}

			return output;
		}

		start_end<x_y<float>> get_bottom_line(bool clockwise) // clockwise in down direction
		{
			return clockwise ? start_end<x_y<float>>(left_wall.start, right_wall.start) : start_end<x_y<float>>(right_wall.start, left_wall.start);
		}

		start_end<x_y<float>> get_top_line(bool clockwise) // clockwise in down direction
		{
			return clockwise ? start_end<x_y<float>>(left_wall.end, right_wall.end) : start_end<x_y<float>>(right_wall.end, left_wall.end);
		}

		std::vector<int> get_top_bot_nodes(bool top) const
		{
			std::vector<int> out;

			int start_id = node_ids_start + (top ? (steps - 1)*lanes : 0);
			for (int l = 0; l < lanes; ++l)
			{
				out.push_back(start_id + l);
			}

			return out;
		}

		stair(int id_, float x_position_, float y_position_, float direction_, int lanes_, int riser_num_, float length_, float width_, float height_, int floor_num_) :
			id(id_),
			position(x_y<float>(x_position_, y_position_)),
			direction(direction_),
			lanes(lanes_),
			riser_num(riser_num_),
			steps(riser_num_-1),
			length(length_),
			width(width_),
			height(height_*riser_num_/(riser_num_-2)), // fix Exodus stair height
			floor_num(floor_num_),
			next_lane_bot(1),
			next_lane_top(1),
			all_info(true),
			node_ids_start(-1)
		{
			// find walls
			float c = cos(DEG2RAD * direction);
			float s = sin(DEG2RAD * direction);

			x_y<float> hw = { 0.5f*width*c, 0.5f*width*s };
			x_y<float> hl = { -0.5f*length*s, 0.5f*length*c };

			left_wall = { position - hw + hl, position - hw - hl };
			right_wall = { position + hw + hl, position + hw - hl };

			// calc length_mult
			length_mult = sqrt(height_ * height_ + length_ * length_) / length_;

		}

	};

	struct boundary_line
	{
		start_end<x_y<float>> position;
		int floor_num;
		int id;

		boundary_line(float start_x_, float start_y_, float end_x_, float end_y_, int floor_num_, int id_) :
			position(start_end<x_y<float>>(x_y<float>(start_x_, start_y_), x_y<float>(end_x_, end_y_))),
			floor_num(floor_num_), id(id_)
		{}
	};

	struct capsule 
	{
		std::vector<x_y<float>> points;
		std::set<unsigned char> invalid_points;
		float radius;
		int default_pos;
		

		capsule(){}
		capsule(x_y<float> point, float radius): default_pos(0), radius(radius)
		{
			points.push_back(point);
		}
		capsule(start_end<x_y<float>> line, int num_points, float radius_) : radius(radius_)
		{
			default_pos = num_points / 2;
			if (num_points > 1)
			{
				x_y<float> diff = (line.end - line.start) / (num_points - 1);
				x_y<float> current = line.start;
				for (int i = 0; i < num_points; ++i)
				{
					points.push_back(current);
					current += diff;
				}
			}
		}
		capsule(start_end<x_y<float>> line, int num_points, float radius_, int default_pos_) : radius(radius_), default_pos(default_pos_)
		{
			if (default_pos < 0)
				default_pos = 0;
			else if (default_pos >= num_points)
				default_pos = num_points - 1;

			if (num_points > 1)
			{
				x_y<float> diff = (line.end - line.start) / (num_points - 1);
				x_y<float> current = line.start;
				for (int i = 0; i < num_points; ++i)
				{
					points.push_back(current);
					current += diff;
				}
			}
		}

		// make sure to do invalid_points.clear() before first test
		bool line_intersect(start_end<x_y<float>> line)
		{
			bool intersect = true;
			for (int i = 0; i < points.size(); ++i)
			{
				if (invalid_points.empty() || invalid_points.find(i) == invalid_points.end()) // not already invalid
				{
					if (line_circle_intersect(line, i))
					{
						invalid_points.insert(i);
					}
					else
					{
						intersect = false;
					}
				}	
			}
			return intersect;
		}
		bool circle_intersect(x_y<float> pos, float r, int circle)
		{
			float xdiff = pos.x - points[circle].x;
			float ydiff = pos.y - points[circle].y;

			return xdiff * xdiff + ydiff * ydiff < (r + radius)*(r + radius);
		}

		int best_valid(int current_pos)
		{
			int num = points.size();
			if (invalid_points.size() >= num)
			{
				return -1;
			}

			if (current_pos < 0 || current_pos >= num) current_pos = default_pos;

			if (current_pos == default_pos && invalid_points.find(current_pos) == invalid_points.end()) return current_pos;

			std::vector<int> valid_points;
			auto it = invalid_points.begin();
			bool end_of_invalid = it == invalid_points.end();
			for (int i = 0; i < num; ++i)
			{
				if (!end_of_invalid && *it == i)
				{
					++it;
					end_of_invalid = it == invalid_points.end();
					continue;
				}
				valid_points.push_back(i);
			}

			if (valid_points.empty()) return -1;

			int best = valid_points.front();
			int best_dist = abs(best - default_pos);

			for (int& p : valid_points)
			{
				int this_dist = abs(p - default_pos);
				if (
					this_dist < best_dist // this is closer to default than best
					||
					(this_dist == best_dist && abs(p - current_pos) < abs(best - current_pos)) // this is same distance to default than best, but is closer to current position
					)
				{
					best_dist = this_dist;
					best = p;
				}
			}
			
			return best;
		}

		float get_max_width()
		{
			float w = 0.0f;

			for (x_y<float>& v : points)
			{
				float this_w = v.x*v.x + v.y*v.y;
				if (this_w > w)
					w = this_w;
			}

			return sqrt(w) + radius;
		}

	private:
		bool line_circle_intersect(start_end<x_y<float>> line, int pos)
		{
			x_y<float> d = line.end - line.start;
			x_y<float> f = line.start - points[pos];

			float a = d.x*d.x + d.y*d.y;
			float b = 2.0f*(f.x*d.x + f.y*d.y);
			float c = f.x*f.x + f.y*f.y - radius * radius;

			float disc = b * b - (4.0f*a*c);

			if (disc <= 0.0f)
			{
				return false;
			}
			else
			{
				disc = sqrt(disc);

				float t1 = (-b - disc) / (2.0f * a);
				float t2 = (-b + disc) / (2.0f * a);

				if ((t1 >= 0.0f && t1 <= 1.0f) || (t2 >= 0.0f && t2 <= 1.0f) || (t1 < 0.0f && t2 > 1.0f))
				{
					return true;
				}
				else
				{
					return false;
				}
			}
		}
	};

	struct polygon
	{
		std::vector<x_y<float>> verticies;

		void add_vertex(float x, float y) {
			verticies.push_back(x_y<float>(x, y));
		}
		void add_vertex(x_y<float> point)
		{
			verticies.push_back(point);
		}
		void add_vertex(start_end<x_y<float>> line)
		{
			verticies.push_back(line.start);
			verticies.push_back(line.end);
		}

		void operator+= (const x_y<float>& next)
		{
			verticies.push_back(next);
		}

	private:
		bool onSegment(x_y<float> p, x_y<float> q, x_y<float> r) const
		{
			if (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) &&
				q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y))
				return true;
			return false;
		}
		int orientation(x_y<float> p, x_y<float> q, x_y<float> r) const
		{
			float val = (q.y - p.y) * (r.x - q.x) -
				(q.x - p.x) * (r.y - q.y);

			if (val == 0.0f) return 0;
			return (val > 0) ? 1 : 2;
		}
		bool doIntersect(x_y<float> p1, x_y<float> q1, x_y<float> p2, x_y<float> q2) const
		{
			int o1 = orientation(p1, q1, p2);
			int o2 = orientation(p1, q1, q2);
			int o3 = orientation(p2, q2, p1);
			int o4 = orientation(p2, q2, q1);

			if (o1 != o2 && o3 != o4)
				return true;

			//if (o1 == 0 && onSegment(p1, p2, q1)) return true;
			//if (o2 == 0 && onSegment(p1, q2, q1)) return true;
			//if (o3 == 0 && onSegment(p2, p1, q2)) return true;
			//if (o4 == 0 && onSegment(p2, q1, q2)) return true;

			return false;
		}
	public:
		bool is_point_inside(x_y<float> pos) const
		{
			int n = verticies.size();
			if (n < 3)  return false;

			x_y<float> extreme(9999.0f, pos.y);

			int count = 0, i = 0;
			do
			{
				int next = (i + 1) % n;

				if (doIntersect(verticies[i], verticies[next], pos, extreme))
				{
					if (orientation(verticies[i], pos, verticies[next]) == 0)
						return onSegment(verticies[i], pos, verticies[next]);

					count++;
				}
				i = next;
			} while (i != 0);

			return count & 1;
		}
		bool does_line_intersect(start_end<x_y<float>> pos) const
		{
			int n = verticies.size();
			if (n < 2)  return false;

			int i = 0;
			do
			{
				int next = (i + 1) % n;
				if (doIntersect(verticies[i], verticies[next], pos.start, pos.end))
					return true;
				i = next;
			} while (i != 0);
			return false;
		}
		bool does_circle_intersect(x_y<float> pos, float r) const
		{
			int n = verticies.size();
			if (n < 2)  return false;

			// does circle intersect with a line
			for (int i = 0; i < n; ++i)
			{
				int next = i + 1;

				if (next == n) next = 0;

				float p_x = verticies[i].x;
				float p_y = verticies[i].y;

				float t_x = verticies[next].x - p_x;
				float t_y = verticies[next].y - p_y;

				for (int i = 0; i <= 20; ++i)
				{
					if ((p_x - pos.x)*(p_x - pos.x) + (p_y - pos.y)*(p_y - pos.y) <= r * r)
						return true;

					p_x += t_x * 0.05f;
					p_y += t_y * 0.05f;
				}
			}

			// is circle centre inside polygon
			if (n == 2) return false;
			if (is_point_inside(pos)) return true;

			return false;
		}
		bool does_poly_intersect(const polygon& other) const
		{
			for (auto it = other.verticies.begin(); it != other.verticies.end(); ++it)
			{
				auto it_next = it + 1;
				if (it_next == other.verticies.end()) it_next = other.verticies.begin();

				if (does_line_intersect({*it,*it_next}))
				{
					return true;
				}
			}
			return false;
		}

		void rotate(Angle a) // in degrees clockwise
		{
			if (a == 0.0f) return;

			for (std::vector<x_y<float>>::iterator it = verticies.begin(); it != verticies.end(); ++it)
			{
				float cosa = cos(((float)a) * DEG2RAD);
				float sina = sin(((float)a) * DEG2RAD);

				float x = (it->x)*cosa + (it->y)*sina;
				float y = (it->y)*cosa - (it->x)*sina;

				it->x = x;
				it->y = y;
			}
		}
		void translate(x_y<float> vec)
		{
			for (std::vector<x_y<float>>::iterator it = verticies.begin(); it != verticies.end(); ++it)
			{
				it->x += vec.x;
				it->y += vec.y;
			}
		}
		void translate(vector2 vec)
		{
			for (std::vector<x_y<float>>::iterator it = verticies.begin(); it != verticies.end(); ++it)
			{
				it->x += vec.x;
				it->y += vec.y;
			}
		}

		float get_width(bool x_axis) // false = y_axis
		{
			if (verticies.size() < 2) return 0.0f;

			float min = INFINITY;
			float max = -INFINITY;
			for (const x_y<float>& v : verticies)
			{
				const float& d = x_axis ? v.x : v.y;
				if (d > max)
					max = d;
				if (d < min)
					min = d;
			}

			return max - min;
		}
		float get_max_width()
		{
			float w = 0.0f;

			for (x_y<float>& v : verticies)
			{
				float this_w = v.x*v.x + v.y*v.y;
				if (this_w > w)
					w = this_w;
			}

			return sqrt(w);
		}
	};

	struct config
	{
		float x, y, theta;

		config(float x, float y, float theta) : x(x), y(y), theta(theta) {}
		config() {};

		config operator+ (const config& other) const
		{
			return config(x + other.x, y + other.y, theta + other.theta);
		}

		config operator- (const config& other) const
		{
			return config(x - other.x, y - other.y, theta - other.theta);
		}

		config operator* (const float& other) const
		{
			return config(x*other, y*other, theta*other);
		}

		config operator/ (const float& other) const
		{
			return config(x / other, y / other, theta / other);
		}

		operator x_y<float>()
		{
			return { x,y };
		}

		x_y<float> xy()
		{
			return { x,y };
		}
	};

	struct bezier
	{
		vector2 u0, u1, d0, d1, m;
		std::vector<config> curve_up;
		std::vector<config> curve_down;

		bezier(vector2 u0, vector2 u1, vector2 d0, vector2 d1, vector2 m, int num_points)
			: u0(u0), u1(u1), d0(d0), d1(d1), m(m)
		{
			curve_up = get_curve(num_points, false);
			curve_down = get_curve(num_points, true);
		}
		bezier() {}

		void set_points(vector2 u0_, vector2 u1_, vector2 d0_, vector2 d1_, vector2 m_, int num_points)
		{
			u0 = u0_;
			u1 = u1_;
			d0 = d0_;
			d1 = d1_;
			m = m_;

			curve_up = get_curve(num_points, false);
			curve_down = get_curve(num_points, true);
		}
		std::vector<config> get_curve(int num_points, bool down)
		{
			if (num_points < 2) return std::vector<config>();

			vector2& p0 = down ? m  : u0;
			vector2& p1 = down ? d1 : u1;
			vector2& p2 = down ? d0 : m;

			float t_diff = 1.0f / (num_points-1);

			std::vector<vector2> curve1;

			for (float t1 = 0.0f; t1 <= 1.0001f; t1 += t_diff)
			{
				float t2 = 1.0f - t1;

				curve1.push_back(p0*(t2*t2) + p1 * (2.0f*t2*t1) + p2 * (t1*t1));
			}

			std::vector<config> curve2;

			curve2.push_back({ p0.x, p0.y, p0.angle_to_other(p1) });

			for (int i = 1; i < curve1.size() - 1; ++i)
			{
				curve2.push_back({curve1[i].x, curve1[i].y, curve1[i].angle_to_other(curve1[i + 1])});
			}

			curve2.push_back({ p2.x, p2.y, p1.angle_to_other(p2) });

			return curve2;
		}
		float distance_from_curve(x_y<float> xy, float theta, float l_speed, float a_speed) // time distance from curve (-1 means no curve)
		{
			if (curve_up.empty() || curve_down.empty()) return 0.0f;
			
			float l_speed_sq = l_speed * l_speed;
			float a_speed_sq = a_speed * a_speed;

			float min_dist_sq = 9999.0f;

			for (config& c : curve_up)
			{
				float a_dist = angle_diff(theta, c.theta);
				float new_dist_sq = distsq_x_y(xy, c.xy())/l_speed_sq + a_dist*a_dist/a_speed_sq;
				if (min_dist_sq > new_dist_sq)
				{
					min_dist_sq = new_dist_sq;
				}
			}
			for (config& c : curve_down)
			{
				float a_dist = angle_diff(theta, c.theta);
				float new_dist_sq = distsq_x_y(xy, c.xy()) / l_speed_sq + a_dist * a_dist / a_speed_sq;
				if (min_dist_sq > new_dist_sq)
				{
					min_dist_sq = new_dist_sq;
				}
			}

			if (min_dist_sq == 9999.0f) return 0.0f;

			return sqrt(min_dist_sq);
		}
		float distance_from_curve(x_y<float> xy) // time distance from curve (-1 means no curve)
		{
			if (curve_up.empty() || curve_down.empty()) return -1.0f;

			float min_dist_sq = 9999.0f;

			for (config& c : curve_up)
			{
				float new_dist_sq = distsq_x_y(xy, c.xy());
				if (min_dist_sq > new_dist_sq)
				{
					min_dist_sq = new_dist_sq;
				}
			}
			for (config& c : curve_down)
			{
				float new_dist_sq = distsq_x_y(xy, c.xy());
				if (min_dist_sq > new_dist_sq)
				{
					min_dist_sq = new_dist_sq;
				}
			}

			if (min_dist_sq == 9999.0f) return -1.0f;

			return sqrt(min_dist_sq);
		}
	};

	struct landing
	{
		int id;
		int floor;
		int up_stair_id; // -1 if none
		int down_stair_id; // -1 if none
		start_end<x_y<float>> up_stair_entrance; // from inside to outside
		start_end<x_y<float>> down_stair_entrance; // from inside to outside
		start_end<x_y<float>> opposite_wall; // from up to down
		bool all_info;
		std::vector<int> tnode_ids;

		x_y<float> up_normal;
		x_y<float> down_normal;

		std::map<int, bezier> prefab_bezier; // <prefab_id, bezier> 

		// methods
		bool construct_bezier(int prefab_id, float depth, float entrance, int num_points) // entrance == 0.0f -> inside; entrance == 1.0f -> outside
		{
			if (!all_info || depth < 0.0f) return false;
			if (entrance < 0.0f) entrance = 0.0f;
			else if (entrance > 1.0f) entrance = 1.0f;
			
			float entrance2 = 1.0f - entrance;

			// up
			x_y<float> u0 = up_stair_entrance.start*entrance2 + up_stair_entrance.end*entrance;

			std::pair<x_y<float>, bool> up_intersect = intersection_inf_lines({ u0,u0 + up_normal }, { opposite_wall.start - (up_normal*depth) , opposite_wall.end - (up_normal*depth) });

			if (!up_intersect.second) return false;
			x_y<float> u1 = up_intersect.first;

			// down
			x_y<float> d0 = down_stair_entrance.start*entrance2 + down_stair_entrance.end*entrance;

			std::pair<x_y<float>, bool> dw_intersect = intersection_inf_lines({ d0,d0 + down_normal }, { opposite_wall.start - (down_normal*depth) , opposite_wall.end - (down_normal*depth) });

			if (!dw_intersect.second) return false;
			x_y<float> d1 = dw_intersect.first;

			// mid
			x_y<float> m = (u1 + d1) / 2.0f;

			prefab_bezier[prefab_id] = { u0, u1, d0, d1, m, num_points };

		}
		bool construct_bezier(int prefab_id, float entrance_dist, bool from_inside, int num_points)
		{
			if (entrance_dist < 0.0f) return false;

			float x_difu = (up_stair_entrance.start.x - up_stair_entrance.end.x);
			float y_difu = (up_stair_entrance.start.y - up_stair_entrance.end.y);
			float x_difd = (down_stair_entrance.start.x - down_stair_entrance.end.x);
			float y_difd = (down_stair_entrance.start.y - down_stair_entrance.end.y);

			float dist = sqrt(std::min(x_difu * x_difu + y_difu * y_difu, x_difd * x_difd + y_difd * y_difd));

			if (entrance_dist > dist) return false;
			float entrance = entrance_dist / dist;

			if (!from_inside)
			{
				entrance = 1.0f - entrance;
			}

			float depthu = distsq_x_y(opposite_wall.start, up_stair_entrance.end);
			float depthd = distsq_x_y(opposite_wall.end, down_stair_entrance.end);

			float depth = (1.0f - entrance)*(sqrt(std::min(depthu, depthd)));

			return construct_bezier(prefab_id, depth, entrance, num_points);
		}

		void set_normals()
		{
			{
				vector2 up_norm = up_stair_entrance.end - up_stair_entrance.start;
				up_norm.norm_this();
				vector2 up_norm1 = { up_norm.y, -up_norm.x };
				vector2 up_norm2 = { -up_norm.y, up_norm.x };

				up_norm = (up_norm1 + up_stair_entrance.end).distsq(opposite_wall.start) < (up_norm2 + up_stair_entrance.end).distsq(opposite_wall.start) ? up_norm1 : up_norm2;
				up_normal = { up_norm.x, up_norm.y };
			}
			{
				vector2 dw_norm = down_stair_entrance.end - down_stair_entrance.start;
				dw_norm.norm_this();
				vector2 dw_norm1 = { dw_norm.y, -dw_norm.x };
				vector2 dw_norm2 = { -dw_norm.y, dw_norm.x };

				dw_norm = (dw_norm1 + down_stair_entrance.end).distsq(opposite_wall.end) < (dw_norm2 + down_stair_entrance.end).distsq(opposite_wall.end) ? dw_norm1 : dw_norm2;
				down_normal = { dw_norm.x, dw_norm.y };
			}
			all_info = true;
		}

		polygon get_polygon_boundary()
		{
			polygon poly;
			poly.add_vertex(up_stair_entrance);
			poly.add_vertex(opposite_wall);
			poly.add_vertex(down_stair_entrance.end);
			poly.add_vertex(down_stair_entrance.start);
			return poly;
		}

		float get_distance_to_curve(int prefab_id, x_y<float> xy, float theta, float l_speed, float a_speed, float tolarence) // (-1 -> no curve)
		{
			if (prefab_bezier.find(prefab_id) == prefab_bezier.end()) return 0.0f; // if no curve, then potential will spread normally

			float dist = prefab_bezier[prefab_id].distance_from_curve(xy, theta, l_speed, a_speed);

			if (dist < tolarence) return 0.0f;
			return dist;
		}
		float get_distance_to_curve(int prefab_id, x_y<float> xy, float theta, float l_speed, float a_speed) // default tolarence provides at least 2 nodes on each layer
		{
			//float tolarence = sqrt(0.125f / (l_speed*l_speed) + fLAYER_GAP * fLAYER_GAP / (a_speed*a_speed));
			float tolarence = 0.25f / l_speed;
			return get_distance_to_curve(prefab_id, xy, theta, l_speed, a_speed, tolarence);
		}
		float get_distance_to_curve(int prefab_id, x_y<float> xy) // (-1 -> no curve)
		{
			if (prefab_bezier.find(prefab_id) == prefab_bezier.end()) return -1.0f; // if no curve, then negative

			return prefab_bezier[prefab_id].distance_from_curve(xy);
		}

		// constructors
		landing
		(
			int id,
			int floor,
			int up_stair_id, // -1 if none
			int down_stair_id, // -1 if none
			start_end<x_y<float>> up_stair_entrance, // from inside to outside
			start_end<x_y<float>> down_stair_entrance, // from inside to outside
			start_end<x_y<float>> opposite_wall // from up to down
		) :
			id(id),
			floor(floor),
			up_stair_id(up_stair_id), // -1 if none
			down_stair_id(down_stair_id), // -1 if none
			up_stair_entrance(up_stair_entrance), // from inside to outside
			down_stair_entrance(down_stair_entrance), // from inside to outside
			opposite_wall(opposite_wall) // from up to down
		{
			set_normals();
		}

		landing
		(
			int id,
			int floor,
			int up_stair_id, // -1 if none
			int down_stair_id, // -1 if none
			start_end<x_y<float>> up_stair_entrance, // from inside to outside
			start_end<x_y<float>> down_stair_entrance // from inside to outside
		) :
			id(id),
			floor(floor),
			up_stair_id(up_stair_id), // -1 if none
			down_stair_id(down_stair_id), // -1 if none
			up_stair_entrance(up_stair_entrance), // from inside to outside
			down_stair_entrance(down_stair_entrance), // from inside to outside
			all_info(false)
		{}

		landing
		(
			int id,
			stair* s1,
			stair* s2,
			float depth
		) :
			id(id),
			up_stair_entrance(up_stair_entrance), // from inside to outside
			down_stair_entrance(down_stair_entrance) // from inside to outside
		{
			stair* s_up = s1->floor_num > s2->floor_num ? s1 : s2;
			stair* s_dw = s1->floor_num > s2->floor_num ? s2 : s1;
		
			floor = s_up->floor_num;
			up_stair_id = s_up->id;
			down_stair_id = s_dw->id;

			bool clockwise = distsq_x_y(s_up->right_wall.start, s_dw->right_wall.end) > distsq_x_y(s_up->left_wall.start, s_dw->left_wall.end);

			up_stair_entrance = s_up->get_bottom_line(clockwise);
			down_stair_entrance = s_dw->get_top_line(clockwise);

			vector2 up_norm = clockwise ? s_up->right_wall.start - s_up->right_wall.end : s_up->left_wall.start - s_up->left_wall.end;
			up_norm.norm_this();
			up_norm *= depth;
			x_y<float> up_norm_xy = { up_norm.x, up_norm.y };

			vector2 dw_norm = clockwise ? s_dw->right_wall.end - s_dw->right_wall.start : s_dw->left_wall.end - s_dw->left_wall.start;
			dw_norm.norm_this();
			dw_norm *= depth;
			x_y<float> dw_norm_xy = { dw_norm.x, dw_norm.y };

			opposite_wall = { up_stair_entrance.end + up_norm_xy , down_stair_entrance.end + dw_norm_xy };

			set_normals();
		}

		landing
		(
			int id,
			stair* s1,
			stair* s2,
			start_end<x_y<float>> opposite_wall // from up to down
		) :
			id(id),
			up_stair_entrance(up_stair_entrance), // from inside to outside
			down_stair_entrance(down_stair_entrance), // from inside to outside
			opposite_wall(opposite_wall) // from up to down
		{
			stair* s_up = s1->floor_num > s2->floor_num ? s1 : s2;
			stair* s_dw = s1->floor_num > s2->floor_num ? s2 : s1;

			floor = s_up->floor_num;
			up_stair_id = s_up->id;
			down_stair_id = s_dw->id;

			bool clockwise = distsq_x_y(s_up->right_wall.start, s_dw->right_wall.end) > distsq_x_y(s_up->left_wall.start, s_dw->left_wall.end);

			up_stair_entrance = s_up->get_bottom_line(clockwise);
			down_stair_entrance = s_dw->get_top_line(clockwise);

			set_normals();
		}
	};
	
	struct velocity_obstacle
	{
		// var
		bool valid;
		int other_ent_id;
		bool other_object;

		start_end<x_y<float>> left_line;
		start_end<x_y<float>> right_line;

		float other_vel_comp;
		vector2 this_cor_to_other;
		float dist_to_collision;
		float min_time;
		float add_cost;
		float drive_scale;
		float ok_mag_sq;
		bool within_crit_dist;
		bool zero_in_vo;
		bool reciprocal;
		bool other_not_moving;
		bool blocking_cor;

		std::map<int,float> additional_pvo_nodes;

		// misc methods
		float relative_width(vector2 displ, const std::vector<x_y<float>>& vertices)
		{
			float min = INFINITY;
			float max = -INFINITY;

			for (const x_y<float>& v : vertices)
			{
				float dist = displ.x*v.y - displ.y*v.x;
				if (dist < min) min = dist;
				if (dist > max) max = dist;
			}

			return max - min;
		}
		std::optional<float> get_time_to_collision(vector2 velocity) const
		{
			float vel_dif = other_vel_comp + this_cor_to_other.dot(velocity);
			if (vel_dif > 0.0f) // distance between them is decreasing
			{
				return dist_to_collision / vel_dif;
			}
			else
				return INFINITY;
		}
		// is point inside
		bool point_left_of_line(x_y<float> a, x_y<float> b, x_y<float> c) const  // is c left of line (a,b)
		{
			return ((b.x - a.x)*(c.y - a.y) - (b.y - a.y)*(c.x - a.x)) > 0.0f;
		}
		bool point_left_of_line(vector2 a, vector2 b, vector2 c) const  // is c left of line (a,b)
		{
			return ((b.x - a.x)*(c.y - a.y) - (b.y - a.y)*(c.x - a.x)) > 0.0f;
		}
		bool point_left_of_line(x_y<float> a, x_y<float> b, vector2 c) const  // is c left of line (a,b)
		{
			return ((b.x - a.x)*(c.y - a.y) - (b.y - a.y)*(c.x - a.x)) > 0.0f;
		}
		bool point_left_of_line(x_y<float> a, x_y<float> b, vector2 c, float& dist_to_line) const  // is c left of line (a,b)
		{
			dist_to_line = (b.x - a.x)*(c.y - a.y) - (b.y - a.y)*(c.x - a.x);
			
			if (dist_to_line < 0.0f)
			{
				dist_to_line *= -1;
				return false;
			}

			return true;
		}
		bool velocity_in_vo(vector2 v) const
		{
			if (!valid) return false;

			return (within_crit_dist || v.x*v.x + v.y*v.y >= ok_mag_sq)
				&& !point_left_of_line(left_line.start, left_line.end, v)
				&& point_left_of_line(right_line.start, right_line.end, v);
		}

		bool velocity_in_vo_col_time(vector2 v, float& time_to_collision) const  
		{
			if (!valid) return false;

			if // collision imminent
			(
				(within_crit_dist || v.x*v.x + v.y*v.y >= ok_mag_sq)
				&& !point_left_of_line(left_line.start, left_line.end, v)
				&& point_left_of_line(right_line.start, right_line.end, v)
			)
			{
				auto time = get_time_to_collision(v);
				if (time)
				{
					time_to_collision = time.value();
					return true;
				}
			}

			return false;
		}
		bool velocity_in_vo_rescale(vector2 v, float current_scale, float& rescale_factor, float min_scale) const  
		{
			if (!valid) return false;
			
			if (min_scale < drive_scale)
			{
				min_scale = drive_scale;
			}
			
			rescale_factor = 0.0f;

			if (zero_in_vo && v.is_zero())
				return true;

			v *= current_scale;
			float mag_sq = v.x*v.x + v.y*v.y;
			if // collision imminent
				(
					(within_crit_dist || mag_sq >= ok_mag_sq)
					&& !point_left_of_line(left_line.start, left_line.end, v)
					&& point_left_of_line(right_line.start, right_line.end, v)
				)
			{
				if (zero_in_vo || !reciprocal)
					return true;
				
				float s_l = 0.0f;
				float s_r = 0.0f;
				vector2 p = left_line.start;
				vector2 q = left_line.end;
				for (int i = 0; i < 2; ++i) // i = 0 try left line; i = 1 try right line
				{
					float det = v.x*(p.y - q.y) - v.y*(p.x - q.x);
					if (det != 0.0f)
					{
						float s = (q.x*p.y - q.y*p.x) / det;
						if (s > min_scale && s <= 1.0f)
						{
							(i == 0 ? s_l : s_r) = s;
						}
					}

					p = right_line.start;
					q = right_line.end;
				}

				if (s_l > min_scale || s_r > min_scale)
				{
					rescale_factor = MAX(s_l, s_r)*current_scale;
				}
				return true;
			}
			else
			{
				return false;
			}

		}
		bool velocity_in_vo_dist_to_line(vector2 v, float& dist_to_line) const
		{
			if (!valid) return false;

			float d_l = 0.0f;
			float d_r = 0.0f;
			float mag_sq = v.x*v.x + v.y*v.y;
			bool v_in_vo =
				(within_crit_dist || mag_sq >= ok_mag_sq)
				&& !point_left_of_line(left_line.start, left_line.end, v, d_l)
				&& point_left_of_line(right_line.start, right_line.end, v, d_r);

			if (v_in_vo || (zero_in_vo && v.is_zero())) // collision imminent
			{
				dist_to_line = MIN(d_l, d_r);

				if (!reciprocal)
				{
					dist_to_line *= 2;
					
					auto time = get_time_to_collision(v);

					if (time && time.value() < min_time)
					{
						dist_to_line *= min_time / time.value();
					}
				}

				if (zero_in_vo && v.is_zero()) // and if other is stopped (add_cost is 0 unless other is stopped
					dist_to_line += add_cost;

				return true;
			}
			
			return false;

		}

		// constr
		velocity_obstacle() : valid(false) {}

		velocity_obstacle
		(
			int this_id, int other_id, bool other_object, // general info
			const std::vector<x_y<float>>& this_vertices, const std::vector<x_y<float>>& other_vertices, // positions to calc collision cone from
			const std::vector<x_y<float>>& this_people, const std::vector<x_y<float>>& other_people, // positions to calc collision cone from
			float radius,
			x_y<float> this_CoR, x_y<float> other_CoR, // object CoRs
			x_y<float> this_vel, x_y<float> other_vel, // velocities
			bool this_backwards, bool other_backwards, // was the last move away from goal
			float this_acel, float other_acel, // max acels
			float this_max_speed, float other_max_speed, // max speeds
			float dist_cost, // distance cost for additional cost to VOs - object will prefer to move this distance away from goal than stay still if staying still is inside the VO
			float l_r_priority, float min_time_scale_stopped_ped,
			bool this_stopped, bool other_stopped, // are stopped
			bool this_blocking_cor, bool other_blocking_cor, // are blocking corridor
			float this_time_blocking_cor, float other_time_blocking_cor,
			float this_drive, float other_drive, // drives
			velocity_obstacle& other_vo, // other vo reference (was instantiated with default)
			float min_time_to_collision, float min_dist_to_collision, // if collision will happen in more time than this, then velo not in vo 
			bool generalised, bool hybrid, // type of VO
			std::map<int,float> additional_pvo_nodes,
			bool this_valid
		)
			: valid(this_valid), other_ent_id(other_id), other_object(other_object)
		{
			//// SETUP ////
			// sort out drives
			float tiny_drive = 0.001f;
			if (this_drive <= 0.0f) this_drive = tiny_drive;
			if (other_drive <= 0.0f) other_drive = tiny_drive;

			if (this_drive / other_drive < 0.1f) // if other drive is at least 10 times bigger, then treat as non-reciprical
			{
				this_drive = tiny_drive;
			}
			else if (other_drive / this_drive < 0.1f)
			{
				other_drive = tiny_drive;
			}

			if (this_vertices.size()*other_vertices.size() + this_vertices.size()*other_people.size()*2 + this_people.size()*other_vertices.size()*2 + this_people.size()*other_people.size()*3 < 2 ||
				(this_stopped && other_stopped)
				)
			{
				valid = false;
				return;
			}
			
			if (!this_stopped) this_stopped = (this_vel.x == 0.0f && this_vel.y == 0.0f);
			if (!other_stopped) other_stopped = (other_vel.x == 0.0f && other_vel.y == 0.0f);

			if (this_blocking_cor && other_blocking_cor)
			{
				this_blocking_cor = this_time_blocking_cor * this_drive >= other_time_blocking_cor * other_drive;
				other_blocking_cor = !this_blocking_cor;
			}

			blocking_cor = this_blocking_cor;
			other_vo.blocking_cor = other_blocking_cor;

			other_vo.additional_pvo_nodes = additional_pvo_nodes; // set rotation occ for pvo

			//// CREATE COLLISION CONES ////
			/// vector of normalised vectors
			std::vector<vector2> vectors;

			/// make vector of lines from halo to occ
			dist_to_collision = 10000.0f;
			for (const x_y<float>& t_v : this_vertices)
			{
				for (const x_y<float>& o_v : other_vertices) // this vertices X other vertices
				{
					vector2 v = o_v - t_v;

					float dist = v.magnitude();
					if (dist > 0.0f)
					{
						if (dist < dist_to_collision)
						{
							dist_to_collision = dist;
						}

						vectors.push_back(v/dist);
					}
				}

				for (const x_y<float>& o_p : other_people) // this vertices X other people
				{
					vector2 v = o_p - t_v;

					float dist = v.magnitude();
					if (dist - radius > 0.0f)
					{
						if (dist - radius < dist_to_collision)
						{
							dist_to_collision = dist - radius;
						}

						float scale = radius / sqrt(dist*dist - radius * radius);
						vector2 perp = { v.y*scale,-v.x*scale };

						float dist2 = (v + perp).magnitude();

						vectors.push_back((v + perp)/dist2);
						vectors.push_back((v - perp)/dist2);
					}
				}
			}
			for (const x_y<float>& t_p : this_people)
			{
				for (const x_y<float>& o_v : other_vertices) // this people X other vertices
				{
					vector2 v = o_v - t_p;

					float dist = v.magnitude();
					if (dist - radius > 0.0f)
					{
						if (dist - radius < dist_to_collision)
						{
							dist_to_collision = dist - radius;
						}

						float scale = radius / sqrt(dist*dist - radius * radius);
						vector2 perp = { v.y*scale,-v.x*scale };

						float dist2 = (v + perp).magnitude();

						vectors.push_back((v + perp) / dist2);
						vectors.push_back((v - perp) / dist2);
					}
				}

				for (const x_y<float>& o_p : other_people) // this people X other people
				{
					vector2 v = o_p - t_p;

					float dist = v.magnitude();
					float diam = 2 * radius;
					if (dist - diam > 0.0f)
					{
						if (dist - diam < dist_to_collision)
						{
							dist_to_collision = dist - diam;
						}

						vectors.push_back(v / dist);

						float scale = diam / sqrt(dist*dist - diam * diam);
						vector2 perp = { v.y*scale,-v.x*scale };

						float dist2 = (v + perp).magnitude();

						vectors.push_back((v + perp) / dist2);
						vectors.push_back((v - perp) / dist2);
					}
				}
			}

			other_vo.dist_to_collision = dist_to_collision;

			within_crit_dist = (dist_to_collision <= min_dist_to_collision);
			other_vo.within_crit_dist = within_crit_dist;

			if (vectors.size() < 2)
			{
				valid = false;
				return;
			}

			/// find min and max lines - two vectors whose angles differ the most (i.e. whose dot product is smallest)
			float smallest_dotp = 1.0f;
			vector2 vec1 = vectors[0];
			vector2 vec2 = vectors[1];

			for (auto v1 = vectors.begin(); v1 != vectors.end(); ++v1)
			{
				for (auto v2 = v1 + 1; v2 != vectors.end(); ++v2)
				{
					float dotp = v1->dot(*v2);
					if (dotp < smallest_dotp)
					{
						smallest_dotp = dotp;
						vec1 = *v1;
						vec2 = *v2;
					}
				}
			}
			min_time = other_drive*min_time_to_collision/(this_drive + other_drive);
			other_vo.min_time = this_drive*min_time_to_collision / (this_drive + other_drive);

			if (!other_object && other_stopped)
			{
				min_time *= min_time_scale_stopped_ped;
			}

			/// this collision cone
			bool is_2_left = point_left_of_line(0.0f, vec1, vec2); /// is vec2 to left of vec 1
			vector2& left = is_2_left ? vec2 : vec1;
			vector2& right = is_2_left ? vec1 : vec2;
			start_end<x_y<float>> this_left_line = { {0.0f,0.0f}, left };
			start_end<x_y<float>> this_right_line = { {0.0f,0.0f}, right };

			bool is_this_left_inside = point_left_of_line(this_CoR, this_CoR + this_vel, other_CoR); /// is this_left inside and this_right outside
			if (this_backwards) is_this_left_inside = !is_this_left_inside;

			start_end<x_y<float>>& this_inside_line = is_this_left_inside ? this_left_line : this_right_line;
			start_end<x_y<float>>& this_outside_line = is_this_left_inside ? this_right_line : this_left_line;

			/// other collision cone
			left *= -1;
			right *= -1;
			start_end<x_y<float>> other_left_line = { {0.0f,0.0f}, left };
			start_end<x_y<float>> other_right_line = { {0.0f,0.0f}, right };

			bool is_other_left_inside = point_left_of_line(other_CoR, other_CoR + other_vel, this_CoR); /// is other_left inside and other_right outside
			if (other_backwards) is_other_left_inside = !is_other_left_inside;

			start_end<x_y<float>>& other_inside_line = is_other_left_inside ? other_left_line : other_right_line;
			start_end<x_y<float>>& other_outside_line = is_other_left_inside ? other_right_line : other_left_line;

			//// TRANSLATE COLLISION CONES TO MAKE VOS ////

			/// if generalised, set one drive to 0 if at least one is blocking corridor or stopped
			zero_in_vo = false;
			other_vo.zero_in_vo = false;

			/// set up additional cost
			add_cost = 0.0f;
			other_vo.add_cost = 0.0f;
			other_vo.other_not_moving = false;
			other_not_moving = false;

			if (generalised)
			{
				if (this_stopped || other_stopped)
				{
					if (this_stopped && other_object)
					{
						other_drive = tiny_drive;
						other_vo.zero_in_vo = true;
						other_vo.add_cost = dist_cost * this_drive * other_acel / ((this_drive + other_drive)*other_max_speed);
						other_vo.other_not_moving = true;
					}
					if (other_stopped)
					{
						this_drive = tiny_drive;
						zero_in_vo = true;
						add_cost = dist_cost * other_drive * this_acel / ((this_drive + other_drive)*this_max_speed);
						other_not_moving = true;
					}
				}
				else if (this_blocking_cor)
				{
					other_drive = tiny_drive;
				}
				else if (other_blocking_cor)
				{
					this_drive = tiny_drive;
				}
			}

			drive_scale = this_drive / (this_drive + other_drive);
			other_vo.drive_scale = other_drive / (this_drive + other_drive);

			/// set validities and reciprocal
			if (this_valid)
				valid = other_drive > tiny_drive;

			other_vo.valid = this_drive > tiny_drive;

			reciprocal = other_vo.valid;
			other_vo.reciprocal = valid;

			/// find translations
			x_y<float> outside_trans = generalised ? (this_vel*this_drive + other_vel * other_drive) / (this_drive + other_drive) : (this_vel + other_vel)*0.5f;
			x_y<float> this_inside_trans = hybrid ? other_vel : outside_trans;
			x_y<float> other_inside_trans = hybrid ? this_vel : outside_trans;

			this_outside_line += outside_trans;
			this_inside_line += this_inside_trans;
			other_outside_line += outside_trans;
			other_inside_line += other_inside_trans;

			/// if non reciprocal, make it want to move to outside
			
			if (!reciprocal && (abs(this_vel.x) > 0.001f || abs(this_vel.y) > 0.001f))
			{
				vector2 trans = this_inside_line.end - this_outside_line.end;
				trans *= l_r_priority;

				//this_inside_line.start += trans;
				this_inside_line.end += trans;
				this_inside_line.end /= sqrt(this_inside_line.end.x*this_inside_line.end.x + this_inside_line.end.y*this_inside_line.end.y);
			}
			else if (!other_vo.reciprocal && (abs(other_vel.x) > 0.001f || abs(other_vel.y) > 0.001f))
			{
				vector2 trans = other_inside_line.end - other_outside_line.end;
				trans *= l_r_priority;

				//other_inside_line.start += trans;
				other_inside_line.end += trans;
				other_inside_line.end /= sqrt(other_inside_line.end.x*other_inside_line.end.x + other_inside_line.end.y*other_inside_line.end.y);
			}

			/// set this vo
			left_line = this_left_line;
			right_line = this_right_line;

			/// set up other_vo
			other_vo.other_ent_id = this_id;
			other_vo.other_object = true;
			other_vo.left_line = other_left_line;
			other_vo.right_line = other_right_line;

			/// find min speed that will cause a collision within min_time_to_collision
			if (min_time_to_collision <= 0.0f)
			{
				ok_mag_sq = 0.0f;
				other_vo.ok_mag_sq = 0.0f;
			}
			else
			{
				other_vo.this_cor_to_other = this_CoR - other_CoR;
				other_vo.this_cor_to_other.norm_this();

				float min_speed_collision = dist_to_collision / min_time;

				other_vel_comp = other_stopped ? 0.0f : other_vo.this_cor_to_other.dot(other_vel);

				ok_mag_sq = min_speed_collision - other_vel_comp;
				ok_mag_sq = ok_mag_sq < 0.0f ? 0.0f : ok_mag_sq * ok_mag_sq;

				this_cor_to_other = (other_vo.this_cor_to_other)*-1.0f;

				min_speed_collision = dist_to_collision / other_vo.min_time;

				other_vo.other_vel_comp = this_stopped ? 0.0f : this_cor_to_other.dot(this_vel);
				other_vo.ok_mag_sq = min_speed_collision - other_vo.other_vel_comp;
				other_vo.ok_mag_sq = other_vo.ok_mag_sq < 0.0f ? 0.0f : other_vo.ok_mag_sq*other_vo.ok_mag_sq;
			}

			/// work out if a zero velocity is in VO
			if (!zero_in_vo) zero_in_vo = velocity_in_vo(0.0f);
			if (!other_vo.zero_in_vo) other_vo.zero_in_vo = velocity_in_vo(0.0f);

		}
	};

	static std::optional<float> point_to_line_in_direction(x_y<float> p, x_y<float> a, x_y<float> b, vector2 v)
		// returns scalar s where p + sv is on line (a,b); optional has no value if no such s exists
	{
		float det = v.x * (a.y - b.y) + v.y * (b.x - a.x);
		
		if (det == 0.0f) return {};

		float t = (v.x * (p.y - b.y) + v.y * (b.x - p.x))/det;

		if (t < 0.0f || t > 1.0f) return {};

		if (v.x != 0.0f)
		{
			return (t*(a.x - b.x) + b.x - p.x) / v.x;
		}

		return (t*(a.y - b.y) + b.y - p.y) / v.y;
	}
	
	struct person
	{
		bool active;
		int id;

		// for VOs
		vector2 position;
		vector2 velocity;
		float default_speed;
		float drive;
		int floor_id;
		bool moved;
		bool stopped;
		std::vector<velocity_obstacle> pvos;

		// for enter / leave obj
		float fitness;
		bool in_object;
		int object_id;
		int node_id;

		void remove_vo(int object_id)
		{
			for (auto it = pvos.begin(); it != pvos.end(); ++it)
			{
				if (object_id == it->other_ent_id)
				{
					pvos.erase(it);
					return;
				}
			}
		}

		void add_vo(velocity_obstacle vo)
		{
			remove_vo(vo.other_ent_id);
			pvos.push_back(vo);
		}

		void enter_object(int obj_id)
		{
			if (!active) return;

			in_object = true;
			object_id = obj_id;

			pvos.clear();
			moved = false;
		}

		void leave_object(float x_pos, float y_pos, float x_vel, float y_vel, int tnode_id)
		{
			if (!active) return;

			in_object = false;
			object_id = -1;

			position.x = x_pos;
			position.y = y_pos;
			velocity.x = x_vel;
			velocity.y = y_vel;

			node_id = tnode_id;

			moved = true;
		}

		person(int id, float x_pos, float y_pos, float x_vel, float y_vel, int floor_id, float drive, float fitness, int tnode_id) :
			active(true), id(id), position(x_pos, y_pos), velocity(x_vel, y_vel), floor_id(floor_id), moved(true), drive(drive),
			fitness(MAX(0.0f, MIN(1.0f, fitness / 5.0f))), // as default fitness in EXODUS is 5.0. Anything more than this will still move at max speed
			in_object(false), object_id(-1), node_id(tnode_id), stopped(true), default_speed(1.35f)
		{
			
		}

		person() :
			active(false), stopped(true)
		{}
	};

	struct attachment_point
	{
		int id;
		capsule occupation_circles;
		int occupied_circle;
		person occupant;
		
		float attachment_time;
		int required_attendants;
		bool is_driver;
		bool inside;

		std::set<unsigned char> invalid_pos;

		// constructor
		attachment_point(int id_, start_end<x_y<float>> line, int num_points, float radius, float attachment_time_, int required_attendants_, bool is_driver_) :
			id(id_),
			occupation_circles(line, num_points, radius),
			attachment_time(attachment_time_),
			required_attendants(required_attendants_),
			is_driver(is_driver_),
			occupant(),
			inside(false)
		{
			occupied_circle = -1;
		}
		attachment_point(int id_, start_end<x_y<float>> line, int num_points, float radius, int default_occ, float attachment_time_, int required_attendants_, bool is_driver_) :
			id(id_),
			occupation_circles(line, num_points, radius, default_occ),
			attachment_time(attachment_time_),
			required_attendants(required_attendants_),
			is_driver(is_driver_),
			occupant(),
			inside(false)
		{
			occupied_circle = -1;
		}
		attachment_point(int id, x_y<float> point, float radius, float attachment_time, int required_attendants, bool is_driver) :
			id(id),
			occupation_circles(point, radius),
			attachment_time(attachment_time),
			required_attendants(required_attendants),
			is_driver(is_driver),
			occupant(),
			inside(true)
		{
			occupied_circle = -1;
		}

		bool can_move()
		{
			if (is_driver && !occupant.active)
			{
				return false;
			}
			return true;
		}
		void rotate(Angle a) // in degrees clockwise
		{
			if (a == 0.0f) return;

			float cosa = cos(((float)a) * DEG2RAD);
			float sina = sin(((float)a) * DEG2RAD);

			for (std::vector<x_y<float>>::iterator it = occupation_circles.points.begin(); it != occupation_circles.points.end(); ++it)
			{
				float x = (it->x)*cosa + (it->y)*sina;
				float y = (it->y)*cosa - (it->x)*sina;

				it->x = x;
				it->y = y;
			}
		}
		void translate(x_y<float> vec)
		{
			for (std::vector<x_y<float>>::iterator it = occupation_circles.points.begin(); it != occupation_circles.points.end(); ++it)
			{
				it->x += vec.x;
				it->y += vec.y;
			}
		}
		void translate(vector2 vec)
		{
			for (std::vector<x_y<float>>::iterator it = occupation_circles.points.begin(); it != occupation_circles.points.end(); ++it)
			{
				it->x += vec.x;
				it->y += vec.y;
			}
		}
		void attach(person &p)
		{
			occupied_circle = inside ? 0 : occupation_circles.best_valid(occupied_circle);
			if (occupied_circle >= 0)
			{
				occupant = p;
			}
		}
		void attach(person &p, int pos)
		{
			if (pos < occupation_circles.points.size())
			{
				occupied_circle = inside ? 0 : pos;
				if (occupied_circle >= 0)
				{
					occupant = p;
				}
			}
		}
		void move_occ(std::set<unsigned char> invalid, bool interpolate)
		{
			if (inside) return;
			if (occupant.active)
			{
				if (interpolate)
				{
					occupation_circles.invalid_points = invalid_pos;
					invalid_pos = invalid;
				}
				else
				{
					occupation_circles.invalid_points = invalid;
				}
				occupied_circle = occupation_circles.best_valid(occupied_circle);
			}
		}
		x_y<float> get_occupant_pos() const 
		{
			if (occupied_circle > 0 && occupied_circle < occupation_circles.points.size())
				return occupation_circles.points[occupied_circle];
			else
				return occupation_circles.points[0];
		}

		void detatch()
		{
			occupant = person();
			occupied_circle = -1;
		}

		bool does_circle_intersect(x_y<float> pos, float r)
		{
			if (!occupant.active || occupied_circle < 0 || occupied_circle >= occupation_circles.points.size()) return false;

			return occupation_circles.circle_intersect( pos, r, occupied_circle);
		}
	};

	static void relative_to_object(Angle orientation, vector2 position, polygon& poly, std::vector<attachment_point>& attachment_points)
	{
		//rotate
		if (orientation != 0)
		{
			poly.rotate(orientation);
			for (std::vector<attachment_point>::iterator it = attachment_points.begin(); it != attachment_points.end(); ++it)
			{
				it->rotate(orientation);
			}
		}

		//translate
		poly.translate(position);
		for (std::vector<attachment_point>::iterator it = attachment_points.begin(); it != attachment_points.end(); ++it)
		{
			it->translate(position);
		}
	}

	struct object_prefab
	{
		int object_prefab_id;
		polygon polygon;
		std::vector<attachment_point> attachment_points;
		float max_linear_speed;
		float max_stair_dec_speed;
		float max_stair_acc_speed;
		float max_angular_speed;
		float max_acceleration;
		float holonomicity[iNUM_HOLO]; // 
		bool turn_on_spot;
		bool stair_up;
		bool stair_down;
		bool stair_side;
		float landing_entrance; // distance from edge of stairs
		bool landing_inside; // ditance from inside or outside
		float integral_drive;

		object_prefab(int object_prefab_id_, float max_linear_speed_, float max_angular_speed_, float max_stair_dec_speed, float max_stair_acc_speed, float max_acceleration_, float holonomicity_[iNUM_HOLO], bool turn_on_spot_, bool stair_up, bool stair_down, bool stair_side, float landing_entrance, bool landing_inside, float integral_drive) :
			object_prefab_id(object_prefab_id_),
			max_linear_speed(max_linear_speed_),
			max_stair_dec_speed(max_stair_dec_speed),
			max_stair_acc_speed(max_stair_acc_speed),
			max_angular_speed(max_angular_speed_),
			max_acceleration(max_acceleration_),
			turn_on_spot(turn_on_spot_),
			stair_up(stair_up),
			stair_down(stair_down),
			stair_side(stair_side),
			landing_entrance(landing_entrance),
			landing_inside(landing_inside),
			integral_drive(integral_drive)
		{
			for (int i = 0; i < iNUM_HOLO; ++i)
			{
				holonomicity[i] = holonomicity_[i];

				// clamped between 0 and 1
				holonomicity[i] = holonomicity[i] < 0.0f ? 0.0f : holonomicity[i] > 1.0f ? 1.0f : holonomicity[i];
			}
		}

		// methods
		void add_vertex(float x, float y) {
			polygon.add_vertex(x,y);
		}
		void add_attachment_point(int id_, start_end<x_y<float>> line, int num_points, float radius, float attachment_time_, int required_attendants_, bool is_driver_) {
			attachment_points.push_back(
				attachment_point(id_, line, num_points, radius, attachment_time_, required_attendants_,is_driver_)
			);
		}
		void add_attachment_point(int id_, start_end<x_y<float>> line, int num_points, float radius, int default_occ, float attachment_time_, int required_attendants_, bool is_driver_) {
			attachment_points.push_back(
				attachment_point(id_, line, num_points, radius, default_occ, attachment_time_, required_attendants_, is_driver_)
			);
		}

	};

	struct occupied_attachment
	{
		int point_id;
		int occupant_id;

		occupied_attachment(int point_id_, int occupant_id_) : point_id(point_id_), occupant_id(occupant_id_) {}
	};

	struct object_instance
	{
		int object_id;
		int object_prefab_id;
		int start_node_id;
		int start_floor_num;
		Orient start_orientation;
		vector2 start_position;
		std::vector<std::pair<int,person>> occupied_attachment_points;

		object_instance(int object_id_, int object_prefab_id_, int start_node_id_, int start_floor_num_, int start_orientation_, float start_x, float start_y) :
			object_id(object_id_),
			object_prefab_id(object_prefab_id_),
			start_node_id(start_node_id_),
			start_floor_num(start_floor_num_),
			start_orientation(start_orientation_),
			start_position(start_x, start_y)
		{}

		// Methods
		void occupy_attachment_point(int point_id, person occupant)
		{
			occupied_attachment_points.push_back(std::pair<int, person>(point_id, occupant));
		}
		void vacate_attachment_point(int point_id) {
			for (std::vector<std::pair<int, person>>::iterator it = occupied_attachment_points.begin(); it != occupied_attachment_points.end(); ++it)
			{
				if (it->first = point_id)
				{
					occupied_attachment_points.erase(it);
					return;
				}
			}
			return;
		}
	};

	struct occupation_seed
	{
		int object_id;
		int floor_num;
		int stair_id;
		int node_id;
		vector2 position;
		Angle orientation;
		polygon polygon;
		std::vector<attachment_point> attachment_points;
		std::vector<velocity_obstacle> vos;
		vector2 velocity;

		void relative_to_object()
		{
			//rotate
			if (orientation != 0)
			{
				polygon.rotate(orientation);
				for (std::vector<attachment_point>::iterator it = attachment_points.begin(); it != attachment_points.end(); ++it)
				{
					it->rotate(orientation);
				}
			}

			//translate
			polygon.translate(position);
			for (std::vector<attachment_point>::iterator it = attachment_points.begin(); it != attachment_points.end(); ++it)
			{
				it->translate(position);
			}
		}
		
	};

	struct occupation_tree
	{
		int object_id;
		bool succeed;
		std::vector<x_y<float>> halo_nodes;
		bool connected_halo;
	};

	enum stair_arc_direction : unsigned char { NOT_STAIR_ARC = 0U, STAIR_ARC_UP, STAIR_ARC_DOWN, STAIR_ARC_SIDEUP, STAIR_ARC_SIDEDOWN, STAIR_ARC_SIDE }; // NOT_STAIR_ARC is false, others are true

	static const char* stair_arc_dir_str(unsigned char dir)
	{
		switch (dir)
		{
		case NOT_STAIR_ARC: return "NOT_STAIR_ARC";
		case STAIR_ARC_UP: return "STAIR_ARC_UP";
		case STAIR_ARC_DOWN: return "STAIR_ARC_DOWN";
		case STAIR_ARC_SIDEUP: return "STAIR_ARC_SIDEUP";
		case STAIR_ARC_SIDEDOWN: return "STAIR_ARC_SIDEDOWN";
		case STAIR_ARC_SIDE: return "STAIR_ARC_SIDE";
		default: return "";
		}
	}

	static void invert_this_stair_type(unsigned char& stair_type)
	{
		switch (stair_type)
		{
		case STAIR_ARC_UP:
			stair_type = STAIR_ARC_DOWN;
			return;
		case STAIR_ARC_DOWN:
			stair_type = STAIR_ARC_UP;
			return;
		case STAIR_ARC_SIDEUP:
			stair_type = STAIR_ARC_SIDEDOWN;
			return;
		case STAIR_ARC_SIDEDOWN:
			stair_type = STAIR_ARC_SIDEUP;
			return;
		default: // NOT_STAIR_ARC || STAIR_ARC_SIDE || invalid
			return;
		}
	}

	static unsigned char invert_stair_type(unsigned char stair_type)
	{
		switch (stair_type)
		{
		case STAIR_ARC_UP:
			return STAIR_ARC_DOWN;
		case STAIR_ARC_DOWN:
			return STAIR_ARC_UP;
		case STAIR_ARC_SIDEUP:
			return STAIR_ARC_SIDEDOWN;
		case STAIR_ARC_SIDEDOWN:
			return STAIR_ARC_SIDEUP;
		case STAIR_ARC_SIDE:
			return STAIR_ARC_SIDE;
		default: // NOT_STAIR_ARC || invalid
			return NOT_STAIR_ARC;
		}
	}

	enum item : unsigned char { EMPTY, GO_TO, WAIT, REMOVE, PICKUP };
	struct task 
	{
		int priority;
		item type;

		std::vector<cnode_pos> cnodes;
		int person_id; // if -ve, then pick up closest person. If +ve, then pick up that person id

		bool started;
		float delay;
		bool STOP;

		task(int _priority, std::vector<cnode_pos> _cnodes) : priority(_priority), cnodes(_cnodes), type(GO_TO), started(false), STOP(false) {}
		task(int _priority, float _delay) : priority(_priority), delay(_delay), type(WAIT), started(false), STOP(false) {}
		task(int _priority) : priority(_priority), type(REMOVE), started(false), STOP(false) {}
		task(int _priority, int _person_id) : priority(_priority), person_id(_person_id), type(PICKUP), started(false), STOP(false) {}

		task() : type(EMPTY), STOP(false) {}

		void make_empty()
		{
			priority = -1;
			type = EMPTY;
			cnodes.clear();
			started = false;
			delay = 0.0f;
			STOP = false;
		}
		void replace(std::set<task>::iterator it)
		{
			priority = it->priority;
			type = it->type;
			cnodes = it->cnodes;
			delay = it->delay;
			started = it->started;
			person_id = it->person_id;
			STOP = false;

		}

		bool operator< (const task& other) const
		{
			if (priority < other.priority)
				return true;
			return false;
		}
		bool operator< (task& other) const
		{
			if (priority < other.priority)
				return true;
			return false;
		}
		bool operator> (const task& other) const
		{
			if (priority > other.priority)
				return true;
			return false;
		}
		bool operator> (task& other) const
		{
			if (priority > other.priority)
				return true;
			return false;
		}
	};

	struct occ_nodes
	{
		bool valid;
		std::vector<int> occupied_nodes;
		bool connected_halo;
	};

	struct potential_move
	{
		// node info
		cnode_pos node;
		vector2 new_position;
		vector2 old_position;
		float displacement;
		vector2 velocity;
		float max_lin_vel;
		float max_acel;
		bool trans;
		bool rot;
		bool not_move;
		int occ_nodes_vec_pos;

		// ranking
		float potential;
		float weight;
		float wait;
		float move_time;
		float VO_cost;
		float add_cost;
		bool in_VO;
		float holo;
		bool valid; // does not collide and has valid variables for comparison
		unsigned char stair_dir;
		float stair_len_mult;
		bool towards_goal; // new pot < old pot

		//////////////////////////////

		// setting
		void set_valid()
		{
			valid = (trans || rot || not_move) && node.vec_pos >= 0 && holo > 0.0f && displacement >= 0.0f;
		}

		void set_VO_cost(const std::vector<velocity_obstacle>& vos, float min_scale)
		{
			if (!valid)
			{
				set_valid();
				if (!valid) return;
			}

			in_VO = false;
			if (vos.empty()) return;

			bool rescale = trans;
			float rescale_factor = 1.0f;
			int skip = -1;
			for (int i = 0; i < vos.size(); ++i)
			{
				// if collision would occur after target is reached, then skip vo
				if (i == skip || !vos[i].valid || (vos[i].dist_to_collision >= ((max_lin_vel + vos[i].other_vel_comp) * potential))) continue;

				if (rescale)
				{
					float this_factor = 0.0f;
					if (vos[i].velocity_in_vo_rescale(velocity, rescale_factor, this_factor, min_scale))
					{
						if (this_factor <= 0.0f)
						{
							rescale = false;
							rescale_factor = 1.0f;
							skip = -1;
							i = -1;
							continue;
						}
						else if (this_factor < rescale_factor)
						{
							rescale_factor = this_factor;
							skip = i;
							i = -1;
							continue;
						}
					}
				}
				else
				{
					float this_cost = 0.0f;
					if (vos[i].velocity_in_vo_dist_to_line(velocity, this_cost));
					{
						VO_cost += (this_cost / max_acel);
						in_VO = true;
					}
				}
			}

			if (rescale)
			{
				velocity *= rescale_factor;
				wait /= rescale_factor;
			}
		}

		void set_weight(float max_lin_vel, float max_ang_vel, float seconds)
		{
			if (trans)
			{
				float t_t = displacement / (max_lin_vel);
				if (rot)
				{
					// trans and rot
					float t_a = fLAYER_GAP / max_ang_vel;
					weight = sqrt(t_t*t_t + t_a*t_a);
					return;
				}
				// trans and !rot
				weight = t_t;
				return;
			}
			if (rot)
			{
				// !trans and rot
				weight = fLAYER_GAP / max_ang_vel;
				return;
			}

			// !trans and !rot
			weight = seconds;
		}

		// constructor
		potential_move() : node(0,0), new_position(0.0f), displacement(0.0f), velocity(0.0f), trans(false), rot(false), potential(0.0f), weight(0.0f), holo(0.0f), stair_dir(NOT_STAIR_ARC), stair_len_mult(1.0f), not_move(false), valid(false), move_time(0.0f) {}

		potential_move(cnode_pos node, vector2 new_position, float displacement, vector2 old_position, bool trans, bool rot, bool not_move, float potential, float old_potential, float holo, unsigned char stair_dir, float stair_len_mult_, float max_lin_vel, float max_ang_vel, float max_acel, float seconds)
			: node(node), new_position(new_position), displacement(displacement), old_position(old_position), trans(trans), rot(rot), potential(potential), max_lin_vel(max_lin_vel), holo(holo), stair_dir(stair_dir), not_move(not_move), max_acel(max_acel), VO_cost(0.0f), add_cost(0.0f)
		{
			// set stair length multiplier
			switch (stair_dir)
			{
			case NOT_STAIR_ARC:
			case STAIR_ARC_SIDE:
				stair_len_mult = 1.0f;
				break;
			case STAIR_ARC_UP:
			case STAIR_ARC_DOWN:
				stair_len_mult = stair_len_mult_;
				break;
			case STAIR_ARC_SIDEUP:
			case STAIR_ARC_SIDEDOWN:
				stair_len_mult = 0.5f * (1.0f + stair_len_mult_);
				break;
			}

			// set weight
			set_weight(max_lin_vel*holo, max_ang_vel, seconds);
			wait = weight;
			move_time = weight;

			towards_goal = potential < old_potential;

			velocity = (weight <= seconds ? 0.0f : ((new_position - old_position) / weight));

			set_valid();
		}

		// comparisons and ranking (<) behaviour
		inline float comparison() const
		{
			return potential + weight + VO_cost + add_cost;
		}

		inline float comparison_not_move() const
		{
			return potential + VO_cost + add_cost;
		}

		bool operator<(const potential_move& other) const // if this move is more desirable than other move, return true
		{
			// put "not move" at end
			if (not_move) return false;
			if (other.not_move) return true;

			// valid test
			if (!valid && other.valid) return false;
			if (!other.valid && valid) return true;

			return comparison() < other.comparison();
		}

		// comparison (auto - DO NOT CHANGE)
		bool operator>(const potential_move& other) const
		{
			return other.operator<(*this);
		}
		bool operator==(const potential_move& other) const
		{
			return node == other.node;
		}

	};

	struct onion_config
	{
		int id;

		float time;

		vector2 position;
		Angle angle;
		std::pair<int, int> floors;
		int stair_id;

		std::pair<vector2, float> velocity;

		polygon poly;
		std::vector<attachment_point> att_points;

		bool draw_me;

		onion_config(int id, float time, vector2 position, Angle angle, std::pair<int, int> floors, int stair_id, std::pair<vector2, float> velocity, const polygon& poly, const std::vector<attachment_point>& att_points) :
			id(id),
			time(time),
			position(position),
			angle(angle),
			floors(floors),
			stair_id(stair_id),
			velocity(velocity),
			poly(poly),
			att_points(att_points),
			draw_me(false)
		{}
	};
}


#include "NetworkServerHandler.h"
#include "NetworkClientHandler.h"

///////////////////////////////////////////////
// sim data
///////////////////////////////////////////////
namespace data_for_TCP
{
	class msg_element
	{
		union
		{
			int i;
			float f;
		};

		msg_element();

	public:
		bool is_int;

		msg_element(float fl)
		{
			f = fl;
			is_int = false;
		}
		msg_element(int in)
		{
			i = in;
			is_int = true;
		}

		operator int() const
		{
			return i;
		}
		operator float() const
		{
			return f;
		}
	};

	class object_config
	{
		int prefab_id;
		float speed;

	public:
		int id;
		int floor;
		float x;
		float y;
		float angle;
		int task_complete;
		bool deleted;
		
		std::vector<msg_element> get_content() const
		{
			//EXMS_MOVEOBJECT(10520)
			//      <iObjectId><iObjectType><iLevel><fxpos><fypos><fzpos><frotation><fspeed>
			if (deleted)
			{
				return { id };
			}
			else
			{
				return { id,      prefab_id,  floor,    x,     y,   0.0f,    angle,   speed , task_complete };
			}
		}

		object_config
		(
			int id,
			int prefab_id,
			int floor,
			float x,
			float y,
			float angle,
			float speed,
			int task_complete
		) :
			id(id),
			prefab_id(prefab_id),
			floor(floor),
			x(x),
			y(y),
			angle(angle),
			speed(speed),
			task_complete(task_complete),
			deleted(false)
		{}

		object_config // deleted object
		(
			int id,
			bool deleted
		) :
			id(id),
			deleted(deleted)
		{}
	};

	class occupied_nodes
	{
		//EXMS_NODEOBJECT(10524)<uObjectID><uNodeCount>(<uNodeID1>,...)

	public:

		int object_id;
		std::vector<int> node_ids;

		std::vector<msg_element> get_content() const
		{
			std::vector<msg_element> content = { object_id, (int)node_ids.size() };
			content.insert(content.end(), node_ids.begin(), node_ids.end());
			return content;
		}

		occupied_nodes
		(
			int object_id,
			std::vector<int> _node_ids
		) :
			object_id(object_id)
		{
			REMOVE_DUPLICATES(_node_ids);
			node_ids = _node_ids;
		}
	};

	class pvo
	{
		//EXMS_VELOCITYOBSTACLE(10526)<uPersonID><uNodeCount>(<uNodeID1><fCost1>,...)

	public:
		int person_id;
		std::map<int, std::map<int, float>> node_obj_cost; // node_obj_cost.at(node).at(obj) = cost
		float additional_cost;

		std::vector<msg_element> get_content() const
		{
			std::vector<msg_element> content = { person_id, (int)node_obj_cost.size() };
			for (auto& noc : node_obj_cost)
			{
				content.push_back((int)noc.first);

				float cost = 0.0f;
				for (auto& oc : noc.second)
				{
					cost += oc.second;
				}
				content.push_back(cost + additional_cost);
			}
			return content;
		}

		inline void add_node_obj_cost(int node, int object, float cost)
		{
			(node_obj_cost[node])[object] = cost;
		}

		float total_cost_for_node(int node)
		{
			auto it = node_obj_cost.find(node);

			if (it == node_obj_cost.end()) return 0.0f;

			float cost = 0.0f;
			for (auto& oc : it->second)
			{
				cost += oc.second;
			}
			return cost;
		}

		pvo
		(
			int person_id,
			std::map<int, std::map<int, float>> node_obj_cost,
			float additional_cost
		) :
			person_id(person_id),
			node_obj_cost(node_obj_cost),
			additional_cost(additional_cost)
		{}
		
	};

	class stair_node
	{
		int node_id;
		int stair_id;
		int lane;
		int step;

	public:

		std::vector<msg_element> get_content() const
		{
			// <iNodeID><iStairID><iLane><iStep>
			return { node_id,stair_id,lane,step };
		}

		stair_node
		(
			int node_id,
			int stair_id,
			int lane,
			int step
		) :
			node_id(node_id),
			stair_id(stair_id),
			lane(lane),
			step(step)
		{}
	};

	class attach_people
	{
		//EXMS_ATTACHOBJECTPERSON(10540)<iObjectId><iPersonId><iLocation>

	public:
		int object_id;
		int person_id;
		int point_id; // aka location

		std::vector<msg_element> get_content() const
		{
			return { object_id, person_id, point_id, };
		}

		attach_people
		(
			int object_id,
			int person_id,
			int point_id
		) :
			object_id(object_id),
			person_id(person_id),
			point_id(point_id)
		{}

	};

	static std::vector<stair_node> get_stair_nodes(const ObjCont::stair& st)
	{
		if (st.node_ids_start == -1) return {};

		std::vector<stair_node> out;

		for (int l = 0; l < st.lanes; ++l)
		{
			for (int s = 0; s < st.steps; ++s)
			{
				// if s=0 and l=0 is bottom left (as you walk up)
				out.push_back(stair_node(st.node_ids_start + l + (s * st.lanes), st.id, l, s));
			}
		}

		return out;
	}

	struct all_sim_data
	{
		std::vector<object_config> configs;
		std::vector<occupied_nodes> occ_nodes;
		std::vector<pvo> vos;

		all_sim_data() {}

		all_sim_data
		(
			std::vector<object_config> configs,
			std::vector<occupied_nodes> occ_nodes,
			std::vector<pvo> vos
		) :
			configs(configs),
			occ_nodes(occ_nodes),
			vos(vos)
		{}
	};

	struct msg_str
	{
		const char* s_code;
		std::vector<bool> is_int;
		std::vector<bool> rep_pattern;
		bool add_pattern;
		int num_el;

		msg_str
		(
			const char* s_code,
			std::vector<bool> is_int
		) : s_code(s_code), is_int(is_int), num_el(is_int.size()), add_pattern(false)
		{}
		msg_str
		(
			const char* s_code,
			std::vector<bool> is_int,
			std::vector<bool> rep_pattern
		) : s_code(s_code), is_int(is_int), num_el(is_int.size()), rep_pattern(rep_pattern), add_pattern(rep_pattern.size() > 0)
		{}

		const char* to_str(CNetworkMessage* msg, int index)
		{
			char buf[12];
			if (index < is_int.size())
			{
				if (is_int[index])
				{
					sprintf(buf, "%d", msg->GetInt(index * 4));
				}
				else
				{
					sprintf(buf, "%f", msg->GetFloatLittle(index * 4));
				}
			}
			return buf;
		}

	msg_str() : s_code("***NO_MESSAGE***"), num_el(0) {}
	};
}