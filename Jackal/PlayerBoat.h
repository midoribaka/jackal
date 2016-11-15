#pragma once

#include "PlayItem.h"

#include <QPainter>

class PlayerBoat : public PlayItem
{
public:
	PlayerBoat()
	{
		QPixmap pic(":/Resources/items/boat_h.png");

		setPixmap(pic.scaled(40, 20));
	}
};