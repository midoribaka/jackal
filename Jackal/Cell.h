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
	void set_side_size(size_t _size);

	//todo bad because noactioncell does not need make_idle, make_ready, mask, activate, set_grid_pos
	virtual void set_grid_pos(const QPoint& _pos) = 0;
	virtual void make_idle() = 0;
	virtual void make_ready() = 0;
	virtual void activate() = 0;
	virtual int mask() const = 0;

protected:
	QPoint m_grid_pos;
};
