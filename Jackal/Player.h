#pragma once

#include "PlayItem.h"
#include "ShipItem.h"
#include "PirateItem.h"

#include <memory>

#include <QDebug>

class Player : public QObject
{
	Q_OBJECT

public:
	Player() : m_coins(0), m_rum(0)
	{
		//will be owned by GameScene
		m_items_vec.push_back(new ShipItem(QRectF(0, 0, 40, 40)));
		m_items_vec.push_back(new PirateItem(QRectF(0, 0, 20, 20)));

		for (auto& _it : m_items_vec)
		{
			QObject::connect(_it, &PlayItem::selected, [this, _it]()
			{
				//unselect all but _it
				for (auto& _other : m_items_vec)
				{
					if (_other != _it)
						_other->make_ready();
				}

				m_selected_item = _it;
				emit item_choosed(_it->grid_pos());
			});
		}

		//machine
		QStateMachine* state_machine = new QStateMachine(this);
		QState* idle_state = new QState(state_machine);
		QState* choosing_item_state = new QState(state_machine);
		QState* making_turn_state = new QState(state_machine);

		QObject::connect(choosing_item_state, &QState::entered, [this]		//make all items ready for selection
		{
			for (auto& it : m_items_vec)
				it->make_ready();
		});

		QObject::connect(idle_state, &QState::entered, [this]		//make all items idle
		{
			for (auto& it : m_items_vec)
				it->make_idle();
		});

		idle_state->addTransition(this, &Player::choose, choosing_item_state);
		making_turn_state->addTransition(this, &Player::finish_turn, idle_state);

		state_machine->setInitialState(idle_state);
		state_machine->start();
	}


	std::vector<PlayItem*> items() const
	{
		return m_items_vec;
	}

	void move_selected_item_to(const QPointF& _point)
	{
		m_selected_item->move_to(_point);
	}

	void set_selected_item_grid_pos(const QPoint& _grid_pos)
	{
		m_selected_item->set_grid_pos(_grid_pos);
	}

private:
	size_t m_coins;
	size_t m_rum;

	std::vector<PlayItem*> m_items_vec;
	PlayItem* m_selected_item;	//bad because it is LAST selected item (and all items may be unselected)

signals:
	void item_choosed(const QPoint& _cell_on_grid);

	void choose();
	void finish_turn();
};