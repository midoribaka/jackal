#pragma once

#include "Cell.h"

#include <QApplication>
#include <QDrag>
#include <QMimeData>

class PlayItem : public RoundedRect
{
public:
	enum class COLOR
	{
		RED,
		GREEN, 
		BLUE,
		BLACK
	};

	PlayItem(COLOR _color, const QRectF& _draw_rect = QRectF(), size_t _radius = 0, RoundedRect* _parent = 0) : RoundedRect(_draw_rect, _radius, _parent)
	{
		switch (_color)
		{
		case COLOR::RED: set_image(QPixmap("./Resources/items/chip_red.png"));	break;
		case COLOR::GREEN: break;
		case COLOR::BLUE: break;
		case COLOR::BLACK: break;
		}
	}

	virtual QRectF boundingRect() const override
	{
		//origin at center
		QRectF br = RoundedRect::boundingRect();
		return br.translated(-br.width() / 2, -br.height() / 2);
	}
	
	void get_moves_from_cell(const Cell* _cell)
	{

	}
};