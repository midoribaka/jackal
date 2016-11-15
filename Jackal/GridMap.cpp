#include "GridMap.h"

GridMap::GridMap(size_t _px_size, size_t _corner_radius, QGraphicsItem* _parent /*= 0*/) : RoundedRect(QRectF(QPointF(0,0), QSizeF(_px_size, _px_size)), _corner_radius, _parent)
{
	cells.resize(rows_num);

	cell_side_size = std::round((_px_size - 2 * padding_size - (rows_num - 1)*spacer_size)/(double)rows_num);	//todo сделать подбор padding в зависимости от разрешения

	for (int i = 0; i < rows_num; ++i)
	{
		cells[i].resize(rows_num);

		for (int j = 0; j < rows_num; ++j)
		{
			if ((i == 0) || (j == 0) || (i == rows_num - 1) || (j == rows_num - 1) || 
				((i == 1) && (j == 1)) || 
				((i == 1) && (j == rows_num - 2)) || 
				((i == rows_num - 2) && (j == 1)) || 
				((i == rows_num - 2) && (j == rows_num - 2)))

				cells[i][j] = new StaticCell(QRectF(0, 0, cell_side_size, cell_side_size), cell_corner_radius, this);
			else
				cells[i][j] = new FlippableCell(QRectF(0, 0, cell_side_size, cell_side_size), cell_corner_radius, this);
		
			cells[i][j]->setPos(get_cell_center(j,i));
		}
	}
}

Cell* GridMap::get_cell(size_t _x, size_t _y) const
{
	return cells[_x][_y];
}

QPointF GridMap::get_cell_center(size_t _x, size_t _y) const
{
	return QPointF(padding_size + cell_side_size/2 + _x*(spacer_size + cell_side_size), padding_size + cell_side_size / 2 + _y*(spacer_size + cell_side_size) );
}
