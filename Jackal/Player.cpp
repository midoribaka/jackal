
#include "Player.h"

Player::Player() : m_coins(0), m_rum(0)
{
	//will be owned by GameScene
	m_ship_item = new ShipItem(QRectF(0, 0, 40, 40));	//todo no constant
	m_pirate_item = new PirateItem(QRectF(0, 0, 20, 20));

	m_items_vec.push_back(m_ship_item);
	m_items_vec.push_back(m_pirate_item);
}

void Player::place_on_scene(GameScene* _scene, PlayerPos _pos, GridMap* _cell_grid)
{
	_scene->addItem(m_ship_item);	//owns by QGraphicsScene 
	_scene->addItem(m_pirate_item);	//owns by QGraphicsScene 

	QPoint pt;
	switch (_pos)
	{
	case PlayerPos::NORD:	pt.setX(6);		pt.setY(0);	break;
	case PlayerPos::OST:	pt.setX(12);	pt.setY(6);	break;
	case PlayerPos::WEST:	pt.setX(0);		pt.setY(6);	break;
	default: /*SOUTH*/		pt.setX(6);		pt.setY(12);
	}

	//
	m_ship_item->setPos(_cell_grid->get_cell(pt)->pos());
	m_pirate_item->setPos(_cell_grid->get_cell(pt)->pos());

	for (auto& _it : m_items_vec)
	{
		_it->set_grid_pos(pt);

		QObject::connect(_it, &PlayItem::choosed, [this, _it, _scene]()
		{
			//unselect all but m_ship_item
			for (auto& _other : m_items_vec)
			{
				if (_other != _it)
					_other->unselect();
			}

			current_item = _it;

			_scene->select_cells_around(current_item->grid_pos());
		});
	}
}

