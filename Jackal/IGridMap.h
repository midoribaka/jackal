#pragma once

#include <memory>
#include <QGraphicsScene>

#include "RoundedRect.h"

class IGridMap : public RoundedRect
{
public:
	static IGridMap* create(size_t _px_size, QGraphicsScene* _scene);	//no shared_ptr for QGraphicItems

	virtual ~IGridMap() {};

	virtual QPoint grid_to_px(const QPoint& _px_pos) const = 0;

	virtual QGraphicsScene* scene() const = 0;	//no shared_ptr for QGraphicItems
};