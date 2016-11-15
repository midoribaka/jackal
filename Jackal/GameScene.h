#pragma once

#include <QGraphicsScene>

#include "GridMap.h"

class GameScene : public QGraphicsScene
{
	const size_t corner_radius = 10;

public:
	GameScene(size_t _side_size, QGraphicsScene* _parent = 0) : QGraphicsScene(_parent)
	{
		addItem(new GridMap(_side_size, corner_radius));
	}
};