#pragma once

#include <QGraphicsObject>
#include <QGradient>
#include <QPen>

class AbstractShape : public QGraphicsObject
{
	Q_OBJECT
	Q_PROPERTY(QPointF position READ pos WRITE setPos)

public:
	AbstractShape() : m_draw_rect(QRectF()), m_brush(Qt::NoBrush), m_pen(Qt::NoPen)
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

	void set_pen(const QPen& _pen)
	{
		m_pen = _pen;
	}

	QPen pen() const
	{
		return m_pen;
	}

	QBrush brush() const
	{
		return m_brush;
	}

	virtual void paint(QPainter *_painter, const QStyleOptionGraphicsItem *_option, QWidget *_widget = Q_NULLPTR) override = 0;

	virtual ~AbstractShape()
	{
	}

protected:
	QRectF m_draw_rect;
	QBrush m_brush;
	QPen m_pen;
};