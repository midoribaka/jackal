#include "PlayItem.h"
#include "ShipItem.h"

IPlayItem* IPlayItem::create(ItemType _type)
{
	switch (_type)
	{
	case ItemType::SHIP: return new ShipItem(QRectF(0, 0, 40, 40));	//todo hardcode
	}
}

PlayItem::PlayItem()
{
	m_state_machine = new QStateMachine(this);		//owns

	QState* idle_state = new QState(m_state_machine);		//не реагирует на эвенты
	QState* active_state = new QState(m_state_machine);		//может быть выбран
	m_selected_state = new QState(m_state_machine);			//выбран
	QState* moving_state = new QState(m_state_machine);		//перемещается

	idle_state->addTransition(this, &IPlayItem::activate, active_state);
	active_state->addTransition(this, &IPlayItem::desactivate, idle_state);
	m_selected_state->addTransition(this, &IPlayItem::deselect, active_state);

	QEventTransition *mouse_press = new QEventTransition(this, QEvent::MouseButtonPress, active_state);
	mouse_press->setTargetState(m_selected_state);

	QObject::connect(m_selected_state, &QState::entered, this, &IPlayItem::selected);	//emit selected

	QObject::connect(m_selected_state, &QState::entered, []()
	{
		int t = 0;
	});	//emit selected

	//Moving animation
	//m_moving = std::make_unique<QPropertyAnimation>(this, "position");
	//m_moving->setEasingCurve(QEasingCurve::InOutQuart);
	//m_selected_state->addTransition(m_moving.get(), &QPropertyAnimation::finished, idle_state);

	m_state_machine->setInitialState(idle_state);

	m_state_machine->start();
}

QRectF PlayItem::boundingRect() const
{
	//origin at center
	QRectF br = AbstractShape::boundingRect();
	return br.translated(-br.width() / 2, -br.height() / 2);
}

void PlayItem::move_to(const QPointF& _pos)
{
	QPointF current = pos();
	QPointF d = _pos - current;
	m_moving->setDuration(1000 * hypot(d.x(), d.y()) / 50);	//50px/1sec	//todo no constant

	m_moving->setStartValue(pos());
	m_moving->setEndValue(_pos);
	m_moving->start();
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
