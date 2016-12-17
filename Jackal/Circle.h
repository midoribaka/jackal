#pragma once

#include "AbstractShape.h"
#include <QPainter>

class Circle : public AbstractShape
{
public:
	Circle()
	{
	}

	virtual void paint(QPainter *_painter, const QStyleOptionGraphicsItem *_option, QWidget *_widget = Q_NULLPTR) override
	{
		_painter->setBrush(AbstractShape::m_brush);
		_painter->setPen(AbstractShape::m_pen);
		_painter->drawEllipse(boundingRect());
	}

	virtual ~Circle()
	{
	}
};