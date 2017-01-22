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
	/*

		0		1		2		3		4
	-----------------------------------------
0	| 0		| 1 NWW	| 2		| 3 NNO	| 4		|
	-----------------------------------------
1	| 5 NNW	| 6 NW	| 7 N	| 8 NO	| 9 NOO	|
	-----------------------------------------
2	| 10	| 11 W	| 12 X	| 13 O	| 14	|
	-----------------------------------------
3	|15 SWW	|16 SW	| 17 S	| 18 SO	|19 SOO	|
	-----------------------------------------
4	| 20	|21 SWW	| 22	|23 SSO	| 24	|
	-----------------------------------------

	*/
	enum MOVES
	{
		NWW = 1 << 1,	//2^1
		NNO = 1 << 3,	//2^3
		NNW = 1 << 5,	//...
		NW	= 1 << 6,
		N	= 1 << 7,
		NO	= 1 << 8,
		NOO = 1 << 9,
		W	= 1 << 11,
		O	= 1 << 13,
		SWW = 1 << 15,
		SW	= 1 << 16,
		S	= 1 << 17,
		SO	= 1 << 18,
		SOO = 1 << 19,
		SSW = 1 << 21,
		SSO = 1 << 23,
		ALL = 0xFFFFFFFF,
		AROUND = N | NO | NW | O | W | S | SW | SO,
		LEFT_SIDE	= 0x218862,
		RIGHT_SIDE	= 0x8C2308,
		TOP_SIDE	= 0x3EA,
		BOTTOM_SIDE = 0xAF8000
	};

	ActionCell() : m_move_mask(0)
	{
		m_selection = std::make_unique<RectSelection>(this);	
		m_selection->set_pen(QColor(172, 65, 255));

		m_state_machine = new QStateMachine(this);
		m_initial_state = new QState(QState::ParallelStates);

		m_state_machine->addState(m_initial_state);
		m_state_machine->setInitialState(m_initial_state);

		QState* action_group = new QState(m_initial_state);
		QState* idle = new QState(action_group);
		QState* ready = new QState(action_group);
		QState* activated = new QState(action_group);

		idle->addTransition(this, &Cell::make_ready, ready);
		ready->addTransition(this, &ActionCell::activated_called, activated);
		ready->addTransition(this, &Cell::make_idle, idle);
		activated->addTransition(this, &Cell::make_idle, idle);

		QEventTransition *mouse_press = new QEventTransition(this, QEvent::MouseButtonPress, ready);
		mouse_press->setTargetState(activated);

		QObject::connect(ready, &QState::entered, [this]
		{
			m_selection->hover_in();
		});

		QObject::connect(ready, &QState::exited, [this]
		{
			m_selection->hover_out();
		});

		QObject::connect(activated, &QState::entered, [this]
		{
			emit activated_called();	// -> to grid_map -> to scene
		});

		action_group->setInitialState(idle);
	}

	virtual ~ActionCell()
	{
	}

	void activate(std::shared_ptr<Player> _player) override = 0;

	int mask() const override
	{
		return m_move_mask;
	}

	void set_grid_pos(const QPoint& _pos)
	{
		m_grid_pos = _pos;

		switch (m_grid_pos.x())
		{
		//island edges
		case 1:		
			m_move_mask &= ~MOVES::LEFT_SIDE;	
			break;	//rm left side
		case 11:	
			m_move_mask &= ~MOVES::RIGHT_SIDE;	
			break;	//rm right side

		//sea
		case 0:		
			m_move_mask |= MOVES::N | MOVES::S; 
			break;
		case 12:	
			m_move_mask |= MOVES::N | MOVES::S; 
			break;
		}

		switch (m_grid_pos.y())
		{
		case 1:		
			m_move_mask &= ~MOVES::TOP_SIDE;	
			break;	//rm top side
		case 11:	
			m_move_mask &= ~MOVES::BOTTOM_SIDE;	
			break;	//rm bottom side

		//sea
		case 0:		
			m_move_mask |= MOVES::O | MOVES::W; 
			break;
		case 12:	
			m_move_mask |= MOVES::O | MOVES::W; 
			break;
		}


	}

protected:
	QState* m_initial_state;
	QStateMachine* m_state_machine;
	int m_move_mask;

	void mousePressEvent(QGraphicsSceneMouseEvent *_event) override
	{
		auto wrapped = new QStateMachine::WrappedEvent(this, new QMouseEvent(QEvent::MouseButtonPress, _event->pos(), _event->button(), _event->buttons(), _event->modifiers()));
		m_state_machine->postEvent(wrapped);	//will be deleted inside
		RoundedRect::mousePressEvent(_event);
	}

private:
	std::unique_ptr<RectSelection> m_selection;

signals:
	void activated_called();
};
