#include "GridMap.h"

IGridMap* IGridMap::create(size_t _px_size, QGraphicsScene* _scene)
{
	return new GridMap(_px_size, _scene);
}

GridMap::GridMap(size_t _px_size, QGraphicsScene* _scene) : m_scene(_scene)
{
	m_scene->addItem(this);

	setZValue(0);

	RoundedRect::set_draw_rect(QRectF(QPointF(0, 0), QSizeF(_px_size, _px_size))); //todo this must be maked by scene

	const size_t rn = rows_num;
	m_cells.resize(rn);

	size_t s = (_px_size - 2 * padding_size - (rn - 1)*spacer_size) / rn;	//floor
	cell_side_size = (s % 2) ? --s : s;	//долой нечётные числа

	//пересчёт размеров полей, чтобы исключить искажения из-за округления
	padding_size = (_px_size - cell_side_size*rn - (rn - 1)*spacer_size) / 2;		//todo работает довольно хреново

	std::unique_ptr<TreasureIslandDeck> deck(new TreasureIslandDeck());

	/*
	0,12
	y
	^
	|
	|
	|
	0,0------------->x 12,0	
	*/

	//Создание клеток
	for (int x = 0; x < rn; ++x)
	{
		m_cells[x].resize(rn);

		for (int y = 0; y < rn; ++y)
		{
			ICell* cell = nullptr;
			if ((y == 0) || (x == 0) || (y == rn - 1) || (x == rn - 1))
				cell = ICell::create(CellType::SEA);
			else if (((y == 1) && (x == 1)) || ((y == 1) && (x == rn - 2)) || ((y == rn - 2) && (x == 1)) || ((y == rn - 2) && (x == rn - 2)))
				cell = ICell::create(CellType::CORNER);
			else
				cell = deck->pop_one();

			cell->setParentItem(this);			//owns
			cell->set_side_size(cell_side_size);
			cell->setPos(grid_to_px(QPoint(x, y)));

			QObject::connect(cell, &ICell::selected, this, std::bind(&IGridMap::cell_selected, this, cell));

			m_cells[x][y] = cell;
		}
	}

	//Коррекция масок todo сделать всё в одном цикле
	for (int x = 0; x < rn; ++x)
	{
		for (int y = 0; y < rn; ++y)
		{
			Mask cmask;

			if (y == 0 || y == 12)
			{
				if (x == 1)
					cmask = mask::right();
				else if (x == 11)
					cmask = mask::left();
				else
					cmask = mask::left_right();
			}

			if (x == 0 || x == 12)
			{
				if (y == 1)
					cmask = mask::top();
				else if (y == 11)
					cmask = mask::bottom();
				else
					cmask = mask::top_bottom();
			}

			m_cells[x][y]->set_mask(cmask);
		}
	}

}

QGraphicsScene* GridMap::scene() const
{
	return m_scene;
}

QPoint GridMap::grid_to_px(const QPoint& _grid_pos) const
{
	return QPoint(padding_size + cell_side_size / 2 + _grid_pos.x()*(spacer_size + cell_side_size), padding_size + cell_side_size / 2 + (rows_num-1-_grid_pos.y())*(spacer_size + cell_side_size));
}

QPoint GridMap::px_to_grid(const QPoint& _grid_pos) const
{
	return QPoint((_grid_pos.x() - padding_size) / (cell_side_size + spacer_size), (rows_num - 1) - (_grid_pos.y() - padding_size) / (cell_side_size + spacer_size));
}

void GridMap::activate_cells_around(const QPoint& _grid_pos)
{
	action_on_masked_cell(_grid_pos, [this](ICell* _cell)
	{
		QMetaObject::invokeMethod(_cell, "activate", Qt::QueuedConnection);
	});;
}

void GridMap::desactivate_cells_around(const QPoint& _grid_pos)
{
	action_on_masked_cell(_grid_pos, [this](ICell* _cell)
	{
		QMetaObject::invokeMethod(_cell, "desactivate", Qt::QueuedConnection);
	});;
}

void GridMap::action_on_masked_cell(const QPoint& _grid_pos, const std::function<void(ICell*)>& _fnc)
{
	Mask cmask = m_cells[_grid_pos.x()][_grid_pos.y()]->mask();
	
	size_t size = cmask.size();

	for (auto& it : cmask)
	{
		size_t x = _grid_pos.x() + it.first;
		size_t y = _grid_pos.y() + it.second;

		_fnc(m_cells[x][y]);
	}
}
