#pragma once

#include <QVector>

#include <memory>

#include "RoundedRect.h"
#include "Cell.h"

class GridMap : public RoundedRect
{
	Q_OBJECT

	const size_t rows_num = 13;
	const size_t cell_corner_radius = 0;	//todo не работает
	const size_t spacer_size = 4;

	size_t padding_size = 2;	//адаптивный размер полей сетки под разрешение экрана

public:
	GridMap(size_t _px_size, size_t _corner_radius, QGraphicsItem* _parent = 0);

	std::shared_ptr<Cell> get_cell(size_t _x, size_t _y) const;

private:
	QVector<QVector<std::shared_ptr<Cell> > > cells;

	QPointF get_cell_center(size_t _x, size_t _y) const;

	size_t cell_side_size;


signals:
	void reset_field();
};