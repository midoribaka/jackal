
#include "Cell.h"

Cell::Cell() : m_grid_pos(0, 0)
{
	setZValue(default_z_value);
}

Cell::~Cell()
{
}

QPoint Cell::grid_pos() const
{
	return m_grid_pos;
}

void Cell::set_side_size(size_t _size)
{
	CenteredRoundedRect::set_draw_rect(QRectF(0, 0, _size, _size));
}
