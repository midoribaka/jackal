#pragma once

#include <memory>

#include <QPropertyAnimation>

#include "RoundedRect.h"

class Selection : public RoundedRect
{
	Q_OBJECT
	Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)	
	Q_PROPERTY(bool activate READ activated WRITE activate)

public:
	Selection(QGraphicsObject* _selected_item)
	{
		setParentItem(_selected_item);

		setOpacity(0);
		setZValue(1.5);

		m_hover_in = std::make_unique<QPropertyAnimation>(this, "opacity");
		m_hover_in->setStartValue(0.0);
		m_hover_in->setEndValue(1.0);
		m_hover_in->setDuration(400);
		m_hover_in->setEasingCurve(QEasingCurve::OutBack);

		m_hover_out = std::make_unique<QPropertyAnimation>(this, "opacity");
		m_hover_out->setStartValue(1.0);
		m_hover_out->setEndValue(0.0);
		m_hover_out->setDuration(400);
		m_hover_out->setEasingCurve(QEasingCurve::InOutQuad);
	}

	virtual void paint(QPainter *_painter, const QStyleOptionGraphicsItem *_option, QWidget *_widget = Q_NULLPTR) override
	{
		QPen pen;
		pen.setWidth(3);
		pen.setColor(QColor(250, 240, 150));
		_painter->setPen(pen);

		QRectF over_size = parentItem()->boundingRect().translated(-1, -1);
		over_size.setSize(QSize(over_size.width() + 2, over_size.height() + 2));
		_painter->drawRoundRect(over_size, corner_radius, corner_radius);
	}

	void show()
	{
		m_hover_in->start();
	}

	void hide()
	{
		m_hover_out->start();
	}

	bool activated()
	{
		return isVisible();
	}

	void activate(bool _arg)
	{
		(_arg) ? show() : hide();
	}

private:
	std::unique_ptr<QPropertyAnimation> m_hover_in, m_hover_out;
};