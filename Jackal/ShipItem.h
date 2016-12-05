#pragma once

#include "PlayItems.h"

class ShipItem : public PlayItem
{
public:
	ShipItem(QObject* _parent_obj, const QRectF& _draw_rect = QRectF()) : PlayItem(_parent_obj, _draw_rect)
	{
		set_color(QColor(70, 50, 10));
		m_state_machine->start();
	}
};
