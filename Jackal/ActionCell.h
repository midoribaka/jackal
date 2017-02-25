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

		QState* idle_state = new QState(action_group);
		QState* active_state = new QState(action_group);
		QState* selected_state = new QState(action_group);

		idle_state->addTransition(this, &ActionCell::activate, active_state);
		active_state->addTransition(this, &ActionCell::desactivate, idle_state);
		selected_state->addTransition(this, &ActionCell::deselect, idle_state);

		QEventTransition *mouse_press = new QEventTransition(this, QEvent::MouseButtonPress, active_state);
		mouse_press->setTargetState(selected_state);

		QObject::connect(selected_state, &QState::entered, this, &ActionCell::prepare);

		m_selection->bind_to_state(active_state);

		action_group->setInitialState(idle_state);
	}

	virtual ~ActionCell()
	{
	}

protected slots:
	virtual void prepare() = 0;
	virtual void run_action() override = 0;

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
