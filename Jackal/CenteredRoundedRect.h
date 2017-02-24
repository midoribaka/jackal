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

	virtual void paint(QPainter *_painter, const QStyleOptionGraphicsItem *_option, QWidget *_widget = Q_NULLPTR) override
	{
		_painter->setBrush(AbstractShape::m_brush);
		_painter->setPen(AbstractShape::m_pen);
		_painter->drawRoundRect(boundingRect(), m_corner_radius, m_corner_radius);

		if (!m_image.isNull())
		{
			QRectF draw_rect = boundingRect();
			QPixmap img = m_image.scaled(draw_rect.width(), draw_rect.height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
			_painter->drawPixmap(boundingRect(), img, img.rect());
		}
	}

	virtual ~CenteredRoundedRect()
	{
	}

protected:
	size_t m_corner_radius;
};