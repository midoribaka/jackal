#pragma once

#include <memory>

#include <QPropertyAnimation>

#include "RoundedRect.h"
#include "Circle.h"

class RectSelection : public RoundedRect
{

public:
	RectSelection(QGraphicsObject* _selected_item): m_alfa(0)
	{
		setParentItem(_selected_item);

		setOpacity(0);
		setZValue(1.5);

		m_hover_in = std::make_unique<QPropertyAnimation>(this, "opacity");
		m_hover_in->setStartValue(0.0);
		m_hover_in->setEndValue(0.9);
		m_hover_in->setDuration(400);
		m_hover_in->setEasingCurve(QEasingCurve::OutBack);

		m_hover_out = std::make_unique<QPropertyAnimation>(this, "opacity");
		m_hover_out->setStartValue(0.9);
		m_hover_out->setEndValue(0.0);
		m_hover_out->setDuration(400);
		m_hover_out->setEasingCurve(QEasingCurve::InOutQuad);

		QPen pen;
		pen.setWidth(2);
		AbstractShape::set_pen(pen);
	}

	virtual QRectF boundingRect() const override
	{
//		return parentItem()->boundingRect().adjusted(-1, -1, 1, 1);	//don't put this line into _painter->drawRoundRect(boundingRect(), corner_radius, corner_radius); THIS WILL NOT WORK
		return parentItem()->boundingRect();
	}

	void hover_in()
	{
		m_hover_in->start();
	}

	void hover_out()
	{
		m_hover_out->start();
	}

	void set_color(const QColor& _color)
	{
		QPen pen = AbstractShape::pen();
		pen.setColor(_color);
		AbstractShape::set_pen(pen);
		update();	//add redraw to queue
	}

private:
	size_t m_alfa;
	std::unique_ptr<QPropertyAnimation> m_hover_in, m_hover_out;
	QColor m_color;
};

class RoundSelection : public Circle
{

public:
	RoundSelection(QGraphicsObject* _selected_item) : m_alfa(0)
	{
		setParentItem(_selected_item);

		setOpacity(0);
		setZValue(1.5);

		m_hover_in = std::make_unique<QPropertyAnimation>(this, "opacity");
		m_hover_in->setStartValue(0.0);
		m_hover_in->setEndValue(0.9);
		m_hover_in->setDuration(400);
		m_hover_in->setEasingCurve(QEasingCurve::OutBack);

		m_hover_out = std::make_unique<QPropertyAnimation>(this, "opacity");
		m_hover_out->setStartValue(0.9);
		m_hover_out->setEndValue(0.0);
		m_hover_out->setDuration(400);
		m_hover_out->setEasingCurve(QEasingCurve::InOutQuad);

		QPen pen;
		pen.setWidth(2);
		AbstractShape::set_pen(pen);
	}

	virtual QRectF boundingRect() const override
	{
		//		return parentItem()->boundingRect().adjusted(-1, -1, 1, 1);	//don't put this line into _painter->drawRoundRect(boundingRect(), corner_radius, corner_radius); THIS WILL NOT WORK
		return parentItem()->boundingRect();
	}

	void hover_in()
	{
		m_hover_in->start();
	}

	void hover_out()
	{
		m_hover_out->start();
	}

	void set_color(const QColor& _color)
	{
		QPen pen = AbstractShape::pen();
		pen.setColor(_color);
		AbstractShape::set_pen(pen);
		update();	//add redraw to queue
	}

private:
	size_t m_alfa;
	std::unique_ptr<QPropertyAnimation> m_hover_in, m_hover_out;
	QColor m_color;
};