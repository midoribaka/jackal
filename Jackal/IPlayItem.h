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
	void activate();
	void select();
	void make_idle();
};