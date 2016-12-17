#include "GameScene.h"

GameScene::GameScene(size_t _side_size, QObject* _parent /*= 0*/) : QGraphicsScene(_parent)
{
	//Create grid
	m_grid_map = new GridMap(_side_size, corner_radius);

	QObject::connect(m_grid_map, &GridMap::cell_clicked, [](ActionCell* _activated_cell)
	{
		_activated_cell->activate();
	});

	addItem(m_grid_map);			//owns by QGraphicsScene

	//Create player
	m_player = std::make_shared<Player>();
	m_player->place_on_scene(this, Player::PlayerPos::SOUTH, m_grid_map);
}

void GameScene::select_cells_around(const QPoint& _grid_pos)
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
			m_grid_map->get_cell(QPoint(_grid_pos.x() - mask_side / 2 + j, _grid_pos.y() - mask_side / 2 + i))->make_ready();
		}

		++bit_counter;
	}
}
