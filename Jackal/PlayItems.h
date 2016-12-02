#pragma once

#include <memory>

#include <QStateMachine>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include <QMouseEventTransition>
#include <QSignalTransition>

#include "Selection.h"

class PlayItem : public RoundedRect
{
	Q_OBJECT

public:
	enum class COLOR
	{
		RED,
		GREEN, 
		BLUE,
		BLACK
	};

	QStateMachine* get_state_machine()
	{
		return m_state_machine;
	}

	PlayItem(QObject* _parent_obj, COLOR _color, const QRectF& _draw_rect = QRectF())
	{
		setParent(_parent_obj);

		RoundedRect::set_geometry(_draw_rect, 0);

		m_selection = std::make_unique<Selection>(this);	//must be after set_geometry


		switch (_color)
		{
		case COLOR::RED: set_image(QPixmap("./Resources/items/chip_red.png"));	break;
		case COLOR::GREEN: break;
		case COLOR::BLUE: set_image(QPixmap("./Resources/items/chip_blue.png"));	break;
		case COLOR::BLACK: break;
		}

		//Common StateMachine
		/*
		idle_state-> ->choosed_state
		choosed_state-> ->idle_state
		choosed_state-> ->moving_state
		moving_state-> ->idle_state
		*/
		m_state_machine = new QStateMachine(this);

		QState* idle_state = new QState(m_state_machine);
		QState* choosed_state = new QState(m_state_machine);
		QState* moving_state = new QState(m_state_machine);

		QEventTransition *mouse_press = new QEventTransition(this, QEvent::MouseButtonPress, idle_state);
		mouse_press->setTargetState(choosed_state);

		idle_state->assignProperty(m_selection.get(), "visible", false);
		choosed_state->assignProperty(m_selection.get(), "visible", true);

	//	QSignalTransition* deselect_signal = choosed_state->addTransition(_parent_obj, SIGNAL(deselect_items()), idle_state);	//todo add parent
		QObject::connect(choosed_state, &QState::entered, this, &PlayItem::choosed);	//emit signal to scene to deselect all another items 

		m_state_machine->setInitialState(idle_state);
	}

	virtual QRectF boundingRect() const override
	{
		//origin at center
		QRectF br = RoundedRect::boundingRect();
		return br.translated(-br.width() / 2, -br.height() / 2);
	}

protected:
	std::unique_ptr<Selection> m_selection;
	QStateMachine* m_state_machine;

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *_event) override
	{
		auto wrapped = new QStateMachine::WrappedEvent(this, new QMouseEvent(QEvent::MouseButtonPress, _event->pos(), _event->button(), _event->buttons(), _event->modifiers()));
		m_state_machine->postEvent(wrapped);	//will be deleted inside
		RoundedRect::mousePressEvent(_event);
		_event->accept();	//чтобы эвент не приходил нижнему item-у
	}

signals:
	void choosed();
};

class PirateItem : public PlayItem
{
public:
	PirateItem(QObject* _parent_obj, const QRectF& _draw_rect = QRectF()) : PlayItem(_parent_obj, COLOR::RED, _draw_rect)
	{
		m_state_machine->start();
	}

};


class ShipItem : public PlayItem
{
public:
	ShipItem(QObject* _parent_obj, const QRectF& _draw_rect = QRectF()) : PlayItem(_parent_obj, COLOR::BLUE, _draw_rect)
	{
		m_state_machine->start();
	}
};