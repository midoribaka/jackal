#include "PlayerQueue.h"

std::shared_ptr<IPlayerQueue> IPlayerQueue::create()
{
	return std::make_shared<PlayerQueue>();
}

PlayerQueue::PlayerQueue() : m_mask(0)
{
}

void PlayerQueue::set_nord_player(std::shared_ptr<IPlayer> _player)
{
	m_nord_player = _player;
	m_mask |= NORD;
	reconnect_by_mask();
}

void PlayerQueue::set_east_player(std::shared_ptr<IPlayer> _player)
{
	m_east_player = _player;
	m_mask |= EAST;
	reconnect_by_mask();
}

void PlayerQueue::set_south_player(std::shared_ptr<IPlayer> _player)
{
	m_south_player = _player;
	m_mask |= SOUTH;
	reconnect_by_mask();
}

void PlayerQueue::set_west_player(std::shared_ptr<IPlayer> _player)
{
	m_west_player = _player;
	m_mask |= WEST;
	reconnect_by_mask();
}

void PlayerQueue::reconnect_by_mask()
{
	if (m_mask == ALL)
	{
		QObject::connect(m_nord_player.get(), &IPlayer::turn_finished, m_east_player.get(), &IPlayer::select);
		QObject::connect(m_east_player.get(), &IPlayer::turn_finished, m_south_player.get(), &IPlayer::select);
		QObject::connect(m_south_player.get(), &IPlayer::turn_finished, m_west_player.get(), &IPlayer::select);
		QObject::connect(m_west_player.get(), &IPlayer::turn_finished, m_nord_player.get(), &IPlayer::select);
	}
}
