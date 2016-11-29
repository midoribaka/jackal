#pragma once

#include "Cell.h"

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

	PlayItem(COLOR _color, const QRectF& _draw_rect = QRectF(), size_t _radius = 0, RoundedRect* _parent = 0)
	{
		switch (_color)
		{
		case COLOR::RED: set_image(QPixmap("./Resources/items/chip_red.png"));	break;
		case COLOR::GREEN: break;
		case COLOR::BLUE: break;
		case COLOR::BLACK: break;
		}

		//StateMachine
		/*
		idle_state-> ->choosed_state
		choosed_state-> ->idle_state
		choosed_state-> ->moving_state
		moving_state-> ->idle_state
		*/

		QStateMachine* state_machine = new QStateMachine();

		QState* idle_state = new QState(state_machine);
		QState* choosed_state = new QState(state_machine);
		QState* moving_state = new QState(state_machine);

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