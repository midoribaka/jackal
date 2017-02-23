#include "Player.h"

#include <QDebug>
#include <QStateMachine>

std::shared_ptr<IPlayer> IPlayer::create(IGridMap* _map, PlayerPos _pos, const QString& _name)
{
	return std::make_shared<Player>(_map, _pos, _name);
}

Player::Player(IGridMap* _map, PlayerPos _pos, const QString& _name) : m_coins(0), m_rum(0), m_map(_map), m_name(_name), m_last_selected(nullptr)
{
	//machine
	QStateMachine* state_machine = new QStateMachine(this);

	QState* idle_state = new QState(state_machine);				//игрок не активен
	QState* making_turn_state = new QState(state_machine);		//игрок делает ход

	//items
	IPlayItem* ship = IPlayItem::create(ItemType::SHIP);		//todo hardcode

	IPlayItem* ship2;
	switch (_pos)
	{
	case PlayerPos::NORD:	ship->setPos(m_map->grid_to_px(QPoint(6, 12))); break;	//todo hardcode
	case PlayerPos::EAST:	ship->setPos(m_map->grid_to_px(QPoint(12, 6)));	break;
	case PlayerPos::SOUTH:	ship->setPos(m_map->grid_to_px(QPoint(6, 0)));
		ship2 = IPlayItem::create(ItemType::SHIP);						//just test
		ship2->setPos(m_map->grid_to_px(QPoint(5, 0))); 
		m_items.push_back(ship2);	
		break;
	case PlayerPos::WEST:	ship->setPos(m_map->grid_to_px(QPoint(0, 6)));	break;
	}

	m_items.push_back(ship);	

	//настройка items
	for (auto& it : m_items)
	{
		m_map->scene()->addItem(it);	//owns

		//что делать, когда item выбран для хода
		QObject::connect(it, &IPlayItem::selected, [it, this]()
		{
			qDebug() << "PlayItem selected";

			//убираем старые клетки и делаем deselect последнего item-а
			if (m_last_selected)
			{
				m_map->desactivate_cells_around(m_last_point);
				m_last_selected->deselect();
			}
				
			//выделяем новые клетки
			m_last_selected = it;
			m_last_point = m_map->px_to_grid(it->pos().toPoint());
			m_map->activate_cells_around(m_last_point);
		});

		//что делать когда item закончил движение на новой клетке
		QObject::connect(it, &IPlayItem::moving_finished, this, &Player::playitem_finish_moving_callback);
	}

	QObject::connect(making_turn_state, &QState::entered, [this]
	{
		qDebug() << "Player " + m_name + " in making_turn_state";

		for (auto& it : m_items)
			it->activate();

		QObject::connect(m_map, &IGridMap::cell_selected, this, &Player::cell_selected_callback);
	});

	QObject::connect(making_turn_state, &QState::exited, [this]
	{
		qDebug() << "Player " + m_name + " leaves making_turn_state";
		m_last_selected = nullptr;
		QObject::disconnect(m_map, &IGridMap::cell_selected, this, &Player::cell_selected_callback);
	});

	QObject::connect(idle_state, &QState::entered, [this]
	{
		qDebug() << "Player " + m_name + " in idle_state";	
	});

	idle_state->addTransition(this, &IPlayer::select, making_turn_state);
	making_turn_state->addTransition(this, &IPlayer::deselect, idle_state);

	state_machine->setInitialState(idle_state);
	state_machine->start();
}

void Player::cell_selected_callback(const ICell* _selected_cell)
{
	qDebug() << "Cell selected";

	for (auto& it : m_items)		//all items in active state become idle except the one in selected state
		it->desactivate();

	m_map->desactivate_cells_around(m_last_point);

	/*

	std::shared_ptr<QMetaObject::Connection> single_shot_connection = std::make_shared<QMetaObject::Connection>();

	*single_shot_connection = QObject::connect(_selected_cell, &ICell::preparation_finished, [conn]()
	{
		m_last_selected->move_to(_selected_cell->pos().toPoint());
		QObject::disconnect(*single_shot_connection);
	}

	*/
	//prepare cell

	m_last_selected->move_to(_selected_cell->pos().toPoint());
}

void Player::playitem_finish_moving_callback()
{
	QPoint new_grid_pos = m_map->px_to_grid(m_last_selected->pos().toPoint());

	//run cell_action

	emit turn_finished();	//next player turn begins
}
