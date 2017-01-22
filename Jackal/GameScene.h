#pragma once

#include <vector>
#include <memory>

#include <QGraphicsScene>

#include "GridMap.h"
#include "Player.h"

class GameScene : public QGraphicsScene
{
	Q_OBJECT

	const size_t corner_radius = 0;

	enum class PlayerPos
	{
		WEST,
		NORD,
		OST,
		SOUTH
	};

public:
	GameScene(size_t _side_size, QObject* _parent = 0) : QGraphicsScene(_parent)
	{
		//Create grid
		m_grid_map = new GridMap(_side_size, corner_radius);
		addItem(m_grid_map);			//owns by QGraphicsScene

		//Create player	
		std::shared_ptr<Player> current_player = create_player(PlayerPos::SOUTH);

		QObject::connect(m_grid_map, &GridMap::cell_clicked, [current_player](ActionCell* _activated_cell)
		{
			_activated_cell->activate(current_player);
		});

		QObject::connect(current_player.get(), &Player::item_choosed, m_grid_map, &GridMap::select_cells_around);

		//см ссылку, иначе ничё не пашет
		//http://stackoverflow.com/questions/10059721/qt-qstatemachine-sync-problems-initial-state-not-set-on-started-signal
		QMetaObject::invokeMethod(current_player.get(), "choose", Qt::QueuedConnection);
	}

private:
	std::shared_ptr<Player> create_player(PlayerPos _pos)
	{
		std::shared_ptr<Player> player = std::make_shared<Player>();
		std::vector<PlayItem*> player_items = player->items();

		QPoint pt;
		switch (_pos)
		{
		case PlayerPos::NORD:	pt.setX(6);		pt.setY(0);	break;
		case PlayerPos::OST:	pt.setX(12);	pt.setY(6);	break;
		case PlayerPos::WEST:	pt.setX(0);		pt.setY(6);	break;
		default: /*SOUTH*/		pt.setX(6);		pt.setY(12);

		}
		for (auto& _it : player_items)
		{
			addItem(_it);		//owns by QGraphicsScene 
			_it->set_grid_pos(pt);
			_it->setPos(m_grid_map->get_cell(pt)->pos());
		}

		return player;
	}

	GridMap* m_grid_map;
	
};