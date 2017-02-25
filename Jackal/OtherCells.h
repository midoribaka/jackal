#pragma once

#include "ActionCell.h"
#include "FlippableCell.h"

#include "Mask.h"

#include <random>

class SeaCell : public ActionCell
{
public:
	SeaCell()
	{
		CenteredRoundedRect::set_image(QPixmap("./Resources/cell_img/sea_tx3.jpg"));

		m_mask = mask::cross();

		m_state_machine->start();	//todo baaad
	}

protected:
	void prepare() override
	{
		emit ICell::ready();
	}

	void run_action() override
	{
		int t = 0;
	}
};

class EmptyCell : public FlippableCell
{
public:
	EmptyCell()
	{
		//выбираем один из четырёх
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(1, 4);
		m_front_side_image = QPixmap("./Resources/cell_img/empty-" + QString::number(dis(gen)) + ".png");

		m_mask = mask::square();
	}
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

	const int angle_choice[4] = { NORD, OST, SOUTH, WEST };

public:
	RandomDirectionCell()
	{
		//выбираем случайное направление
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(0, 3);
		angle = angle_choice[dis(gen)];
	}

protected:
	virtual void set_image(const QPixmap& _image) override
	{
		CenteredRoundedRect::set_image(_image.transformed(QTransform().rotate(angle)));
	}

protected:
	int angle;	//угол поворота
};

//

class SideArrowCell : public RandomDirectionCell
{
public:
	SideArrowCell()
	{
		m_front_side_image = QPixmap("./Resources/cell_img/arrow-1.png");
	}
};

//

class DiagonalArrowCell : public RandomDirectionCell
{
public:
	DiagonalArrowCell()
	{
		m_front_side_image = QPixmap("./Resources/cell_img/arrow-2.png");
	}
};

//

class DoubleSideArrowCell : public RandomDirectionCell
{
public:
	DoubleSideArrowCell()
	{
		m_front_side_image = QPixmap("./Resources/cell_img/arrow-3.png");
	}
};

//

class DoubleDiagonalArrowCell : public RandomDirectionCell
{
public:
	DoubleDiagonalArrowCell()
	{
		m_front_side_image = QPixmap("./Resources/cell_img/arrow-4.png");
	}
};

//

class TripleArrowCell : public RandomDirectionCell
{
public:
	TripleArrowCell()
	{
		m_front_side_image = QPixmap("./Resources/cell_img/arrow-5.png");
	}
};

//

class QuadSideArrowCell : public RandomDirectionCell
{
public:
	QuadSideArrowCell()
	{
		m_front_side_image = QPixmap("./Resources/cell_img/arrow-6.png");

		m_mask = mask::cross();
	}
};

//

class QuadDiagonalArrowCell : public RandomDirectionCell
{
public:
	QuadDiagonalArrowCell()
	{
		m_front_side_image = QPixmap("./Resources/cell_img/arrow-7.png");

		m_mask = mask::quad_diagonal();
	}
};

//

class HorseCell : public FlippableCell
{
public:
	HorseCell()
	{
		m_front_side_image = QPixmap("./Resources/cell_img/horse.png");

		m_mask = mask::horse();
	}
};

//

class Spinning2Cell : public FlippableCell
{
public:
	Spinning2Cell()
	{
		m_front_side_image = QPixmap("./Resources/cell_img/rotate-2.png");
	}
};

//

class Spinning3Cell : public FlippableCell
{
public:
	Spinning3Cell()
	{
		m_front_side_image = QPixmap("./Resources/cell_img/rotate-3.png");
	}
};

//

class Spinning4Cell : public FlippableCell
{
public:
	Spinning4Cell()
	{
		m_front_side_image = QPixmap("./Resources/cell_img/rotate-4.png");
	}
};

//

class Spinning5Cell : public FlippableCell
{
public:
	Spinning5Cell()
	{
		m_front_side_image = QPixmap("./Resources/cell_img/rotate-5.png");
	}
};

//

class IceCell : public FlippableCell
{
public:
	IceCell()
	{
		m_front_side_image = QPixmap("./Resources/cell_img/ice.png");
	}
};

//

class TrapCell : public FlippableCell
{
public:
	TrapCell()
	{
		m_front_side_image = QPixmap("./Resources/cell_img/pitfall.png");
	}
};

//

class CanonCell : public RandomDirectionCell
{
public:
	CanonCell()
	{
		m_front_side_image = QPixmap("./Resources/cell_img/gun.png");
	}
};

//

class FortressCell : public FlippableCell
{
public:
	FortressCell()
	{
		m_front_side_image = QPixmap("./Resources/cell_img/fort.png");

		m_mask = mask::square();
	}
};

//

class GirlFortressCell : public FlippableCell
{
public:
	GirlFortressCell()
	{
		m_front_side_image = QPixmap("./Resources/cell_img/fort-w-aborigine.png");

		m_mask = mask::square();
	}
};

//

class RumBarrelCell : public FlippableCell
{
public:
	RumBarrelCell()
	{
		m_front_side_image = QPixmap("./Resources/cell_img/keg-of-rum.png");

		m_mask = mask::square();
	}
};

//

class CrocodileCell : public FlippableCell
{
public:
	CrocodileCell()
	{
		m_front_side_image = QPixmap("./Resources/cell_img/crocodile.png");
	}
};

//

class OgreCell : public FlippableCell
{
public:
	OgreCell()
	{
		m_front_side_image = QPixmap("./Resources/cell_img/cannibal.png");
	}
};

//

class BalloonCell : public FlippableCell
{
public:
	BalloonCell()
	{
		m_front_side_image = QPixmap("./Resources/cell_img/balloon.png");
	}
};

//

class PlaneCell : public FlippableCell
{
public:
	PlaneCell()
	{
		m_front_side_image = QPixmap("./Resources/cell_img/airplane.png");
	}
};

//

class Coins1Cell : public FlippableCell
{
public:
	Coins1Cell()
	{
		m_front_side_image = QPixmap("./Resources/cell_img/coins-1.png");

		m_mask = mask::square();
	}
};

//

class Coins2Cell : public FlippableCell
{
public:
	Coins2Cell()
	{
		m_front_side_image = QPixmap("./Resources/cell_img/coins-2.png");

		m_mask = mask::square();
	}
};

//

class Coins3Cell : public FlippableCell
{
public:
	Coins3Cell()
	{
		m_front_side_image = QPixmap("./Resources/cell_img/coins-3.png");

		m_mask = mask::square();
	}
};

//

class Coins4Cell : public FlippableCell
{
public:
	Coins4Cell()
	{
		m_front_side_image = QPixmap("./Resources/cell_img/coins-4.png");

		m_mask = mask::square();
	}
};

//

class Coins5Cell : public FlippableCell
{
public:
	Coins5Cell()
	{
		m_front_side_image = QPixmap("./Resources/cell_img/coins-5.png");

		m_mask = mask::square();
	}
};

//

class TreasureCell : public FlippableCell
{
public:
	TreasureCell()
	{
		m_front_side_image = QPixmap("./Resources/cell_img/treasure.png");

		m_mask = mask::square();
	}
};

//

class CarambaCell : public FlippableCell
{
public:
	CarambaCell()
	{
		m_front_side_image = QPixmap("./Resources/cell_img/caramba.png");

		m_mask = mask::square();
	}
};

//

class LightHouseCell : public FlippableCell
{
public:
	LightHouseCell()
	{
		m_front_side_image = QPixmap("./Resources/cell_img/lighthouse.png");
	}
};

//

class BenGunnCell : public FlippableCell
{
public:
	BenGunnCell()
	{
		m_front_side_image = QPixmap("./Resources/cell_img/ben-gunn.png");

		m_mask = mask::square();
	}
};

//

class MissionaryCell : public FlippableCell
{
public:
	MissionaryCell()
	{
		m_front_side_image = QPixmap("./Resources/cell_img/missionary.png");

		m_mask = mask::square();
	}
};

//

class FridayCell : public FlippableCell
{
public:
	FridayCell()
	{
		m_front_side_image = QPixmap("./Resources/cell_img/friday.png");

		m_mask = mask::square();
	}
};

//

class Rum1Cell : public FlippableCell
{
public:
	Rum1Cell()
	{
		m_front_side_image = QPixmap("./Resources/cell_img/bottles-of-rum-1.png");

		m_mask = mask::square();
	}
};

//

class Rum2Cell : public FlippableCell
{
public:
	Rum2Cell()
	{
		m_front_side_image = QPixmap("./Resources/cell_img/bottles-of-rum-2.png");

		m_mask = mask::square();
	}
};

//

class Rum3Cell : public FlippableCell
{
public:
	Rum3Cell()
	{
		m_front_side_image = QPixmap("./Resources/cell_img/bottles-of-rum-3.png");

		m_mask = mask::square();
	}
};

//

class CaveCell : public FlippableCell
{
public:
	CaveCell()
	{
		m_front_side_image = QPixmap("./Resources/cell_img/cave.png");
	}
};

//

class EarthquakeCell : public FlippableCell
{
public:
	EarthquakeCell()
	{
		m_front_side_image = QPixmap("./Resources/cell_img/earthquake.png");

		m_mask = mask::square();
	}
};

//

class JungleCell : public FlippableCell
{
public:
	JungleCell()
	{
		m_front_side_image = QPixmap("./Resources/cell_img/jungle.png");

		m_mask = mask::square();
	}
};

//

class GrassCell : public FlippableCell
{
public:
	GrassCell()
	{
		m_front_side_image = QPixmap("./Resources/cell_img/grass.png");

		m_mask = mask::square();
	}
};