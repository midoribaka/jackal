#pragma once

#include <memory>
#include <set>

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
	Q_PROPERTY(QPoint common_pos READ common_pos WRITE set_common_pos)

public:
	PlayItem();

	void move_to(const QPoint& _pos) override;

	virtual ~PlayItem();

	virtual void paint(QPainter *_painter, const QStyleOptionGraphicsItem *_option, QWidget *_widget = Q_NULLPTR) override = 0;

	void mousePressEvent(QGraphicsSceneMouseEvent *_event) override;

	void set_grid_pos(const QPoint& _pos) override;

	QPoint grid_pos() const override;

	void grab_child_item(IPlayItem* _item) override;

	void release_child_item(IPlayItem* _item) override;

	QPoint common_pos() const;
	void set_common_pos(const QPoint& _pos);

protected:
	QState* m_selected_state;		//bind in childs to selection object

private:
	QStateMachine* m_state_machine;
	QPoint m_grid_pos;
	std::unique_ptr<QPropertyAnimation> m_moving_animation;

	std::set<PlayItem*> m_child_items;

signals:
	void make_moving();
};
