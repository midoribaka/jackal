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
		//QRectF rect = boundingRect();
		//QRadialGradient rg(rect.center().x(), rect.center().y(), rect.width() / 2, 0, 0);
		//rg.setColorAt(0.0, QColor(255, 255, 255, 191));
		//rg.setColorAt(0.2, QColor(255, 255, 127, 191));
		//rg.setColorAt(0.9, QColor(150, 150, 200, 63));
		//_painter->setBrush(rg);
		_painter->setPen(Qt::NoPen);	
		_painter->drawEllipse(boundingRect());
	}

	virtual ~Circle()
	{
	}
};