#include "PlayItem.h"
#include "ShipItem.h"
#include "PirateItem.h"

#include <functional>
#include <QDebug>

IPlayItem* IPlayItem::create(ItemType _type)
{
	switch (_type)
	{
	case ItemType::SHIP: return new ShipItem(QRectF(0, 0, 50, 50));	//todo hardcode
	case ItemType::PIRATE: return new PirateItem(QRectF(0, 0, 30, 30));	//todo hardcode
	}
}

PlayItem::PlayItem() : m_grid_pos(-1,-1)
{
	m_moving_animation = std::make_unique<QPropertyAnimation>(this, "common_pos");
	m_moving_animation->setEasingCurve(QEasingCurve::InOutQuart);

	//machine
	m_state_machine = new QStateMachine(this);		//owns

	QState* idle_state = new QState(m_state_machine);		//не реагирует на эвенты
	QState* active_state = new QState(m_state_machine);		//может быть выбран
	m_selected_state = new QState(m_state_machine);			//выбран

	idle_state->addTransition(this, &IPlayItem::activate, active_state);
	active_state->addTransition(this, &IPlayItem::desactivate, idle_state);
	m_selected_state->addTransition(this, &IPlayItem::deselect, active_state);

	QEventTransition *mouse_press = new QEventTransition(this, QEvent::MouseButtonPress, active_state);
	mouse_press->setTargetState(m_selected_state);

	QObject::connect(m_selected_state, &QState::entered, this, &IPlayItem::selected);
	QObject::connect(m_moving_animation.get(), &QPropertyAnimation::finished, this, &IPlayItem::moving_finished);	

	m_state_machine->setInitialState(idle_state);

	m_state_machine->start();
}

void PlayItem::move_to(const QPoint& _pos)
{
	QPoint current = pos().toPoint();
	QPoint d = _pos - current;
	m_moving_animation->setDuration(1000 * hypot(d.x(), d.y()) / 50);	//50px/1sec	//todo no constant

	m_moving_animation->setStartValue(current);
	m_moving_animation->setEndValue(_pos);

	deselect();
	desactivate();
	m_moving_animation->start();
}

PlayItem::~PlayItem()
{
}

void PlayItem::mousePressEvent(QGraphicsSceneMouseEvent *_event)
{
	auto wrapped = new QStateMachine::WrappedEvent(this, new QMouseEvent(QEvent::MouseButtonPress, _event->pos(), _event->button(), _event->buttons(), _event->modifiers()));
	m_state_machine->postEvent(wrapped);	//will be deleted inside
	AbstractShape::mousePressEvent(_event);
	_event->accept();
}

void PlayItem::set_grid_pos(const QPoint& _pos)
{
	m_grid_pos = _pos;
	emit grid_pos_changed();
}

QPoint PlayItem::grid_pos() const
{
	return m_grid_pos;
}

void PlayItem::grab_child_item(IPlayItem* _item)
{
	m_child_items.insert(qobject_cast<PlayItem*>(_item));
}

void PlayItem::release_child_item(IPlayItem* _item)
{
	m_child_items.erase(qobject_cast<PlayItem*>(_item));
}

QPoint PlayItem::common_pos() const
{
	return pos().toPoint();
}

void PlayItem::set_common_pos(const QPoint& _pos)
{
	setPos(_pos);
	for (auto& it : m_child_items)
		it->setPos(_pos);
}
