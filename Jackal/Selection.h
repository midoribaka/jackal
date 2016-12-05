#pragma once

#include <memory>

#include <QPropertyAnimation>

#include "RoundedRect.h"

class Selection : public RoundedRect
{
	Q_OBJECT
	Q_PROPERTY(int clarity READ clarity WRITE set_clarity)
	Q_PROPERTY(bool activate READ activated WRITE activate)

public:
	Selection(QGraphicsObject* _selected_item) : m_alfa(0)
	{
		setVisible(false);

		setParentItem(_selected_item);
		QRectF over_size = _selected_item->boundingRect().translated(-1,-1);
		over_size.setSize(QSize(over_size.width() + 2, over_size.height() + 2));
		RoundedRect::set_geometry(over_size);
		setZValue(1.5);

		m_hover_in = std::make_unique<QPropertyAnimation>(this, "clarity");
		m_hover_in->setStartValue(0);
		m_hover_in->setEndValue(200);
		m_hover_in->setDuration(400);
		m_hover_in->setEasingCurve(QEasingCurve::OutBack);

		m_hover_out = std::make_unique<QPropertyAnimation>(this, "clarity");
		m_hover_out->setStartValue(200);
		m_hover_out->setEndValue(0);
		m_hover_out->setDuration(400);
		m_hover_out->setEasingCurve(QEasingCurve::InOutQuad);
	}

	virtual void paint(QPainter *_painter, const QStyleOptionGraphicsItem *_option, QWidget *_widget = Q_NULLPTR) override
	{
		QPen pen;
		pen.setWidth(3);
		pen.setColor(QColor(250, 240, 150, m_alfa));
		_painter->setPen(pen);
		_painter->drawRoundRect(boundingRect(), corner_radius, corner_radius);
	}

	void show()
	{
		setVisible(true);
		m_hover_in->start();
	}

	void hide()
	{
		setVisible(false);
		m_hover_in->start();
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
	int clarity() const
	{
		return m_alfa;
	}

	void set_clarity(int _alfa)
	{
		m_alfa = _alfa;
		update();		//add repaint to queue
	}

	std::unique_ptr<QPropertyAnimation> m_hover_in, m_hover_out;

	int m_alfa;
};