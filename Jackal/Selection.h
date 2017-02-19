#pragma once

#include <memory>

#include <QPropertyAnimation>
#include <QState>

#include "RoundedRect.h"
#include "Circle.h"

class Selection : public AbstractShape
{
	Q_OBJECT

public:
	Selection(QGraphicsObject* _selected_item) : m_alfa(0)
	{
		setParentItem(_selected_item);

		setOpacity(0);
		setZValue(1.5);

		QPen pen;
		pen.setWidth(2);
		AbstractShape::set_pen(pen);

		m_hover_in = std::make_unique<QPropertyAnimation>(this, "opacity");
		m_hover_out = std::make_unique<QPropertyAnimation>(this, "opacity");

		QObject::connect(m_hover_in.get(), &QPropertyAnimation::finished, this, &Selection::end_hover_in);
		QObject::connect(m_hover_out.get(), &QPropertyAnimation::finished, this, &Selection::end_hover_out);

		m_hover_in->setStartValue(0.0);
		m_hover_in->setEndValue(0.9);
		m_hover_in->setDuration(400);
		m_hover_in->setEasingCurve(QEasingCurve::OutBack);

		m_hover_out->setStartValue(0.9);
		m_hover_out->setEndValue(0.0);
		m_hover_out->setDuration(400);
		m_hover_out->setEasingCurve(QEasingCurve::InOutQuad);
	}
	virtual ~Selection()
	{
	}

	void set_color(const QColor& _color)
	{
		QPen pen = AbstractShape::pen();
		pen.setColor(_color);
		AbstractShape::set_pen(pen);
		update();	//add redraw to queue
	}

	void hover_in()
	{
		emit begin_hover_in();
		m_hover_in->start();
	}

	void hover_out()
	{
		emit begin_hover_out();
		m_hover_out->start();
	}

	void bind_to_state(QState* _state)
	{
		QObject::connect(_state, &QState::entered, [this]
		{
			hover_in();
		});

		QObject::connect(_state, &QState::exited, [this]
		{
			hover_out();
		});
	}

	QRectF boundingRect() const override
	{
		return parentItem()->boundingRect().adjusted(-1, -1, 1, 1);	//don't put this line into _painter->drawRoundRect(boundingRect(), corner_radius, corner_radius); THIS WILL NOT WORK
	}

	virtual void paint(QPainter *_painter, const QStyleOptionGraphicsItem *_option, QWidget *_widget = Q_NULLPTR) override = 0;

protected:
	size_t m_alfa;
	std::unique_ptr<QPropertyAnimation> m_hover_in, m_hover_out;

signals:
	void begin_hover_in();
	void end_hover_in();
	void begin_hover_out();
	void end_hover_out();
};

class RectSelection : public Selection
{
public:
	RectSelection(QGraphicsObject* _selected_item) : Selection(_selected_item)
	{
	}

	virtual void paint(QPainter *_painter, const QStyleOptionGraphicsItem *_option, QWidget *_widget = Q_NULLPTR) override
	{
		_painter->setBrush(AbstractShape::m_brush);
		_painter->setPen(AbstractShape::m_pen);
		_painter->drawRoundRect(boundingRect(), 0, 0);
	}
};

class RoundSelection : public Selection
{
public:
	RoundSelection(QGraphicsObject* _selected_item) : Selection(_selected_item)
	{
		AbstractShape::set_brush(Qt::NoBrush);
	}

	virtual void paint(QPainter *_painter, const QStyleOptionGraphicsItem *_option, QWidget *_widget = Q_NULLPTR) override
	{
		_painter->setBrush(AbstractShape::m_brush);
		_painter->setPen(AbstractShape::m_pen);
		_painter->drawEllipse(boundingRect());
	}
};