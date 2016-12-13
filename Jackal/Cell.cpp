
#include "Cell.h"

Cell::Cell() : m_grid_pos(0, 0)
{
	setZValue(default_z_value);
}

Cell::~Cell()
{
}

QRectF Cell::boundingRect() const
{
	//origin at center
	QRectF br = RoundedRect::boundingRect();
	return br.translated(-br.width() / 2, -br.height() / 2);
}

QPoint Cell::grid_pos() const
{
	return m_grid_pos;
}

void Cell::set_side_size(size_t _size)
{
	RoundedRect::set_draw_rect(QRectF(0, 0, _size, _size));
}
