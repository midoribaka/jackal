#pragma once

#include "ActionCell.h"

//
class FlippableCell : public ActionCell
{
	Q_OBJECT

public:
	FlippableCell() : m_back_side_image("./Resources/cell_img/cell_back.png")
	{
		set_image(m_back_side_image);

		//Add rotation around OY property
		QGraphicsRotation* flip = new QGraphicsRotation();	//todo memory leak
		flip->setAxis(Qt::YAxis);

		QList<QGraphicsTransform *> animation_list;
		animation_list << flip;
		setTransformations(animation_list);		//add properties: angle

		//Rotation
		QPropertyAnimation *smooth_first_half_rotation = new QPropertyAnimation(flip, "angle");
		smooth_first_half_rotation->setStartValue(0);
		smooth_first_half_rotation->setEndValue(90);
		smooth_first_half_rotation->setDuration(250);
		smooth_first_half_rotation->setEasingCurve(QEasingCurve::InOutQuad);

		QPropertyAnimation *smooth_second_half_rotation = new QPropertyAnimation(flip, "angle");
		smooth_second_half_rotation->setStartValue(90);
		smooth_second_half_rotation->setEndValue(0);
		smooth_second_half_rotation->setDuration(250);
		smooth_second_half_rotation->setEasingCurve(QEasingCurve::InOutQuad);

		//Bring to front
		QPropertyAnimation *bring_to_front = new QPropertyAnimation(this, "zValue");
		bring_to_front->setKeyValueAt(0, qvariant_cast<qreal>(zValue()));
		bring_to_front->setKeyValueAt(0.5, qvariant_cast<qreal>(zValue() + 1));
		bring_to_front->setKeyValueAt(1, qvariant_cast<qreal>(zValue()));
		bring_to_front->setDuration(1000);
		bring_to_front->setEasingCurve(QEasingCurve::Linear);

		//Scale
		QPropertyAnimation *smooth_scale = new QPropertyAnimation(this, "scale");		//свойство добавляется при помощи QGraphicsScale
		smooth_scale->setKeyValueAt(0, qvariant_cast<qreal>(1.0));
		smooth_scale->setKeyValueAt(0.5, qvariant_cast<qreal>(1.3));
		smooth_scale->setKeyValueAt(1, qvariant_cast<qreal>(1.0));
		smooth_scale->setDuration(1000);
		smooth_scale->setEasingCurve(QEasingCurve::InOutQuad);

		//Animation workflow
		QSequentialAnimationGroup* smooth_rotation = new QSequentialAnimationGroup();
		smooth_rotation->addAnimation(smooth_first_half_rotation);
		smooth_rotation->addAnimation(smooth_second_half_rotation);

		m_flip_animation = std::make_unique<QParallelAnimationGroup>(this);
		m_flip_animation->addAnimation(smooth_rotation);
		m_flip_animation->addAnimation(smooth_scale);
		m_flip_animation->addAnimation(bring_to_front);

		//State machine
		QState* flip_group = new QState(m_initial_state);
		QState* back_on_top = new QState(flip_group);
		QState* front_on_top = new QState(flip_group);

		back_on_top->addTransition(this, &FlippableCell::flip, front_on_top);

		QObject::connect(back_on_top, &QState::exited, [this]
		{
			m_next_image = &m_front_side_image;
			m_flip_animation->start();
		});

		QObject::connect(front_on_top, &QState::exited, [this]
		{
			m_next_image = &m_back_side_image;
			m_flip_animation->start();
		});
	
		QObject::connect(smooth_first_half_rotation, &QPropertyAnimation::finished, [this]	//half way flip finished
		{
			set_image(*m_next_image);
		});

		QObject::connect(m_flip_animation.get(), &QPropertyAnimation::finished, this, &ICell::ready);

		flip_group->setInitialState(back_on_top);
		m_state_machine->start();
	}

protected:
	void prepare() override
	{
		emit flip();
	}

	void run_action() override
	{

	}

	QPixmap m_front_side_image;

private:
	QPixmap m_back_side_image;	//current image is RoundedRect::image
	std::unique_ptr<QParallelAnimationGroup> m_flip_animation;
	QPixmap* m_next_image;

signals:
	void flip();
};
