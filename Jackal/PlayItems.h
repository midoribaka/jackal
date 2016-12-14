#pragma once

#include <memory>

#include <QStateMachine>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include <QMouseEventTransition>
#include <QSignalTransition>
#include <QPropertyAnimation>

#include "Selection.h"

class PlayItem : public RoundedRect
{
	Q_OBJECT
	Q_PROPERTY(QPointF position READ pos WRITE setPos)

public:
	PlayItem(QObject* _parent_obj, const QRectF& _draw_rect = QRectF())
	{
		setParent(_parent_obj);

		m_selection = std::make_unique<RectSelection>(this);	
		m_selection->set_color(QColor(250, 240, 150));
		RoundedRect::set_draw_rect(_draw_rect);

		m_state_machine = new QStateMachine(this);

		QState* ready_state = new QState(m_state_machine);
		QState* choosed_state = new QState(m_state_machine);
		QState* idle_state = new QState(m_state_machine);

		QEventTransition *mouse_press = new QEventTransition(this, QEvent::MouseButtonPress, ready_state);
		mouse_press->setTargetState(choosed_state);

		QObject::connect(choosed_state, &QState::entered, [this]
		{
			m_selection->hover_in();
		});

		QObject::connect(choosed_state, &QState::exited, [this]
		{
			m_selection->hover_out();
		});

		//ready_state->assignProperty(m_selection.get(), "activate", false);
		//choosed_state->assignProperty(m_selection.get(), "activate", true);
		//idle_state->assignProperty(m_selection.get(), "activate", false);

		choosed_state->addTransition(this, SIGNAL(make_ready()), ready_state);
		QObject::connect(choosed_state, &QState::entered, this, &PlayItem::choosed);	//emit signal to scene to deselect all another items 

		//Moving animation
		m_moving = std::make_unique<QPropertyAnimation>(this, "position");
		m_moving->setEasingCurve(QEasingCurve::InOutQuart);

		QObject::connect(m_moving.get(), &QPropertyAnimation::finished, this, &PlayItem::finish);
		choosed_state->addTransition(this, SIGNAL(finish()), idle_state);

		m_state_machine->setInitialState(ready_state);
	}

	virtual QRectF boundingRect() const override
	{
		//origin at center
		QRectF br = RoundedRect::boundingRect();
		return br.translated(-br.width() / 2, -br.height() / 2);
	}

	void unselect()	// calling by scene
	{
		emit make_ready();	// -> to ready state
	}

	void move_to(const QPointF& _pos)
	{
		QPointF current = pos();

		int s1 = _pos.x() - current.x();
		int s2 = _pos.y() - current.y();
		int mod = sqrt(s1*s1 + s2*s2);

		m_moving->setDuration(1000*mod / 50);	//50px/1sec	//todo no constant

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

protected:
	std::unique_ptr<RectSelection> m_selection;
	QStateMachine* m_state_machine;

	void mousePressEvent(QGraphicsSceneMouseEvent *_event) override
	{
		auto wrapped = new QStateMachine::WrappedEvent(this, new QMouseEvent(QEvent::MouseButtonPress, _event->pos(), _event->button(), _event->buttons(), _event->modifiers()));
		m_state_machine->postEvent(wrapped);	//will be deleted inside
		RoundedRect::mousePressEvent(_event);
		_event->accept();	//чтобы эвент не приходил нижнему item-у
	}

private:
	QPoint m_grid_pos;
	std::unique_ptr<QPropertyAnimation> m_moving;

signals :
	void choosed();
	void make_ready();
	void finish();
};
