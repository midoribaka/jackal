#pragma once

#include "PlayItems.h"

class PirateItem : public PlayItem
{
public:
	PirateItem(QObject* _parent_obj, const QRectF& _draw_rect = QRectF()) : PlayItem(_parent_obj, _draw_rect)
	{
		set_brush(QColor(30, 200, 20));
		m_state_machine->start();
	}
};