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
	void set_side_size(size_t _side) override;
	const Mask&  mask() const  override;
	void set_mask(const Mask& _mask) override;
	virtual void run_action() override = 0;

protected:
	Mask m_mask;
};
