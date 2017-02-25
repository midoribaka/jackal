#pragma once

#include <memory>

#include "IPlayer.h"
#include "IPlayItem.h"

#include "ItemGroup.h"

class Player : public IPlayer
{
	Q_OBJECT

public:
	Player(IGridMap* _map, PlayerPos _pos, const QString& _name);

private:
	size_t m_coins;
	size_t m_rum;

	std::unique_ptr<ItemGroup> m_item_group;
	IGridMap* m_map;
	QString m_name;

private slots:
	void cell_selected_callback(const ICell* _selected_cell);
};