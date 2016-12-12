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

			QPoint pt(6, 12);

			_item->setPos(m_grid_map->get_cell(pt)->pos());
			_item->set_grid_pos(pt);

			QObject::connect(_item, &PlayItem::choosed, [this, _item]()
			{
				//unselect all but _item
				for (auto& it : play_items)
				{
					if (it != _item)
						it->unselect();
				}

				//
				std::shared_ptr<Cell> item_cell = m_grid_map->get_cell(_item->grid_pos());
			//	int cell_mask = item_cell->mask();

			});
		};

		add_new_play_item(new ShipItem(this, QRectF(0, 0, 40, 40)));	//todo no constant
		add_new_play_item(new PirateItem(this, QRectF(0, 0, 20, 20)));

		//QTimer::singleShot(6000, [this]()
		//{
		//	QPointF pf = m_grid_map->get_cell(6, 11)->pos();

		//	play_items[1]->move_to(pf);
		//});
	}

private:
	void select_cells_around(const std::shared_ptr<Cell>& _center_cell)
	{
	//	int mask = _center_cell->mask();
		int mask = 0;
		QPoint center_cell_pos = _center_cell->grid_pos();

		size_t counter = 1;
		while (mask > counter)
		{
			size_t test = mask & counter;
			if (test)
			{
				size_t n = log2(test);
				size_t i = n / 3;	//floor
				size_t j = n - i * 3;

			//	m_grid_map[i][j]->set_
			}
			
		}

			
	};

	GridMap* m_grid_map;
	std::vector<PlayItem*> play_items;
};