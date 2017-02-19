#pragma once

#include "PlayItem.h"

class ShipItem : public PlayItem
{
public:
	ShipItem(const QRectF& _draw_rect = QRectF())
	{
		set_draw_rect(_draw_rect);
		set_brush(QColor(70, 50, 10));

		Selection* selection =  new RectSelection(this);	//ownership
		selection->set_color(QColor(250, 240, 150));
		selection->bind_to_state(m_selected_state);
	}

	virtual void paint(QPainter *_painter, const QStyleOptionGraphicsItem *_option, QWidget *_widget = Q_NULLPTR) override
	{
		_painter->setBrush(AbstractShape::m_brush);
		_painter->setPen(AbstractShape::m_pen);
		_painter->drawRoundRect(boundingRect(), 2, 2);
	}
};
