#pragma once

#include <memory>
#include <functional>

#include "IGridMap.h"
#include "ICell.h"
#include "Deck.h"

class GridMap : public IGridMap
{
	Q_OBJECT

	const size_t rows_num = 13;
	const size_t spacer_size = 4;
	const size_t mask_side = 5;

	size_t padding_size = 2;	//адаптивный размер полей сетки под разрешение экрана

public:
	GridMap(size_t _px_size, QGraphicsScene* _scene);
	QGraphicsScene* scene() const override;
	QPoint grid_to_px(const QPoint& _px_pos) const override;
	void activate_cells_around(const QPoint& _grid_pos) override;
	void desactivate_cells_around(const QPoint& _grid_pos) override;

private:
	void action_on_masked_cell(const QPoint& _grid_pos, const std::function<void(ICell*)>& _fnc);

	size_t cell_side_size;
	std::vector<std::vector<ICell*> > m_cells;
	QGraphicsScene* m_scene;
};