#pragma once

#include <vector>
#include <memory>

#include <QGraphicsScene>

#include "GridMap.h"
#include "Player.h"

class Player;
class GameScene : public QGraphicsScene
{
	Q_OBJECT

	const size_t corner_radius = 0;

public:
	GameScene(size_t _side_size, QObject* _parent = 0);

public slots:
	void select_cells_around(const QPoint& _grid_pos);;

private:
	GridMap* m_grid_map;
	std::shared_ptr<Player> m_player;
};