#pragma once

#include "RoundedRect.h"

#include <QGraphicsRotation>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>

class Cell : public RoundedRect
{
public:
	Cell(QRectF _draw_rect, size_t _radius = 0, RoundedRect* _parent = 0) : RoundedRect(_draw_rect, _radius, _parent), draw_rect(_draw_rect), corner_radius(_radius)
	{
	}

	virtual ~Cell()
	{
	}

	virtual QRectF boundingRect() const override
	{
		//origin at center
		QRectF br = RoundedRect::boundingRect();
		return br.translated(-br.width() / 2, -br.height() / 2);
	}
protected:
	QRectF draw_rect;
	size_t corner_radius;
};

//
class StaticCell : public Cell
{
public:
	StaticCell(QRectF _draw_rect, size_t _radius = 0, RoundedRect* _parent = 0) : Cell(_draw_rect, _radius, _parent)
	{
		set_image(QPixmap(":/Resources/cell_img/cell_sea.png"));
	}
};

//
class FlippableCell : public Cell, public QObject
{
public:
	FlippableCell(QRectF _draw_rect, size_t _radius = 0, RoundedRect* _parent = 0) : Cell(_draw_rect, _radius, _parent), front_side_image(":/Resources/cell_img/cell_front.png"), back_side_image(":/Resources/cell_img/cell_back.png")
	{
		set_image(back_side_image);

		QGraphicsRotation *flip = new QGraphicsRotation();
		flip->setAxis(Qt::YAxis);

		QGraphicsScale* scale = new QGraphicsScale();

		QList<QGraphicsTransform *> animation_list;
		animation_list << flip << scale;

		setTransformations(animation_list);		//add properties: angle and scale

		QPropertyAnimation *smooth_first_half_rotation = new QPropertyAnimation(flip, "angle");	//свойство добавляется при помощи QGraphicsRotation
		smooth_first_half_rotation->setStartValue(0);
		smooth_first_half_rotation->setEndValue(90);
		smooth_first_half_rotation->setDuration(250);
		smooth_first_half_rotation->setEasingCurve(QEasingCurve::InOutQuad);

		QPropertyAnimation *smooth_second_half_rotation = new QPropertyAnimation(flip, "angle");	//свойство добавляется при помощи QGraphicsRotation
		smooth_second_half_rotation->setStartValue(90);
		smooth_second_half_rotation->setEndValue(0);
		smooth_second_half_rotation->setDuration(250);
		smooth_second_half_rotation->setEasingCurve(QEasingCurve::InOutQuad);

		QSequentialAnimationGroup* smooth_rotation = new QSequentialAnimationGroup();
		smooth_rotation->addAnimation(smooth_first_half_rotation);
		smooth_rotation->addAnimation(smooth_second_half_rotation);

		QPropertyAnimation *smooth_scale_x = new QPropertyAnimation(scale, "xScale");		//свойство добавляется при помощи QGraphicsScale
		QPropertyAnimation *smooth_scale_y = new QPropertyAnimation(scale, "yScale");		//свойство добавляется при помощи QGraphicsScale
		smooth_scale_x->setKeyValueAt(0, qvariant_cast<qreal>(1.0));
		smooth_scale_x->setKeyValueAt(0.5, qvariant_cast<qreal>(1.5));
		smooth_scale_x->setKeyValueAt(1, qvariant_cast<qreal>(1.0));
		smooth_scale_x->setDuration(500);
		smooth_scale_x->setEasingCurve(QEasingCurve::InOutQuad);

		smooth_scale_y->setKeyValueAt(0, qvariant_cast<qreal>(1.0));
		smooth_scale_y->setKeyValueAt(0.5, qvariant_cast<qreal>(1.3));
		smooth_scale_y->setKeyValueAt(1, qvariant_cast<qreal>(1.0));
		smooth_scale_y->setDuration(500);
		smooth_scale_y->setEasingCurve(QEasingCurve::InOutQuad);

		flip_animation = new QParallelAnimationGroup(this);
		flip_animation->addAnimation(smooth_rotation);
		flip_animation->addAnimation(smooth_scale_x);
		flip_animation->addAnimation(smooth_scale_y);

		//half way flip
		QObject::connect(smooth_first_half_rotation, &QPropertyAnimation::finished, [this]()
		{
			set_image(front_side_image);
		});

		//finish
		QObject::connect(flip_animation, &QParallelAnimationGroup::finished, [this]() 
		{
			//при анимации ячейки происходит изменение z value (см mousePressEvent), иначе правая от неё ячейка будет её частично перекрывать
			//это возврат к значению по умолчанию по окончании анимации
			setZValue(0);	
		});
	}

	void set_back_side_image(const QPixmap& _image)
	{
		back_side_image = _image;
	}

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *_event) override
	{
		setZValue(1);
		flip_animation->start();
		Cell::mousePressEvent(_event);
	}

private:
	QParallelAnimationGroup *flip_animation;

	QPixmap front_side_image;
	QPixmap back_side_image;

	//current image is RoundedRect::image
};