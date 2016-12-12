#pragma once

#include "RoundedRect.h"
#include <memory>

class Cell : public RoundedRect
{
	Q_OBJECT
	Q_PROPERTY(qreal zValue READ zValue WRITE setZValue)

	const double default_z_value = 1;

public:
	Cell();

	virtual ~Cell();
	virtual QRectF boundingRect() const override;
	QPoint grid_pos() const;
	void set_grid_pos(const QPoint& _pos);
	void set_side_size(size_t _size);

private:
	QPoint m_grid_pos;
};
