#pragma once

#include "Cell.h"

class NoActionCell : public Cell
{
public:
	NoActionCell()
	{
		CenteredRoundedRect::set_image(QPixmap("./Resources/cell_img/corner_cell.png"));
	}

	void activate(std::shared_ptr<Player>) override
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