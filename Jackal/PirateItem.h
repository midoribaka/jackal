#pragma once

#include "PlayItem.h"

class PirateItem : public PlayItem
{
public:
	PirateItem(const QRectF& _draw_rect = QRectF())
	{
		set_draw_rect(_draw_rect);

		m_selection = std::make_unique<RoundSelection>(this);
		m_selection->set_color(QColor(250, 240, 150));

		set_brush(QColor(7, 99, 36));
		m_state_machine->start();
	}

	virtual void paint(QPainter *_painter, const QStyleOptionGraphicsItem *_option, QWidget *_widget = Q_NULLPTR) override
	{
		QRectF rect = boundingRect();
		int width = rect.width();
		QRadialGradient rg(rect.center().x(), rect.center().y(), width / 2, -width / 7, -width / 7);

		_painter->setBrush(Qt::darkGreen);
		_painter->setPen(Qt::NoPen);
		_painter->drawEllipse(rect);

		rg.setColorAt(0.0, QColor(255, 255, 255, 191));
		rg.setColorAt(0.2, QColor(255, 255, 127, 191));
		rg.setColorAt(0.9, QColor(150, 150, 200, 63));
		_painter->setBrush(rg);

		_painter->drawEllipse(rect);
	}
};