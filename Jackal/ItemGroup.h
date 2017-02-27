#pragma once

#include <QObject>
#include <QGraphicsScene>

#include <functional>

#include "IPlayItem.h"

class ItemGroup : QObject
{
	Q_OBJECT
public:
	ItemGroup() : m_ship(nullptr), m_last_selected(nullptr), m_current_selected(nullptr)
	{
	}

	void reset()
	{
		QObject::disconnect(m_current_selected, &IPlayItem::moving_finished, 0, 0);

		m_last_selected = nullptr;
		m_current_selected = nullptr;
	}

	void add_item(IPlayItem* _item, ItemType _type)
	{
		if(_type == ItemType::SHIP)
			m_ship = _item;

		m_items.push_back(_item);

		
		QObject::connect(_item, &IPlayItem::grid_pos_changed, [this, _item]()
		{
			if ((_item != m_ship) && (m_ship->grid_pos() == _item->grid_pos()))
			{
				m_ship->grab_child_item(_item);
			}
			else if ((_item != m_ship) && (m_ship->grid_pos() != _item->grid_pos()))
			{
				m_ship->grab_child_item(_item);
			}
		});
		
	}

	void set_position(const QPoint& _px_pos)
	{
		for (auto& it : m_items)
			it->setPos(_px_pos);
	}

	void add_to_scene(QGraphicsScene* _scene)
	{
		for (auto& it : m_items)
			_scene->addItem(it);
	}

	IPlayItem* last_selected() const
	{
		return m_last_selected;
	}

	IPlayItem* current_selected() const
	{	
		return m_current_selected;
	}

	void emit_desactivate()
	{
		for (auto& it : m_items)
			it->desactivate();
	}

	void emit_activate()
	{
		for (auto& it : m_items)
			it->activate();
	}

	void set_item_selected_callback(const std::function<void(void)>& _fnc)
	{
		for (auto& it : m_items)
		{
			QObject::connect(it, &IPlayItem::selected, [_fnc, it, this]()
			{
				m_last_selected = m_current_selected;

				if (m_last_selected)
					m_last_selected->deselect();

				m_current_selected = it;

				QObject::disconnect(m_last_selected, &IPlayItem::moving_finished, 0, 0);
				QObject::connect(m_current_selected, &IPlayItem::moving_finished, m_finish_move_callback);

				_fnc();
			});
		}
	}

	void set_selected_item_finish_move_callback(const std::function<void(void)>& _fnc)
	{
		m_finish_move_callback = _fnc;
	}

private:
	std::function<void(void)> m_finish_move_callback;

	std::vector<IPlayItem*> m_items;
	IPlayItem* m_ship;
	IPlayItem* m_last_selected;
	IPlayItem* m_current_selected;
};