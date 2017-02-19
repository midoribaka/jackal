#pragma once

#include <memory>

#include "IPlayer.h"
#include "ShipItem.h"

class Player : public IPlayer
{
public:
	Player(IGridMap* _map, PlayerPos _pos, const QString& _name);

private:
	size_t m_coins;
	size_t m_rum;

	std::vector<PlayItem*> m_items_vec;

	IGridMap* m_map;

	QString m_name;
};