#pragma once

#include "PlayItem.h"

class ShipItem : public PlayItem
{

public:
	ShipItem(const QRectF& _draw_rect = QRectF())
	{
		set_draw_rect(_draw_rect);

		m_selection = std::make_unique<RectSelection>(this);
		m_selection->set_color(QColor(250, 240, 150));

		set_brush(QColor(70, 50, 10));
		m_state_machine->start();
	}

	virtual void paint(QPainter *_painter, const QStyleOptionGraphicsItem *_option, QWidget *_widget = Q_NULLPTR) override
	{
		_painter->setBrush(AbstractShape::m_brush);
		_painter->setPen(AbstractShape::m_pen);
		_painter->drawRoundRect(boundingRect(), 2, 2);
	}
};
