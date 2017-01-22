#pragma once

#include <memory>

#include <QStateMachine>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include <QMouseEventTransition>
#include <QSignalTransition>
#include <QPropertyAnimation>

#include "Selection.h"

class PlayItem : public AbstractShape
{
	Q_OBJECT

public:
	PlayItem()
	{
		/*
		ready_state -> (mousePress) -> selected_state

		selected_state -> (scene signal: make_ready) -> ready_state
		
		selected_state -> (moving animation finished signal) -> idle_state
		*/

		m_state_machine = new QStateMachine(this);

		QState* idle_state = new QState(m_state_machine);
		QState* ready_state = new QState(m_state_machine);
		QState* selected_state = new QState(m_state_machine);
		QState* active_state = new QState(m_state_machine);

		QEventTransition *mouse_press = new QEventTransition(this, QEvent::MouseButtonPress, ready_state);
		mouse_press->setTargetState(selected_state);

		idle_state->addTransition(this, &PlayItem::make_ready, ready_state);
		selected_state->addTransition(this, &PlayItem::make_ready, ready_state);

		QObject::connect(selected_state, &QState::entered, [this]
		{
			emit selected();				//emit signal to scene to deselect all another items by calling make_ready
			m_selection->hover_in();
		});

		QObject::connect(selected_state, &QState::exited, [this]
		{
			m_selection->hover_out();
		});

		//Moving animation
		m_moving = std::make_unique<QPropertyAnimation>(this, "position");
		m_moving->setEasingCurve(QEasingCurve::InOutQuart);
		selected_state->addTransition(m_moving.get(), &QPropertyAnimation::finished, idle_state);

		m_state_machine->setInitialState(idle_state);
	}

	QRectF boundingRect() const override
	{
		//origin at center
		QRectF br = AbstractShape::boundingRect();
		return br.translated(-br.width() / 2, -br.height() / 2);
	}

	void move_to(const QPointF& _pos)
	{
		QPointF current = pos();
		QPointF d = _pos - current;
		m_moving->setDuration(1000 * hypot(d.x(), d.y()) / 50);	//50px/1sec	//todo no constant

		m_moving->setStartValue(pos());
		m_moving->setEndValue(_pos);
		m_moving->start();
	}

	void set_grid_pos(const QPoint& _point)
	{
		m_grid_pos = _point;
	}

	QPoint grid_pos() const
	{
		return m_grid_pos;
	}

	virtual ~PlayItem()
	{
	}

	virtual void paint(QPainter *_painter, const QStyleOptionGraphicsItem *_option, QWidget *_widget = Q_NULLPTR) override = 0;

	void mousePressEvent(QGraphicsSceneMouseEvent *_event) override
	{
		auto wrapped = new QStateMachine::WrappedEvent(this, new QMouseEvent(QEvent::MouseButtonPress, _event->pos(), _event->button(), _event->buttons(), _event->modifiers()));
		m_state_machine->postEvent(wrapped);	//will be deleted inside
		AbstractShape::mousePressEvent(_event);
		_event->accept();
	}

protected:
	std::unique_ptr<Selection> m_selection;
	QStateMachine* m_state_machine;

private:
	QPoint m_grid_pos;
	std::unique_ptr<QPropertyAnimation> m_moving;

signals :
	void selected();
	void make_ready();
	void make_idle();
};
