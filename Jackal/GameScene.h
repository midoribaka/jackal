#pragma once

#include <vector>
#include <memory>

#include <QGraphicsScene>

#include "IGridMap.h"
#include "IPlayer.h"
#include "IPlayerQueue.h"

class GameScene : public QGraphicsScene
{
	Q_OBJECT

public:
	GameScene(size_t _side_size, QObject* _parent = 0) : QGraphicsScene(_parent)
	{
		IGridMap* m_grid_map = IGridMap::create(_side_size, this);	//owns by this

		std::shared_ptr<IPlayer> nord_player = IPlayer::create(m_grid_map, PlayerPos::NORD, "Den");
		std::shared_ptr<IPlayer> east_player = IPlayer::create(m_grid_map, PlayerPos::EAST, "Luba");
		std::shared_ptr<IPlayer> south_player = IPlayer::create(m_grid_map, PlayerPos::SOUTH, "Nata");
		std::shared_ptr<IPlayer> west_player = IPlayer::create(m_grid_map, PlayerPos::WEST, "Oleg");

		m_player_queue = IPlayerQueue::create();

		m_player_queue->set_nord_player(nord_player);
		m_player_queue->set_east_player(east_player);
		m_player_queue->set_south_player(south_player);
		m_player_queue->set_west_player(west_player);

		//см ссылку, иначе ничё не пашет
		//http://stackoverflow.com/questions/10059721/qt-qstatemachine-sync-problems-initial-state-not-set-on-started-signal
		QMetaObject::invokeMethod(south_player.get(), "select", Qt::QueuedConnection);
	}

private:
	std::shared_ptr<IPlayerQueue> m_player_queue;
};