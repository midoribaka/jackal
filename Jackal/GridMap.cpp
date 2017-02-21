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

	for (int i = 0; i < rn; ++i)
	{
		m_cells[i].resize(rn);

		for (int j = 0; j < rn; ++j)
		{
			if ((i == 0) || (j == 0) || (i == rn - 1) || (j == rn - 1))
				m_cells[i][j] = ICell::create(CellType::SEA);
			else if (((i == 1) && (j == 1)) || ((i == 1) && (j == rn - 2)) || ((i == rn - 2) && (j == 1)) || ((i == rn - 2) && (j == rn - 2)))
				m_cells[i][j] = ICell::create(CellType::CORNER);
			else
				m_cells[i][j] = deck->pop_one();

			m_cells[i][j]->setParentItem(this);			//owns
			m_cells[i][j]->set_side_size(cell_side_size);
			m_cells[i][j]->setPos(grid_to_px(QPoint(j, i)));
		}

	}
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
	BitMask cmask = m_cells[_grid_pos.x()][_grid_pos.y()]->mask();
	
	size_t size = cmask.size();

	for (int i = 0; i < size; ++i)
	{
		if (cmask.test(i))
		{
			size_t y = i / mask_side;	//floor
			size_t x = i - y * mask_side;

			_fnc(m_cells[x][y]);
		}
	}
}
