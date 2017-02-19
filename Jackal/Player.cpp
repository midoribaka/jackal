#include "Player.h"

#include <QDebug>

std::shared_ptr<IPlayer> IPlayer::create(IGridMap* _map, PlayerPos _pos, const QString& _name)
{
	return std::make_shared<Player>(_map, _pos, _name);
}

Player::Player(IGridMap* _map, PlayerPos _pos, const QString& _name) : m_coins(0), m_rum(0), m_map(_map), m_name(_name)
{
	//items
	ShipItem* ship = new ShipItem(QRectF(0, 0, 40, 40));		//todo hardcode

	m_map->scene()->addItem(ship);	//owns

	switch (_pos)
	{
	case PlayerPos::NORD:	ship->setPos(m_map->grid_to_px(QPoint(6, 0)));	break;	//todo hardcode
	case PlayerPos::EAST:	ship->setPos(m_map->grid_to_px(QPoint(12, 6)));	break;
	case PlayerPos::SOUTH:	ship->setPos(m_map->grid_to_px(QPoint(6, 12)));	break;
	case PlayerPos::WEST:	ship->setPos(m_map->grid_to_px(QPoint(0, 6)));	break;
	}

	//machine
	QStateMachine* state_machine = new QStateMachine(this);

	QState* idle_state = new QState(state_machine);				//игрок не активен
	QState* making_turn_state = new QState(state_machine);		//игрок делает ход

	QObject::connect(making_turn_state, &QState::entered, [this]
	{
		qDebug() << "Player " + m_name + " in making_turn_state";
	});

	QObject::connect(idle_state, &QState::entered, [this]
	{
		qDebug() << "Player " + m_name + " in idle_state";
	});

	idle_state->addTransition(this, &IPlayer::select, making_turn_state);

	state_machine->setInitialState(idle_state);
	state_machine->start();
}
