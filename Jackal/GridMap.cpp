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
	cells.resize(rn);

	size_t s = (_px_size - 2 * padding_size - (rn - 1)*spacer_size) / rn;	//floor
	cell_side_size = (s % 2) ? --s : s;	//долой нечётные числа

	//пересчёт размеров полей, чтобы исключить искажения из-за округления
	padding_size = (_px_size - cell_side_size*rn - (rn - 1)*spacer_size) / 2;		//todo работает довольно хреново

	std::unique_ptr<TreasureIslandDeck> deck(new TreasureIslandDeck());

	for (int i = 0; i < rn; ++i)
	{
		cells[i].resize(rn);

		for (int j = 0; j < rn; ++j)
		{
			if ((i == 0) || (j == 0) || (i == rn - 1) || (j == rn - 1))
				cells[i][j] = std::make_shared<SeaCell>();
			else if (((i == 1) && (j == 1)) || ((i == 1) && (j == rn - 2)) || ((i == rn - 2) && (j == 1)) || ((i == rn - 2) && (j == rn - 2)))
				cells[i][j] = std::make_shared<NoActionCell>();
			else
				cells[i][j] = deck->pop_one();

			cells[i][j]->setParentItem(this);			//todo bad because shared
			cells[i][j]->set_side_size(cell_side_size);
			cells[i][j]->setPos(grid_to_px(QPoint(j, i)));
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
	action_on_masked_cell(_grid_pos, [this](Cell* _cell)
	{
		//_cell->activate();	//todo
	});;
}

void GridMap::desactivate_cells_around(const QPoint& _grid_pos)
{
	action_on_masked_cell(_grid_pos, [this](Cell* _cell)
	{
		//_cell->desactivate();	//todo
	});;
}

void GridMap::action_on_masked_cell(const QPoint& _grid_pos, const std::function<void(Cell*)>& _fnc)
{
	//int mask = get_cell(_grid_pos)->mask();
	
	int mask; //todo
	size_t bit_counter = 0;
	size_t pow_counter = 0;
	
	while (mask > pow_counter)
	{
		pow_counter = 1 << bit_counter;		//2^bit_counter
	
		if (mask & pow_counter)
		{
			size_t i = bit_counter / mask_side;	//floor
			size_t j = bit_counter - i * mask_side;

			//_fnc(get_cell(QPoint(i, j));
		}
	
		++bit_counter;
	}
}
