#pragma once

#include "Cell.h"

class NoActionCell : public Cell
{
public:
	NoActionCell()
	{
		RoundedRect::set_image(QPixmap("./Resources/cell_img/corner_cell.png"));
	}

	void make_ready() override
	{
		//do nothing
	}

	void make_idle() override
	{
		//do nothing
	}

	void activate() override
	{
		//do nothing
	}

	int mask() const override
	{
		return 0;
	}

	void set_grid_pos(const QPoint&) override
	{
		//do nothing
	}
};
//