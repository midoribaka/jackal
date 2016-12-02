#pragma once

#include <QGraphicsScene>

#include "GridMap.h"
#include "PlayItems.h"

class GameScene : public QGraphicsScene
{
	Q_OBJECT

	const size_t corner_radius = 0;

public:
	GameScene(size_t _side_size, QObject* _parent = 0) : QGraphicsScene(_parent)
	{
		m_grid_map = new GridMap(_side_size, corner_radius);
		addItem(m_grid_map);			//owns by QGraphicsScene

		m_ship = new ShipItem(this, QRectF(0, 0, 40, 40));
		addItem(m_ship);
		m_ship->setPos(m_grid_map->get_cell(6, 12)->pos());

		m_pirate = new PirateItem(this, QRectF(0, 0, 20, 20));
		addItem(m_pirate);
		m_pirate->setPos(m_grid_map->get_cell(6, 12)->pos());

		QStateMachine* machine = new QStateMachine(this);

	//	machine->setInitialState(begin);

	//	begin->addTransition(m_ship, SIGNAL(choosed()), m_ship->get_state_machine());
		
		emit deselect_items();
	}

private:
	GridMap* m_grid_map;
	PlayItem* m_pirate;
	PlayItem* m_ship;

signals:
	void deselect_items();
};