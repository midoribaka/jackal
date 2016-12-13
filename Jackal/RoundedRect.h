#pragma once
#include <QGraphicsObject>
#include <QPixmap>
#include <QRect>
#include <QPainter>

#include "AbstractShape.h"

class RoundedRect : public AbstractShape
{
public:
	RoundedRect() : corner_radius(0)
	{
	}

	void set_corner_radius(size_t _radius)
	{
		corner_radius = _radius;
		update();
	}

	virtual void set_image(const QPixmap& _pixmap)
	{
		image = _pixmap;
		update();
	}

	virtual void paint(QPainter *_painter, const QStyleOptionGraphicsItem *_option, QWidget *_widget = Q_NULLPTR) override
	{
		_painter->setBrush(AbstractShape::m_brush);
		_painter->setPen(Qt::NoPen);	// no border
		_painter->drawRoundRect(boundingRect(), corner_radius, corner_radius);

		if (!image.isNull()) 
		{
			_painter->drawPixmap(boundingRect(), image, image.rect());	//autoscale //todo довольно затратно
		}
	}

	virtual ~RoundedRect()
	{
	}

protected:
	QPixmap image;
	size_t corner_radius;
};