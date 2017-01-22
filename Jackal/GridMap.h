#pragma once

#include <QVector>

#include <memory>

#include "RoundedRect.h"
#include "Cell.h"
#include "ActionCell.h"
#include "NoActionCell.h"
#include "Deck.h"

class GridMap : public RoundedRect
{
	Q_OBJECT

	const size_t rows_num = 13;
	const size_t cell_corner_radius = 0;	//todo не работает
	const size_t spacer_size = 4;

	size_t padding_size = 2;	//адаптивный размер полей сетки под разрешение экрана

public:
	GridMap(size_t _px_size, size_t _corner_radius, QGraphicsItem* _parent = 0)
	{
		setZValue(0);

		RoundedRect::set_draw_rect(QRectF(QPointF(0, 0), QSizeF(_px_size, _px_size))); //todo this must be maked by scene
		RoundedRect::set_corner_radius(_corner_radius);

		cells.resize(rows_num);

		size_t s = (_px_size - 2 * padding_size - (rows_num - 1)*spacer_size) / rows_num;	//floor
		cell_side_size = (s % 2) ? --s : s;	//долой нечётные числа

											//пересчёт размеров полей, чтобы исключить искажения из-за округления
		padding_size = (_px_size - cell_side_size*rows_num - (rows_num - 1)*spacer_size) / 2;		//todo работает довольно хреново

		std::unique_ptr<TreasureIslandDeck> deck(new TreasureIslandDeck());

		for (int i = 0; i < rows_num; ++i)
		{
			cells[i].resize(rows_num);

			for (int j = 0; j < rows_num; ++j)
			{
				if ((i == 0) || (j == 0) || (i == rows_num - 1) || (j == rows_num - 1))
				{
					cells[i][j] = std::make_shared<SeaCell>();
				}
				else if (((i == 1) && (j == 1)) ||
					((i == 1) && (j == rows_num - 2)) ||
					((i == rows_num - 2) && (j == 1)) ||
					((i == rows_num - 2) && (j == rows_num - 2)))
				{
					cells[i][j] = std::make_shared<NoActionCell>();
				}
				else
				{
					cells[i][j] = deck->pop_one();
				}

				ActionCell* casted = qobject_cast<ActionCell*>(cells[i][j].get());

				if (casted)
				{
					QObject::connect(casted, &ActionCell::activated_called, [this, casted]
					{
						emit cell_clicked(casted);	// to scene
					});
				}

				cells[i][j]->setParentItem(this);			//todo bad because shared
				cells[i][j]->set_grid_pos(QPoint(j, i));
				cells[i][j]->set_side_size(cell_side_size);
				cells[i][j]->setPos(grid_to_px(j, i));
			}

		}
	}

	std::shared_ptr<Cell> get_cell(const QPoint& _point) const
	{
		return cells[_point.y()][_point.x()];
	}

	void select_cells_around(const QPoint& _grid_pos)
	{
		deselect_cells();

		const size_t mask_side = 5;
		int mask = get_cell(_grid_pos)->mask();

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
				last_selected_cells.push_back(get_cell(QPoint(_grid_pos.x() - mask_side / 2 + j, _grid_pos.y() - mask_side / 2 + i)));
				last_selected_cells.back()->make_ready();
			}

			++bit_counter;
		}
	}

	void deselect_cells()
	{
		for (auto& it : last_selected_cells)
			it->make_idle();
	}

private:
	QPointF grid_to_px(size_t _x, size_t _y) const
	{
		return QPointF(padding_size + cell_side_size / 2 + _x*(spacer_size + cell_side_size), padding_size + cell_side_size / 2 + _y*(spacer_size + cell_side_size));
	}

	size_t cell_side_size;
	QVector<QVector<std::shared_ptr<Cell> > > cells;
	QVector<std::shared_ptr<Cell> > last_selected_cells;

signals:
	void cell_clicked(ActionCell* _cell);
};