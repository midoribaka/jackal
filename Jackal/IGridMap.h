#pragma once

#include <memory>
#include <QGraphicsScene>

#include "RoundedRect.h"
#include "ICell.h"
#include "IPlayItem.h"

class IGridMap : public RoundedRect
{
	Q_OBJECT

public:
	static IGridMap* create(size_t _px_size, QGraphicsScene* _scene);	//no shared_ptr for QGraphicItems

	virtual ~IGridMap() {};

	virtual QPoint grid_to_px(const QPoint& _grid_pos) const = 0;
	virtual QPoint px_to_grid(const QPoint& _px_pos) const = 0;

	virtual QPoint grid_pos(const IPlayItem* _item) const = 0;

	virtual QGraphicsScene* scene() const = 0;	//no shared_ptr for QGraphicItems

	virtual void activate_cells_around(const QPoint& _grid_pos) = 0;
	virtual void activate_cells_around(const IPlayItem* _item) = 0;

	virtual void desactivate_cells_around(const QPoint& _grid_pos) = 0;
	virtual void desactivate_cells_around(const IPlayItem* _item) = 0;

	virtual void run_cell_action(const QPoint& _grid_pos) = 0;

signals:
	void cell_ready(const ICell* _cell);
};
