#pragma once

#include "CenteredRoundedRect.h"

#include <bitset>

enum class CellType
{
	CORNER,
	SEA
};

using BitMask = std::bitset<169>;

class ICell : public CenteredRoundedRect
{
	Q_OBJECT

public:
	static ICell* create(CellType _ctype);

	virtual BitMask mask() const = 0;

	virtual void set_mask(BitMask _mask) = 0;

	virtual void set_side_size(size_t _side) = 0;

	virtual ~ICell() {};

signals:

	//iface
	void activate();	//from idle to active
	void desactivate();	//from active to idle
	void select();		//from active to selected

	//signals

};