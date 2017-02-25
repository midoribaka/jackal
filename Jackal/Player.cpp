#include "Player.h"

#include <QDebug>
#include <QStateMachine>

std::shared_ptr<IPlayer> IPlayer::create(IGridMap* _map, PlayerPos _pos, const QString& _name)
{
	return std::make_shared<Player>(_map, _pos, _name);
}

Player::Player(IGridMap* _map, PlayerPos _pos, const QString& _name) : m_coins(0), m_rum(0), m_map(_map), m_name(_name)
{
	//machine
	QStateMachine* state_machine = new QStateMachine(this);

	QState* idle_state = new QState(state_machine);				//игрок не активен
	QState* making_turn_state = new QState(state_machine);		//игрок делает ход

	idle_state->addTransition(this, &IPlayer::select, making_turn_state);
	making_turn_state->addTransition(this, &IPlayer::deselect, idle_state);

	state_machine->setInitialState(idle_state);

	//items
	m_item_group = std::make_unique<ItemGroup>();
	m_item_group->add_item(IPlayItem::create(ItemType::SHIP), ItemType::SHIP);
	m_item_group->add_item(IPlayItem::create(ItemType::PIRATE), ItemType::PIRATE);

	switch (_pos)
	{
	case PlayerPos::NORD:	m_item_group->set_position(m_map->grid_to_px(QPoint(6, 12)));	break;	//todo hardcode
	case PlayerPos::EAST:	m_item_group->set_position(m_map->grid_to_px(QPoint(12, 6)));	break;
	case PlayerPos::SOUTH:	m_item_group->set_position(m_map->grid_to_px(QPoint(6, 0)));	break;
	case PlayerPos::WEST:	m_item_group->set_position(m_map->grid_to_px(QPoint(0, 6)));	break;
	}

	m_item_group->add_to_scene(m_map->scene());		//owns

	//ITEM SELECTED CALLBACK
	m_item_group->set_item_selected_callback([this]()
	{	
		qDebug() << "PlayItem selected";

		m_map->desactivate_cells_around(m_item_group->last_selected());	//убираем старые клетки и делаем deselect последнего item-а	
		m_map->activate_cells_around(m_item_group->current_selected());	//выделяем новые клетки
	});

	//ITEM FINISH MOVING CALLBACK
	m_item_group->set_selected_item_finish_move_callback([this]
	{
		m_map->run_cell_action(m_map->grid_pos(m_item_group->current_selected()));

		qDebug() << "Turn finished";
		emit turn_finished();	//next player turn begins
	});

	//PLAYER TURN BEGIN
	QObject::connect(making_turn_state, &QState::entered, [this]
	{
		qDebug() << "Player " + m_name + " in making_turn_state";

		m_item_group->emit_activate();	//make items selectable
		QObject::connect(m_map, &IGridMap::cell_ready, this, &Player::cell_selected_callback);
	});

	//PLAYER TURN END
	QObject::connect(making_turn_state, &QState::exited, [this]
	{
		m_item_group->reset();	//last_selected = current_selected = nullptr; current_selected to idle_state
		QObject::disconnect(m_map, &IGridMap::cell_ready, this, &Player::cell_selected_callback);
	});

	state_machine->start();
}

void Player::cell_selected_callback(const ICell* _selected_cell)
{
	qDebug() << "Cell selected";

	IPlayItem* selected_item = m_item_group->current_selected();

	m_item_group->emit_desactivate();	//all items in active state become idle except the one in selected state
	m_map->desactivate_cells_around(selected_item);

	selected_item->move_to(_selected_cell->pos().toPoint());
}