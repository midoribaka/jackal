#pragma once

#include <QGraphicsObject>
#include <QPixmap>
#include <QRect>
#include <QPainter>

#include "CenteredAbstractShape.h"

class CenteredRoundedRect : public CenteredAbstractShape
{
public:
	CenteredRoundedRect() : m_corner_radius(0)
	{
	}

	void set_corner_radius(size_t _radius)
	{
		m_corner_radius = _radius;
		update();
	}

	virtual void set_image(const QPixmap& _pixmap)
	{
		m_image = _pixmap;
		update();
	}

	virtual void paint(QPainter *_painter, const QStyleOptionGraphicsItem *_option, QWidget *_widget = Q_NULLPTR) override
	{
		_painter->setBrush(AbstractShape::m_brush);
		_painter->setPen(AbstractShape::m_pen);
		_painter->drawRoundRect(boundingRect(), m_corner_radius, m_corner_radius);

		if (!m_image.isNull())
		{
			_painter->drawPixmap(boundingRect(), m_image, m_image.rect());	//autoscale //todo довольно затратно
		}
	}

	virtual ~CenteredRoundedRect()
	{
	}

protected:
	QPixmap m_image;
	size_t m_corner_radius;
};