#pragma once

#include <memory>

#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>

#include "RoundedRect.h"

class Selection : public RoundedRect
{
	Q_OBJECT

public:
	Selection(QGraphicsObject* _selected_item): m_alfa(0)
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
	}

	virtual void paint(QPainter *_painter, const QStyleOptionGraphicsItem *_option, QWidget *_widget = Q_NULLPTR) override
	{
		QPen pen;
//		pen.setWidth(3);
//		pen.setColor(QColor(250, 240, 150));
		pen.setWidth(1);				//debug
		pen.setColor(QColor(255, 0, 0));//debug
		_painter->setPen(pen);
		_painter->drawRoundRect(boundingRect(), corner_radius, corner_radius);
	}

	virtual QRectF boundingRect() const override
	{
		return QRectF(-5, -5, 10, 10);	//debug
//		return parentItem()->boundingRect().adjusted(-1, -1, 1, 1);	//don't put this line into _painter->drawRoundRect(boundingRect(), corner_radius, corner_radius); THIS WILL NOT WORK
	}

	void hover_in()
	{
		m_hover_in->start();
	}

	void hover_out()
	{
		m_hover_out->start();
	}

private:
	size_t m_alfa;
	std::unique_ptr<QPropertyAnimation> m_hover_in, m_hover_out;
};