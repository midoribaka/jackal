#pragma once

#include "PlayItems.h"
#include "ShipItem.h"

#include <memory>

class Player
{
public:
	Player() : m_coins(0), m_rum(0)
	{

	}

private:
	size_t m_coins;
	size_t m_rum;

	std::shared_ptr<ShipItem> m_ship_item;


};