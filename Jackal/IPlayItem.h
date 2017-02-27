#pragma once

#include "CenteredRoundedRect.h"

enum class ItemType
{
	PIRATE,
	SHIP
};

class IPlayItem : public CenteredAbstractShape
{
	Q_OBJECT

public:
	static IPlayItem* create(ItemType _type);

	virtual void paint(QPainter *_painter, const QStyleOptionGraphicsItem *_option, QWidget *_widget = Q_NULLPTR) override = 0;

	virtual ~IPlayItem() {};

	virtual void move_to(const QPoint& _pos) = 0;

	virtual void set_grid_pos(const QPoint& _pos) = 0;

	virtual QPoint grid_pos() const = 0;

	virtual void grab_child_item(IPlayItem* _item) = 0;

	virtual void release_child_item(IPlayItem* _item) = 0;

signals:
	//iface
	void activate();	//from idle to active state
	void desactivate();	//from active to idle state
	void select();		//from active to selected state
	void deselect();	//from selected to active state

	//signals
	void selected();
	void moving_finished();
	void grid_pos_changed();

};