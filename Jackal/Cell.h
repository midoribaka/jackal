#pragma once

#include "RoundedRect.h"

#include <qmouseevent>
#include <QGraphicsRotation>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>

#include <QStateMachine>
#include <QMouseEventTransition>
#include <QSignalTransition>
#include <QGraphicsSceneMouseEvent>
#include <QColor>

#include <random>

#include <QTimer>

class Selection : public RoundedRect
{
public:
	Selection(const QRectF& _draw_rect = QRectF(), size_t _radius = 0, RoundedRect* _parent = 0, const QColor& _color = QColor(0, 150, 0, 100)) : RoundedRect(_draw_rect, _radius, _parent, _color)
	{
		setZValue(1.5);
	}

	virtual QRectF boundingRect() const override
	{
		//origin at center
		QRectF br = RoundedRect::boundingRect();
		return br.translated(-br.width() / 2, -br.height() / 2);
	}

	virtual void paint(QPainter *_painter, const QStyleOptionGraphicsItem *_option, QWidget *_widget = Q_NULLPTR) override
	{
		QPen pen;
		pen.setWidth(1);
		_painter->setPen(pen);
		_painter->drawRoundRect(boundingRect(), corner_radius, corner_radius);
	}
};

class Cell : public RoundedRect
{
public:
	Cell(const QRectF& _draw_rect = QRectF(), size_t _radius = 0, RoundedRect* _parent = 0) : RoundedRect(_draw_rect, _radius, _parent), m_actions(nullptr)
	{
		setZValue(1);
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

	QStateMachine* get_actions()
	{
		return m_actions;
	}

	void set_selected(bool _arg)
	{

	}

protected:
	QStateMachine* m_actions;
};

//
class StaticCell : public Cell
{
public:
	StaticCell(const QRectF& _draw_rect, size_t _radius = 0, RoundedRect* _parent = 0) : Cell(_draw_rect, _radius, _parent)
	{
		set_image(QPixmap("./Resources/cell_img/cell_sea.png"));
	}
};

//
class FlippableCell : public Cell
{
public:
	FlippableCell(const QRectF _draw_rect = QRectF(), size_t _radius = 0, RoundedRect* _parent = 0) : Cell(_draw_rect, _radius, _parent), back_side_image("./Resources/cell_img/cell_back.png")
	{
		set_image(back_side_image);

		setAcceptHoverEvents(true);

		//Animation
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

		// turn then scale
		QPropertyAnimation *smooth_scale_x = new QPropertyAnimation(scale, "xScale");		//свойство добавляется при помощи QGraphicsScale
		QPropertyAnimation *smooth_scale_y = new QPropertyAnimation(scale, "yScale");		//свойство добавляется при помощи QGraphicsScale
		smooth_scale_x->setKeyValueAt(0, qvariant_cast<qreal>(1.0));
		smooth_scale_x->setKeyValueAt(0.5, qvariant_cast<qreal>(1.3));
		smooth_scale_x->setKeyValueAt(1, qvariant_cast<qreal>(1.0));
		smooth_scale_x->setDuration(1000);
		smooth_scale_x->setEasingCurve(QEasingCurve::InOutQuad);

		smooth_scale_y->setKeyValueAt(0, qvariant_cast<qreal>(1.0));
		smooth_scale_y->setKeyValueAt(0.5, qvariant_cast<qreal>(1.3));
		smooth_scale_y->setKeyValueAt(1, qvariant_cast<qreal>(1.0));
		smooth_scale_y->setDuration(1000);
		smooth_scale_y->setEasingCurve(QEasingCurve::InOutQuad);

		flip_animation = new QParallelAnimationGroup(this);
		flip_animation->addAnimation(smooth_rotation);
		flip_animation->addAnimation(smooth_scale_x);
		flip_animation->addAnimation(smooth_scale_y);

		//State Machine
		/*	see_back_side -> (mousePressEvent) -> see_front_side
		see_front_side -> (reset signal by scene) -> see_back_side	*/

		state_machine = new QStateMachine(this);

		QState* see_back_side = new QState(state_machine);
		QState* see_front_side = new QState(state_machine);

		QObject::connect(see_back_side, &QState::exited, [this]()
		{
			setZValue(2);
			flip_animation->start();
		});

		QObject::connect(see_front_side, &QState::exited, [this]()
		{
			setZValue(2);
			flip_animation->start();
		});

		QEventTransition *mouse_press = new QEventTransition(this, QEvent::MouseButtonPress, see_back_side);
		mouse_press->setTargetState(see_front_side);

		QSignalTransition *reset_signal = see_front_side->addTransition(parent(), SIGNAL(reset_field()), see_back_side);


		//half way flip
		QObject::connect(smooth_first_half_rotation, &QPropertyAnimation::finished, [this, see_back_side, see_front_side]()
		{
			//Q_PROPERTY may will be better

			if (state_machine->configuration().contains(see_front_side))
				set_image(front_side_image);
			else if (state_machine->configuration().contains(see_back_side))
				set_image(back_side_image);
		});

		//finish
		QObject::connect(flip_animation, &QParallelAnimationGroup::finished, [this]() 
		{
			//при анимации ячейки происходит изменение z value (см mousePressEvent), иначе правая от неё ячейка будет её частично перекрывать
			//это возврат к значению по умолчанию по окончании анимации
			setZValue(1);	
		});

		state_machine->setInitialState(see_back_side);
		state_machine->start();
	}

protected:
	virtual void set_front_side_image(const QPixmap& _image)
	{
		front_side_image = _image;
	}

	void mousePressEvent(QGraphicsSceneMouseEvent *_event) override
	{
		auto wrapped = new QStateMachine::WrappedEvent(this, new QMouseEvent(QEvent::MouseButtonPress, _event->pos(), _event->button(), _event->buttons(), _event->modifiers()));
		state_machine->postEvent(wrapped);
		Cell::mousePressEvent(_event);
	}

	QPixmap front_side_image;
	QPixmap back_side_image;

	//current image is RoundedRect::image
private:
	QParallelAnimationGroup *flip_animation;
	QStateMachine* state_machine;
};

//

class EmptyCell : public FlippableCell
{
public:
	EmptyCell(const QRectF& _draw_rect = QRectF(), size_t _radius = 0, RoundedRect* _parent = 0) : FlippableCell(_draw_rect, _radius, _parent)
	{
		//выбираем один из четырёх
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(1, 4);		
		set_front_side_image(QPixmap("./Resources/cell_img/empty-"+ QString::number(dis(gen))+".png"));
	}

private:
//	std::map<>
};

//

class RandomDirectionCell : public FlippableCell
{
	enum Direction
	{
		NORD = 0,
		OST = 90,
		SOUTH = 180,
		WEST = 270
	};

	const int angle_choice[4] = { NORD, OST, SOUTH, WEST};

public:
	RandomDirectionCell(const QRectF& _draw_rect = QRectF(), size_t _radius = 0, RoundedRect* _parent = 0) : FlippableCell(_draw_rect, _radius, _parent)
	{
		//выбираем случайное направление
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(0, 3);
		angle = angle_choice[dis(gen)];
	}

protected:	
	virtual void set_front_side_image(const QPixmap& _image) override
	{
		front_side_image = _image.transformed(QTransform().rotate(angle));
	}

protected:
	int angle;	//угол поворота
};

//

class SideArrowCell : public RandomDirectionCell
{
public:
	SideArrowCell(const QRectF& _draw_rect = QRectF(), size_t _radius = 0, RoundedRect* _parent = 0) : RandomDirectionCell(_draw_rect, _radius, _parent)
	{
		set_front_side_image(QPixmap("./Resources/cell_img/arrow-1.png"));
	}
};

//

class DiagonalArrowCell : public RandomDirectionCell
{
public:
	DiagonalArrowCell(const QRectF& _draw_rect = QRectF(), size_t _radius = 0, RoundedRect* _parent = 0) : RandomDirectionCell(_draw_rect, _radius, _parent)
	{
		set_front_side_image(QPixmap("./Resources/cell_img/arrow-2.png"));
	}
};

//

class DoubleSideArrowCell : public RandomDirectionCell
{
public:
	DoubleSideArrowCell(const QRectF& _draw_rect = QRectF(), size_t _radius = 0, RoundedRect* _parent = 0) : RandomDirectionCell(_draw_rect, _radius, _parent)
	{
		set_front_side_image(QPixmap("./Resources/cell_img/arrow-3.png"));
	}
};

//

class DoubleDiagonalArrowCell : public RandomDirectionCell
{
public:
	DoubleDiagonalArrowCell(const QRectF& _draw_rect = QRectF(), size_t _radius = 0, RoundedRect* _parent = 0) : RandomDirectionCell(_draw_rect, _radius, _parent)
	{
		set_front_side_image(QPixmap("./Resources/cell_img/arrow-4.png"));
	}
};

//

class TripleArrowCell : public RandomDirectionCell
{
public:
	TripleArrowCell(const QRectF& _draw_rect = QRectF(), size_t _radius = 0, RoundedRect* _parent = 0) : RandomDirectionCell(_draw_rect, _radius, _parent)
	{
		set_front_side_image(QPixmap("./Resources/cell_img/arrow-5.png"));
	}
};

//

class QuadSideArrowCell : public RandomDirectionCell
{
public:
	QuadSideArrowCell(const QRectF& _draw_rect = QRectF(), size_t _radius = 0, RoundedRect* _parent = 0) : RandomDirectionCell(_draw_rect, _radius, _parent)
	{
		set_front_side_image(QPixmap("./Resources/cell_img/arrow-6.png"));
	}
};

//

class QuadDiagonalArrowCell : public RandomDirectionCell
{
public:
	QuadDiagonalArrowCell(const QRectF& _draw_rect = QRectF(), size_t _radius = 0, RoundedRect* _parent = 0) : RandomDirectionCell(_draw_rect, _radius, _parent)
	{
		set_front_side_image(QPixmap("./Resources/cell_img/arrow-7.png"));
	}
};

//

class HorseCell : public FlippableCell
{
public:
	HorseCell(const QRectF& _draw_rect = QRectF(), size_t _radius = 0, RoundedRect* _parent = 0) : FlippableCell(_draw_rect, _radius, _parent)
	{
		set_front_side_image(QPixmap("./Resources/cell_img/horse.png"));
	}
};

//

class Spinning2Cell : public FlippableCell
{
public:
	Spinning2Cell(const QRectF& _draw_rect = QRectF(), size_t _radius = 0, RoundedRect* _parent = 0) : FlippableCell(_draw_rect, _radius, _parent)
	{
		set_front_side_image(QPixmap("./Resources/cell_img/rotate-2.png"));
	}
};

//

class Spinning3Cell : public FlippableCell
{
public:
	Spinning3Cell(const QRectF& _draw_rect = QRectF(), size_t _radius = 0, RoundedRect* _parent = 0) : FlippableCell(_draw_rect, _radius, _parent)
	{
		set_front_side_image(QPixmap("./Resources/cell_img/rotate-3.png"));
	}
};

//

class Spinning4Cell : public FlippableCell
{
public:
	Spinning4Cell(const QRectF& _draw_rect = QRectF(), size_t _radius = 0, RoundedRect* _parent = 0) : FlippableCell(_draw_rect, _radius, _parent)
	{
		set_front_side_image(QPixmap("./Resources/cell_img/rotate-4.png"));
	}
};

//

class Spinning5Cell : public FlippableCell
{
public:
	Spinning5Cell(const QRectF& _draw_rect = QRectF(), size_t _radius = 0, RoundedRect* _parent = 0) : FlippableCell(_draw_rect, _radius, _parent)
	{
		set_front_side_image(QPixmap("./Resources/cell_img/rotate-5.png"));
	}
};

//

class IceCell : public FlippableCell
{
public:
	IceCell(const QRectF& _draw_rect = QRectF(), size_t _radius = 0, RoundedRect* _parent = 0) : FlippableCell(_draw_rect, _radius, _parent)
	{
		set_front_side_image(QPixmap("./Resources/cell_img/ice.png"));
	}
};

//

class TrapCell : public FlippableCell
{
public:
	TrapCell(const QRectF& _draw_rect = QRectF(), size_t _radius = 0, RoundedRect* _parent = 0) : FlippableCell(_draw_rect, _radius, _parent)
	{
		set_front_side_image(QPixmap("./Resources/cell_img/pitfall.png"));
	}
};

//

class CanonCell : public RandomDirectionCell
{
public:
	CanonCell(const QRectF& _draw_rect = QRectF(), size_t _radius = 0, RoundedRect* _parent = 0) : RandomDirectionCell(_draw_rect, _radius, _parent)
	{
		set_front_side_image(QPixmap("./Resources/cell_img/gun.png"));
	}
};

//

class FortressCell : public FlippableCell
{
public:
	FortressCell(const QRectF& _draw_rect = QRectF(), size_t _radius = 0, RoundedRect* _parent = 0) : FlippableCell(_draw_rect, _radius, _parent)
	{
		set_front_side_image(QPixmap("./Resources/cell_img/fort.png"));
	}
};

//

class GirlFortressCell : public FlippableCell
{
public:
	GirlFortressCell(const QRectF& _draw_rect = QRectF(), size_t _radius = 0, RoundedRect* _parent = 0) : FlippableCell(_draw_rect, _radius, _parent)
	{
		set_front_side_image(QPixmap("./Resources/cell_img/fort-w-aborigine.png"));
	}
};

//

class RumBarrelCell : public FlippableCell
{
public:
	RumBarrelCell(const QRectF& _draw_rect = QRectF(), size_t _radius = 0, RoundedRect* _parent = 0) : FlippableCell(_draw_rect, _radius, _parent)
	{
		set_front_side_image(QPixmap("./Resources/cell_img/keg-of-rum.png"));
	}
};

//

class CrocodileCell : public FlippableCell
{
public:
	CrocodileCell(const QRectF& _draw_rect = QRectF(), size_t _radius = 0, RoundedRect* _parent = 0) : FlippableCell(_draw_rect, _radius, _parent)
	{
		set_front_side_image(QPixmap("./Resources/cell_img/crocodile.png"));
	}
};

//

class OgreCell : public FlippableCell
{
public:
	OgreCell(const QRectF& _draw_rect = QRectF(), size_t _radius = 0, RoundedRect* _parent = 0) : FlippableCell(_draw_rect, _radius, _parent)
	{
		set_front_side_image(QPixmap("./Resources/cell_img/cannibal.png"));
	}
};

//

class BalloonCell : public FlippableCell
{
public:
	BalloonCell(const QRectF& _draw_rect = QRectF(), size_t _radius = 0, RoundedRect* _parent = 0) : FlippableCell(_draw_rect, _radius, _parent)
	{
		set_front_side_image(QPixmap("./Resources/cell_img/balloon.png"));
	}
};

//

class PlaneCell : public FlippableCell
{
public:
	PlaneCell(const QRectF& _draw_rect = QRectF(), size_t _radius = 0, RoundedRect* _parent = 0) : FlippableCell(_draw_rect, _radius, _parent)
	{
		set_front_side_image(QPixmap("./Resources/cell_img/airplane.png"));
	}
};

//

class Coins1Cell : public FlippableCell
{
public:
	Coins1Cell(const QRectF& _draw_rect = QRectF(), size_t _radius = 0, RoundedRect* _parent = 0) : FlippableCell(_draw_rect, _radius, _parent)
	{
		set_front_side_image(QPixmap("./Resources/cell_img/coins-1.png"));
	}
};

//

class Coins2Cell : public FlippableCell
{
public:
	Coins2Cell(const QRectF& _draw_rect = QRectF(), size_t _radius = 0, RoundedRect* _parent = 0) : FlippableCell(_draw_rect, _radius, _parent)
	{
		set_front_side_image(QPixmap("./Resources/cell_img/coins-2.png"));
	}
};

//

class Coins3Cell : public FlippableCell
{
public:
	Coins3Cell(const QRectF& _draw_rect = QRectF(), size_t _radius = 0, RoundedRect* _parent = 0) : FlippableCell(_draw_rect, _radius, _parent)
	{
		set_front_side_image(QPixmap("./Resources/cell_img/coins-3.png"));
	}
};

//

class Coins4Cell : public FlippableCell
{
public:
	Coins4Cell(const QRectF& _draw_rect = QRectF(), size_t _radius = 0, RoundedRect* _parent = 0) : FlippableCell(_draw_rect, _radius, _parent)
	{
		set_front_side_image(QPixmap("./Resources/cell_img/coins-4.png"));
	}
};

//

class Coins5Cell : public FlippableCell
{
public:
	Coins5Cell(const QRectF& _draw_rect = QRectF(), size_t _radius = 0, RoundedRect* _parent = 0) : FlippableCell(_draw_rect, _radius, _parent)
	{
		set_front_side_image(QPixmap("./Resources/cell_img/coins-5.png"));
	}
};

//

class TreasureCell : public FlippableCell
{
public:
	TreasureCell(const QRectF& _draw_rect = QRectF(), size_t _radius = 0, RoundedRect* _parent = 0) : FlippableCell(_draw_rect, _radius, _parent)
	{
		set_front_side_image(QPixmap("./Resources/cell_img/treasure.png"));
	}
};

//

class CarambaCell : public FlippableCell
{
public:
	CarambaCell(const QRectF& _draw_rect = QRectF(), size_t _radius = 0, RoundedRect* _parent = 0) : FlippableCell(_draw_rect, _radius, _parent)
	{
		set_front_side_image(QPixmap("./Resources/cell_img/caramba.png"));
	}
};

//

class LightHouseCell : public FlippableCell
{
public:
	LightHouseCell(const QRectF& _draw_rect = QRectF(), size_t _radius = 0, RoundedRect* _parent = 0) : FlippableCell(_draw_rect, _radius, _parent)
	{
		set_front_side_image(QPixmap("./Resources/cell_img/lighthouse.png"));
	}
};

//

class BenGunnCell : public FlippableCell
{
public:
	BenGunnCell(const QRectF& _draw_rect = QRectF(), size_t _radius = 0, RoundedRect* _parent = 0) : FlippableCell(_draw_rect, _radius, _parent)
	{
		set_front_side_image(QPixmap("./Resources/cell_img/ben-gunn.png"));
	}
};

//

class MissionaryCell : public FlippableCell
{
public:
	MissionaryCell(const QRectF& _draw_rect = QRectF(), size_t _radius = 0, RoundedRect* _parent = 0) : FlippableCell(_draw_rect, _radius, _parent)
	{
		set_front_side_image(QPixmap("./Resources/cell_img/missionary.png"));
	}
};

//

class FridayCell : public FlippableCell
{
public:
	FridayCell(const QRectF& _draw_rect = QRectF(), size_t _radius = 0, RoundedRect* _parent = 0) : FlippableCell(_draw_rect, _radius, _parent)
	{
		set_front_side_image(QPixmap("./Resources/cell_img/friday.png"));
	}
};

//

class Rum1Cell : public FlippableCell
{
public:
	Rum1Cell(const QRectF& _draw_rect = QRectF(), size_t _radius = 0, RoundedRect* _parent = 0) : FlippableCell(_draw_rect, _radius, _parent)
	{
		set_front_side_image(QPixmap("./Resources/cell_img/bottles-of-rum-1.png"));
	}
};

//

class Rum2Cell : public FlippableCell
{
public:
	Rum2Cell(const QRectF& _draw_rect = QRectF(), size_t _radius = 0, RoundedRect* _parent = 0) : FlippableCell(_draw_rect, _radius, _parent)
	{
		set_front_side_image(QPixmap("./Resources/cell_img/bottles-of-rum-2.png"));
	}
};

//

class Rum3Cell : public FlippableCell
{
public:
	Rum3Cell(const QRectF& _draw_rect = QRectF(), size_t _radius = 0, RoundedRect* _parent = 0) : FlippableCell(_draw_rect, _radius, _parent)
	{
		set_front_side_image(QPixmap("./Resources/cell_img/bottles-of-rum-3.png"));
	}
};

//

class CaveCell : public FlippableCell
{
public:
	CaveCell(const QRectF& _draw_rect = QRectF(), size_t _radius = 0, RoundedRect* _parent = 0) : FlippableCell(_draw_rect, _radius, _parent)
	{
		set_front_side_image(QPixmap("./Resources/cell_img/cave.png"));
	}
};

//

class EarthquakeCell : public FlippableCell
{
public:
	EarthquakeCell(const QRectF& _draw_rect = QRectF(), size_t _radius = 0, RoundedRect* _parent = 0) : FlippableCell(_draw_rect, _radius, _parent)
	{
		set_front_side_image(QPixmap("./Resources/cell_img/earthquake.png"));
	}
};

//

class JungleCell : public FlippableCell
{
public:
	JungleCell(const QRectF& _draw_rect = QRectF(), size_t _radius = 0, RoundedRect* _parent = 0) : FlippableCell(_draw_rect, _radius, _parent)
	{
		set_front_side_image(QPixmap("./Resources/cell_img/jungle.png"));
	}
};

//

class GrassCell : public FlippableCell
{
public:
	GrassCell(const QRectF& _draw_rect = QRectF(), size_t _radius = 0, RoundedRect* _parent = 0) : FlippableCell(_draw_rect, _radius, _parent)
	{
		set_front_side_image(QPixmap("./Resources/cell_img/grass.png"));
	}
};