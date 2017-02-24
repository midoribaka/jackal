#pragma once

#include <set>
#include <QPoint>

using Mask = std::set<std::pair<int,int> >;

namespace mask
{
	Mask left();
	Mask right();
	Mask top();
	Mask bottom();
	Mask top_bottom();
	Mask left_right();
	Mask cross();
	Mask square();
	Mask horse();
	Mask quad_diagonal();
	Mask diff(const Mask& _source, const Mask& _substract);
	Mask summ(const Mask& _set1, const Mask& _set2);
}