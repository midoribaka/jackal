#pragma once

#include "RoundedRect.h"

class Selection : public RoundedRect
{
	Q_OBJECT
	Q_PROPERTY(bool visible READ isVisible WRITE setVisible)

public:
	Selection(QGraphicsObject* _selected_item)
	{
		setVisible(false);

		setParentItem(_selected_item);
		QRectF over_size = _selected_item->boundingRect();		//todo too many copies
		over_size.setSize(QSize(over_size.width() - 4, over_size.height() - 4));
		RoundedRect::set_geometry(over_size.translated(2,2));
		setZValue(1.5);
	}

	virtual void paint(QPainter *_painter, const QStyleOptionGraphicsItem *_option, QWidget *_widget = Q_NULLPTR) override
	{
		QPen pen;
		pen.setWidth(4);
		pen.setColor(QColor(0, 255, 0));
		_painter->setPen(pen);
		_painter->drawRoundRect(boundingRect(), corner_radius, corner_radius);
	}
};