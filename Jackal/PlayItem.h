#pragma once

#include <memory>

#include <QStateMachine>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include <QMouseEventTransition>
#include <QSignalTransition>
#include <QPropertyAnimation>

#include "Selection.h"

#include "IPlayItem.h"

class PlayItem : public IPlayItem
{
	Q_OBJECT

public:
	PlayItem();

	void move_to(const QPointF& _pos);

	virtual ~PlayItem();

	virtual void paint(QPainter *_painter, const QStyleOptionGraphicsItem *_option, QWidget *_widget = Q_NULLPTR) override = 0;

	void mousePressEvent(QGraphicsSceneMouseEvent *_event) override;

protected:
	QState* m_selected_state;		//bind in childs to selection object

private:
	QStateMachine* m_state_machine;
	QPoint m_grid_pos;
	std::unique_ptr<QPropertyAnimation> m_moving;
};
