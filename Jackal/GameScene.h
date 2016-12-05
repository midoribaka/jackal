#pragma once

#include <vector>

#include <QGraphicsScene>
#include <QTimer>

#include "GridMap.h"
#include "PlayItems.h"
#include "ShipItem.h"
#include "PirateItem.h"

class GameScene : public QGraphicsScene
{
	Q_OBJECT

	const size_t corner_radius = 0;

public:
	GameScene(size_t _side_size, QObject* _parent = 0) : QGraphicsScene(_parent)
	{
		m_grid_map = new GridMap(_side_size, corner_radius);
		addItem(m_grid_map);			//owns by QGraphicsScene

		auto add_new_play_item = [this](PlayItem* _item)
		{
			play_items.push_back(_item);
			addItem(_item);

			_item->setPos(m_grid_map->get_cell(6, 12)->pos());

			QObject::connect(_item, &PlayItem::choosed, [this, _item]()
			{
				//unselect all but _item
				for (auto& it : play_items)
				{
					if (it != _item)
						it->unselect();
				}
			});
		};

		add_new_play_item(new ShipItem(this, QRectF(0, 0, 40, 40)));	//todo no constant
		add_new_play_item(new PirateItem(this, QRectF(0, 0, 20, 20)));

		QTimer::singleShot(6000, [this]()
		{
			QPointF pf = m_grid_map->get_cell(6, 11)->pos();

			play_items[1]->move_to(pf);
		});
	}

private:
	GridMap* m_grid_map;
	std::vector<PlayItem*> play_items;
};