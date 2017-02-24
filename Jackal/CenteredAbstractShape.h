#pragma once

#include "AbstractShape.h"

class CenteredAbstractShape : public AbstractShape
{

public:
	QRectF boundingRect() const override
	{
		//origin at center
		QRectF br = AbstractShape::boundingRect();
		return br.translated(-br.width() / 2, -br.height() / 2);
	}

	virtual void set_image(const QPixmap& _pixmap)
	{
		m_image = _pixmap;
		update();
	}

	virtual void paint(QPainter *_painter, const QStyleOptionGraphicsItem *_option, QWidget *_widget = Q_NULLPTR) override = 0;

	virtual ~CenteredAbstractShape()
	{
	}

protected:
	QPixmap m_image;
};