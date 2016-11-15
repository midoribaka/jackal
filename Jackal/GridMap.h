#pragma once

#include <QVector>

#include "RoundedRect.h"
#include "Cell.h"

class GridMap : public RoundedRect
{
	const size_t rows_num = 13;
	const size_t cell_corner_radius = 5;
	const size_t spacer_size = 4;
	const size_t padding_size = 10;

public:
	GridMap(size_t _px_size, size_t _corner_radius, QGraphicsItem* _parent = 0);

	Cell* get_cell(size_t _x, size_t _y) const;

private:
	QVector<QVector<Cell*> > cells;

	QPointF get_cell_center(size_t _x, size_t _y) const;

	size_t cell_side_size;
};