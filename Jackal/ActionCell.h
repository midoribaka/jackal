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
	enum MOVES
	{
		NONE = 0,
		NW = 1,
		N = 2,
		NO = 4,
		O = 8,
		SO = 16,
		S = 32,
		SW = 64,
		W = 128,
		ALL = 0xFFFFFFFF
	};

	ActionCell()
	{
		m_selection = std::make_unique<Selection>(this);	

		m_state_machine = new QStateMachine(this);
		m_initial_state = new QState(QState::ParallelStates);

		m_state_machine->addState(m_initial_state);
		m_state_machine->setInitialState(m_initial_state);

		QState* action_group = new QState(m_initial_state);
		QState* idle = new QState(action_group);
		QState* ready = new QState(action_group);
		QState* activated = new QState(action_group);

		idle->addTransition(this, &ActionCell::make_ready_called, ready);
		ready->addTransition(this, &ActionCell::make_idle_called, idle);
		ready->addTransition(this, &ActionCell::activate_called, activated);

		QObject::connect(ready, &QState::entered, [this]
		{
			m_selection->hover_in();
		});

		QObject::connect(ready, &QState::exited, [this]
		{
			m_selection->hover_out();
		});

		action_group->setInitialState(idle);
	}

	virtual ~ActionCell()
	{
	}

	void make_ready() override
	{
		emit make_ready_called();
	}

	void make_idle() override
	{
		emit make_idle_called();
	}

	void activate() override
	{
		emit activate_called();
	}

protected:
	QState* m_initial_state;
	QStateMachine* m_state_machine;

private:
	std::unique_ptr<Selection> m_selection;
	int m_move_mask;

signals:
	void make_ready_called();
	void make_idle_called();
	void activate_called();
};
