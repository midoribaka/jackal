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

				//possible moves
				select_cells_around(_item->grid_pos());
			});
		};

		add_new_play_item(new ShipItem(this, QRectF(0, 0, 40, 40)));	//todo no constant
		add_new_play_item(new PirateItem(this, QRectF(0, 0, 20, 20)));
	}

private:
	void select_cells_around(const QPoint& _grid_pos)
	{
		const size_t mask_side = 5;
		int mask = m_grid_map->get_cell(_grid_pos)->mask();

		size_t bit_counter = 0;
		size_t pow_counter = 0;

		while (mask > pow_counter)
		{
			pow_counter = 1 << bit_counter;		//2^bit_counter

			if (mask & pow_counter)
			{
				size_t i = bit_counter / mask_side;	//floor
				size_t j = bit_counter - i * mask_side;

				//todo bad. out of range
				m_grid_map->get_cell(QPoint(_grid_pos.x()- mask_side/2 + j, _grid_pos.y()- mask_side/2 + i))->make_ready();
			}	

			++bit_counter;
		}
	
	};

	GridMap* m_grid_map;
	std::vector<PlayItem*> play_items;
};