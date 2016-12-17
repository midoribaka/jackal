#pragma once

#include "PlayItem.h"
#include "ShipItem.h"
#include "PirateItem.h"
#include "GameScene.h"
#include "GridMap.h"

#include <memory>

class GameScene;
class Player : public QObject
{
	Q_OBJECT

public:
	enum class PlayerPos
	{
		WEST,
		NORD,
		OST,
		SOUTH
	};

	Player();
	void place_on_scene(GameScene* _scene, PlayerPos _pos, GridMap* _cell_grid);

private:
	size_t m_coins;
	size_t m_rum;

	//deleted by scene
	ShipItem* m_ship_item;
	PirateItem* m_pirate_item;

	PlayItem* current_item;
	std::vector<PlayItem*> m_items_vec;

signals:
	void item_choosed(const QPoint& _cell_on_grid);
};