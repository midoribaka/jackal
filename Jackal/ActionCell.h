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

		QState* selected_group = new QState(m_initial_state);
		QState* unselected = new QState(selected_group);
		QState* selected = new QState(selected_group);

		unselected->addTransition(this, &ActionCell::select_self, selected);
		selected->addTransition(this, &ActionCell::unselect_self, unselected);

		unselected->assignProperty(m_selection.get(), "activate", false);
		selected->assignProperty(m_selection.get(), "activate", true);

		selected_group->setInitialState(selected);
	}

	virtual ~ActionCell()
	{
	}

	void select()
	{
		emit select_self();
	};

	void unselect()
	{
		emit unselect_self();
	};

protected:
	QState* m_initial_state;
	QStateMachine* m_state_machine;

private:
	std::unique_ptr<Selection> m_selection;
	int m_move_mask;

signals:
	void select_self();
	void unselect_self();
};
