#pragma once

#include <memory>

#include <QObject>

#include "IPlayer.h"

class IPlayerQueue : public QObject
{
	Q_OBJECT

public:
	static std::shared_ptr<IPlayerQueue> create();

	virtual ~IPlayerQueue() {};

	virtual void set_nord_player(std::shared_ptr<IPlayer> _player) = 0;
	virtual void set_east_player(std::shared_ptr<IPlayer> _player) = 0;
	virtual void set_south_player(std::shared_ptr<IPlayer> _player) = 0;
	virtual void set_west_player(std::shared_ptr<IPlayer> _player) = 0;
};