#include "Mask.h"
#include <algorithm>
#include <iterator>

//todo много лишних вычислений
namespace mask
{
	/*
	|-2,2	|-1,2	|0,2	|1,2	|2,2	|
	|-2,1	|-1,1	|0,1	|1,1	|2,1	|
	|-2,0	|-1,0	|0,0	|1,0	|2,0	|
	|-2,-1	|-1,-1	|0,-1	|1,-1	|2,-1	|
	|-2,-2	|-1,-2	|0,-2	|1,-2	|2,-2	|
	*/

	Mask diff(const Mask& _source, const Mask& _substract)
	{
		Mask diff_set;
		std::set_difference(_source.begin(), _source.end(), _substract.begin(), _substract.end(), std::inserter(diff_set, diff_set.begin()));
		return diff_set;
	}

	Mask summ(const Mask& _set1, const Mask& _set2)
	{
		Mask union_set;
		std::set_union(_set1.begin(), _set1.end(), _set2.begin(), _set2.end(), std::inserter(union_set, union_set.begin()));
		return union_set;
	}

	Mask left()
	{
		return{ {-1,0} };
	}

	Mask right()
	{
		return{ { 1,0 } };
	}

	Mask top()
	{
		return{ { 0,1 } };
	}

	Mask bottom()
	{
		return{ { 0,-1 } };
	}

	Mask left_right()
	{
		return summ(left(), right());
	}

	Mask top_bottom()
	{
		return summ(top(), bottom());
	}

	Mask cross()
	{
		return summ(left_right(), top_bottom());
	}

	Mask square()
	{
		return { {-1,1}, {0,1} , {1,1} , {-1,0} , {1,0} ,{-1,-1}, {-1,0}, {-1,-1} };
	}

	Mask horse()
	{
		return { { 1,2 },{ 2,1 } ,{ 1,-2 }, { 2,-1 },{ -1,-2 },{ -2,-1 },{ -1,2 },{ -2, 1 } };
	}
}