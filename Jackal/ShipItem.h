#pragma once

#include "PlayItem.h"

class ShipItem : public PlayItem
{
public:
	ShipItem(const QRectF& _draw_rect = QRectF())
	{
		set_draw_rect(_draw_rect);

		set_image(QPixmap("./Resources/items/pirate_ship.png"));

		Selection* selection =  new RectSelection(this);	//ownership
		selection->set_color(QColor(250, 240, 150));
		selection->bind_to_state(m_selected_state);
	}

	virtual void paint(QPainter *_painter, const QStyleOptionGraphicsItem *_option, QWidget *_widget = Q_NULLPTR) override
	{
		if (!m_image.isNull())
		{
			QRectF draw_rect = boundingRect();
			QPixmap img = m_image.scaled(draw_rect.width(), draw_rect.height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
			_painter->drawPixmap(boundingRect(), img, img.rect());
		}
	}
};
