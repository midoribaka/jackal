#pragma once

#include <QGraphicsObject>
#include <QGradient>

class AbstractShape : public QGraphicsObject
{
public:
	AbstractShape() : m_draw_rect(QRectF()), m_brush(QColor(255, 255, 255))
	{
	}

	void set_draw_rect(const QRectF& _draw_rect)
	{
		m_draw_rect = _draw_rect;
	}

	virtual QRectF boundingRect() const override
	{
		//origin at 0,0 top left
		return m_draw_rect;
	}

	void set_brush(const QBrush& _brush)
	{
		m_brush = _brush;
	}

	virtual void paint(QPainter *_painter, const QStyleOptionGraphicsItem *_option, QWidget *_widget = Q_NULLPTR) override = 0;

	virtual ~AbstractShape()
	{
	}

protected:
	QRectF m_draw_rect;
	QBrush m_brush;
};