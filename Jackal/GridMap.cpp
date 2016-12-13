#include "GridMap.h"
#include "Deck.h"
#include "NoActionCell.h"

#include <memory>

#include <QTimer>

GridMap::GridMap(size_t _px_size, size_t _corner_radius, QGraphicsItem* _parent /*= 0*/)
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
			else if	(((i == 1) && (j == 1)) ||
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

			cells[i][j]->setParentItem(this);		//owns by scene
			cells[i][j]->set_grid_pos(QPoint(j,i));
			cells[i][j]->set_side_size(cell_side_size);
			cells[i][j]->setPos(grid_to_px(j, i));
		}

	}
}

std::shared_ptr<Cell> GridMap::get_cell(const QPoint& _point) const
{
	return cells[_point.y()][_point.x()];
}

QPointF GridMap::grid_to_px(size_t _x, size_t _y) const
{
	return QPointF(padding_size + cell_side_size/2 + _x*(spacer_size + cell_side_size), padding_size + cell_side_size / 2 + _y*(spacer_size + cell_side_size) );
}
