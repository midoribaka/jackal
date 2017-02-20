#pragma once

#include "AbstractShape.h"

enum class ItemType
{
	PIRATE,
	SHIP
};

class IPlayItem : public AbstractShape
{
	Q_OBJECT

public:
	static IPlayItem* create(ItemType _type);

	virtual void paint(QPainter *_painter, const QStyleOptionGraphicsItem *_option, QWidget *_widget = Q_NULLPTR) override = 0;

	virtual QRectF boundingRect() const override = 0;

signals:
	//iface
	void activate();	//from idle to active state
	void desactivate();	//from active to idle state
	void select();		//from active to selected state
	void deselect();	//from selected to active state

	void selected();
};