#pragma once

#include "CenteredRoundedRect.h"

#include "Mask.h"

enum class CellType
{
	CORNER,
	SEA
};

class ICell : public CenteredRoundedRect
{
	Q_OBJECT

public:
	static ICell* create(CellType _ctype);

	virtual const Mask& mask() const = 0;

	virtual void set_mask(const Mask& _mask) = 0;

	virtual void set_side_size(size_t _side) = 0;

	virtual ~ICell() {};

signals:

	//iface
	void activate();	//from idle to active
	void desactivate();	//from active to idle
	void select();		//from active to selected
	void deselect();	//from selected to idle

	//signals
	void selected();

};