#pragma once
#include <QObject>

#include <memory>

#include "IGridMap.h"

enum class PlayerPos
{
	NORD,
	SOUTH,
	EAST,
	WEST
};

class IPlayer : public QObject
{
	Q_OBJECT

public:
	static std::shared_ptr<IPlayer> create(IGridMap* _map, PlayerPos _pos, const QString& _name);

	virtual ~IPlayer() {};

signals:
	//emits
	void turn_finished();

	//iface functions
	void select();	//to making_turn_state
	void deselect();//to idle_state
};