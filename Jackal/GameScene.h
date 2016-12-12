#pragma once

#include <vector>

#include <QGraphicsScene>
#include <QTimer>

#include "GridMap.h"
#include "PlayItems.h"
#include "ShipItem.h"
#include "PirateItem.h"
#include "ActionCell.h"

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
				select_cells_around(QPoint(6,6));
			//	int cell_mask = item_cell->mask();

			});
		};

		add_new_play_item(new ShipItem(this, QRectF(0, 0, 40, 40)));	//todo no constant
		add_new_play_item(new PirateItem(this, QRectF(0, 0, 20, 20)));
	}

private:
	void select_cells_around(const QPoint& _grid_pos)
	{
	//	int mask = _center_cell->mask();
		int mask = 0xAFABEA;	//all
	//	QPoint center_cell_pos = _center_cell->grid_pos();

		size_t bit_counter = 0;
		size_t pow_counter;

		while (mask > pow_counter)
		{
			pow_counter = 1 << bit_counter;		//2^bit_counter

			if (mask & pow_counter)
			{
				size_t i = bit_counter / 5;	//floor
				size_t j = bit_counter - i * 5;

				//todo bad. out of range

				m_grid_map->get_cell(QPoint(_grid_pos.y()-2 + i, _grid_pos.x()-2+ j))->make_ready();
			}	

			++bit_counter;
		}
	
	};

	GridMap* m_grid_map;
	std::vector<PlayItem*> play_items;
};