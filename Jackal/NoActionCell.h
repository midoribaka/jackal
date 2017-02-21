#pragma once

#include "Cell.h"

class NoActionCell : public Cell
{
public:
	NoActionCell()
	{
		CenteredRoundedRect::set_image(QPixmap("./Resources/cell_img/corner_cell.png"));
	}
};
//