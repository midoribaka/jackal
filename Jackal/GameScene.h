#pragma once

#include <QGraphicsScene>

#include "GridMap.h"
#include "PlayItems.h"

class GameScene : public QGraphicsScene
{
	const size_t corner_radius = 0;

public:
	GameScene(size_t _side_size, QObject* _parent = 0) : QGraphicsScene(_parent)
	{
		m_grid_map = new GridMap(_side_size, corner_radius);
		addItem(m_grid_map);			//owns by QGraphicsScene

		m_chip = new PlayItem(PlayItem::COLOR::RED, QRectF(0, 0, 20, 20));
		addItem(m_chip);
		m_chip->setPos(m_grid_map->get_cell(6, 12)->pos());
	}

private:
	GridMap* m_grid_map;
	PlayItem* m_chip;
};