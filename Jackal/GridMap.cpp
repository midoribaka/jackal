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
	|
	0,0------------->x 12,0	
	*/
	for (int x = 0; x < rn; ++x)
	{
		m_cells[x].resize(rn);

		for (int y = 0; y < rn; ++y)
		{
			if ((y == 0) || (x == 0) || (y == rn - 1) || (x == rn - 1))
				m_cells[x][y] = ICell::create(CellType::SEA);
			else if (((y == 1) && (x == 1)) || ((y == 1) && (x == rn - 2)) || ((y == rn - 2) && (x == 1)) || ((y == rn - 2) && (x == rn - 2)))
				m_cells[x][y] = ICell::create(CellType::CORNER);
			else
				m_cells[x][y] = deck->pop_one();

			m_cells[x][y]->setParentItem(this);			//owns
			m_cells[x][y]->set_side_size(cell_side_size);
			m_cells[x][y]->setPos(grid_to_px(QPoint(x, y)));
		}
	}


	m_cells[1][6]->set_mask(mask::left_right());
	activate_cells_around(QPoint(1,6));
}

QGraphicsScene* GridMap::scene() const
{
	return m_scene;
}

QPoint GridMap::grid_to_px(const QPoint& _px_pos) const
{
	return QPoint(padding_size + cell_side_size / 2 + _px_pos.x()*(spacer_size + cell_side_size), padding_size + cell_side_size / 2 + _px_pos.y()*(spacer_size + cell_side_size));
}

void GridMap::activate_cells_around(const QPoint& _grid_pos)
{
	action_on_masked_cell(_grid_pos, [this](ICell* _cell)
	{
		_cell->activate();
	});;
}

void GridMap::desactivate_cells_around(const QPoint& _grid_pos)
{
	action_on_masked_cell(_grid_pos, [this](ICell* _cell)
	{
		_cell->desactivate();
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
