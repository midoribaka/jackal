#pragma once

#include <QStateMachine>
#include <QGraphicsRotation>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include <QMouseEventTransition>
#include <QSignalTransition>

#include "Selection.h"
#include "Cell.h"

class ActionCell : public Cell
{
	Q_OBJECT

public:
	ActionCell()
	{
		m_selection = std::make_unique<RectSelection>(this);	
		m_selection->set_color(QColor(255, 0, 0));

		m_state_machine = new QStateMachine(this);
		m_initial_state = new QState(QState::ParallelStates);

		m_state_machine->addState(m_initial_state);
		m_state_machine->setInitialState(m_initial_state);

		QState* action_group = new QState(m_initial_state);

		QState* idle = new QState(action_group);
		QState* active = new QState(action_group);
		QState* selected = new QState(action_group);

		idle->addTransition(this, &ActionCell::activate, active);
		active->addTransition(this, &ActionCell::desactivate, idle);

		QEventTransition *mouse_press = new QEventTransition(this, QEvent::MouseButtonPress, active);
		mouse_press->setTargetState(selected);

		m_selection->bind_to_state(active);

		action_group->setInitialState(idle);
	}

	virtual ~ActionCell()
	{
	}

protected:
	QState* m_initial_state;
	QStateMachine* m_state_machine;

	void mousePressEvent(QGraphicsSceneMouseEvent *_event) override
	{
		auto wrapped = new QStateMachine::WrappedEvent(this, new QMouseEvent(QEvent::MouseButtonPress, _event->pos(), _event->button(), _event->buttons(), _event->modifiers()));
		m_state_machine->postEvent(wrapped);	//will be deleted inside
		CenteredRoundedRect::mousePressEvent(_event);
	}

private:
	std::unique_ptr<RectSelection> m_selection;
};
