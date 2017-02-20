#include "Player.h"

#include <QDebug>
#include <QStateMachine>

std::shared_ptr<IPlayer> IPlayer::create(IGridMap* _map, PlayerPos _pos, const QString& _name)
{
	return std::make_shared<Player>(_map, _pos, _name);
}

Player::Player(IGridMap* _map, PlayerPos _pos, const QString& _name) : m_coins(0), m_rum(0), m_map(_map), m_name(_name)
{
	//items
	IPlayItem* ship = IPlayItem::create(ItemType::SHIP);		//todo hardcode

	IPlayItem* ship2;
	switch (_pos)
	{
	case PlayerPos::NORD:	ship->setPos(m_map->grid_to_px(QPoint(6, 0))); break;	//todo hardcode
	case PlayerPos::EAST:	ship->setPos(m_map->grid_to_px(QPoint(12, 6)));	break;
	case PlayerPos::SOUTH:	ship->setPos(m_map->grid_to_px(QPoint(6, 12)));
		ship2 = IPlayItem::create(ItemType::SHIP);						//just test
		ship2->setPos(m_map->grid_to_px(QPoint(5, 12))); 
		m_items.push_back(ship2);	
		break;
	case PlayerPos::WEST:	ship->setPos(m_map->grid_to_px(QPoint(0, 6)));	break;
	}

	m_items.push_back(ship);	

	//настройка items
	for (auto& it : m_items)
	{
		m_map->scene()->addItem(it);	//owns

		QObject::connect(it, &IPlayItem::selected, [it, this]()		//перевести все невыбранные item's в состояние active
		{
			for (auto& it2 : m_items)
			{
				if (it2 != it)
					it2->deselect();
			}
		});
	}

	//machine
	QStateMachine* state_machine = new QStateMachine(this);

	QState* idle_state = new QState(state_machine);				//игрок не активен
	QState* making_turn_state = new QState(state_machine);		//игрок делает ход

	QObject::connect(making_turn_state, &QState::entered, [this]
	{
		qDebug() << "Player " + m_name + " in making_turn_state";

		for (auto& it : m_items)
			it->activate();
	});

	QObject::connect(making_turn_state, &QState::exited, [this]
	{
		qDebug() << "Player " + m_name + " leaves making_turn_state";

		for (auto& it : m_items)
			it->desactivate();
	});

	QObject::connect(idle_state, &QState::entered, [this]
	{
		qDebug() << "Player " + m_name + " in idle_state";
	});

	idle_state->addTransition(this, &IPlayer::select, making_turn_state);

	state_machine->setInitialState(idle_state);
	state_machine->start();
}
