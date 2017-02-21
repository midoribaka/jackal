#pragma once

#include <memory>

#include "Player.h"
#include "ICell.h"

class Cell : public ICell
{
	Q_OBJECT
	Q_PROPERTY(qreal zValue READ zValue WRITE setZValue)

	const double default_z_value = 1;

public:
	Cell();

	virtual ~Cell();
	QPoint grid_pos() const;
	void set_side_size(size_t _size);

	//todo bad because noactioncell does not need make_idle, make_ready, mask, activate, set_grid_pos
	virtual void set_grid_pos(const QPoint& _pos) = 0;
	virtual void activate(std::shared_ptr<Player> _player) = 0;
	virtual int mask() const = 0;

protected:
	QPoint m_grid_pos;

signals:
	void make_ready();
	void make_idle();
};
