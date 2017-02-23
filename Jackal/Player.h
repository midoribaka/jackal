#pragma once

#include <memory>

#include "IPlayer.h"
#include "IPlayItem.h"

class Player : public IPlayer
{
public:
	Player(IGridMap* _map, PlayerPos _pos, const QString& _name);

private:
	size_t m_coins;
	size_t m_rum;

	std::vector<IPlayItem*> m_items;
	IPlayItem* m_last_selected;
	QPoint m_last_point;		//todo ������ ���� � grid point � ��� item


	IGridMap* m_map;

	QString m_name;
};