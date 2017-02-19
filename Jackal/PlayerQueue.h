#pragma once

#include "IPlayerQueue.h"

class PlayerQueue : public IPlayerQueue
{
	enum PlayerPos
	{
		NORD = 1,
		EAST = 2,
		SOUTH = 4,
		WEST = 8,
		ALL = NORD | EAST | SOUTH | WEST
	};

public:
	PlayerQueue();
	void set_nord_player(std::shared_ptr<IPlayer> _player) override;
	void set_east_player(std::shared_ptr<IPlayer> _player) override;
	void set_south_player(std::shared_ptr<IPlayer> _player) override;
	void set_west_player(std::shared_ptr<IPlayer> _player) override;

private:
	void reconnect_by_mask();

	std::shared_ptr<IPlayer> m_south_player, m_nord_player, m_east_player, m_west_player;	//немного тупо без контейнеров

	int m_mask;
};