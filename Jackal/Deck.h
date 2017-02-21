#pragma once

#include "OtherCells.h"

#include <QList>

#include <random>

class Deck
{
public:
	const size_t deck_size = 117;

	Deck() :counter(deck_size)
	{
		std::random_device rd;
		gen.seed(rd());
	}

	ICell* pop_one()
	{
		if (deck.empty())
			return nullptr;

		std::uniform_int_distribution<> dis(0, --counter);

		size_t num = dis(gen);

		ICell* ptr = deck[num];

		deck.removeAt(num);

		return ptr;
	}

protected:
	template<typename _T>
	void add_cells(size_t _num)
	{
		for (int i = 0; i < _num; ++i)
			deck.push_back(new _T());
	}

	std::mt19937 gen;
	QList<ICell*> deck;
	size_t counter;
};

class ClassicDeck : public Deck
{
public:
	ClassicDeck()
	{
		add_cells<EmptyCell>(40);
		add_cells<SideArrowCell>(3);
		add_cells<DiagonalArrowCell>(3);
		add_cells<DoubleSideArrowCell>(3);
		add_cells<DoubleDiagonalArrowCell>(3);
		add_cells<TripleArrowCell>(3);
		add_cells<QuadSideArrowCell>(3);
		add_cells<QuadDiagonalArrowCell>(3);

		add_cells<HorseCell>(2);
		add_cells<Spinning2Cell>(5);
		add_cells<Spinning3Cell>(4);
		add_cells<Spinning4Cell>(2);
		add_cells<Spinning5Cell>(1);

		add_cells<IceCell>(6);
		add_cells<TrapCell>(3);
		add_cells<CanonCell>(2);
		add_cells<FortressCell>(2);
		add_cells<GirlFortressCell>(1);
		add_cells<RumBarrelCell>(4);

		add_cells<CrocodileCell>(4);
		add_cells<OgreCell>(1);
		add_cells<BalloonCell>(2);
		add_cells<PlaneCell>(1);

		add_cells<Coins1Cell>(5);
		add_cells<Coins2Cell>(5);
		add_cells<Coins3Cell>(3);
		add_cells<Coins4Cell>(2);
		add_cells<Coins5Cell>(1);
	}
};

class TreasureIslandDeck : public Deck
{
public:
	TreasureIslandDeck()
	{
		add_cells<EmptyCell>(18);
		add_cells<SideArrowCell>(3);
		add_cells<DiagonalArrowCell>(3);
		add_cells<DoubleSideArrowCell>(3);
		add_cells<DoubleDiagonalArrowCell>(3);
		add_cells<TripleArrowCell>(3);
		add_cells<QuadSideArrowCell>(3);
		add_cells<QuadDiagonalArrowCell>(3);

		add_cells<HorseCell>(2);
		add_cells<Spinning2Cell>(5);
		add_cells<Spinning3Cell>(4);
		add_cells<Spinning4Cell>(2);
		add_cells<Spinning5Cell>(1);

		add_cells<IceCell>(6);
		add_cells<TrapCell>(3);
		add_cells<CanonCell>(2);
		add_cells<FortressCell>(2);
		add_cells<GirlFortressCell>(1);
		add_cells<RumBarrelCell>(4);

		add_cells<CrocodileCell>(4);
		add_cells<OgreCell>(1);
		add_cells<BalloonCell>(2);
		add_cells<PlaneCell>(1);

		add_cells<Coins1Cell>(5);
		add_cells<Coins2Cell>(5);
		add_cells<Coins3Cell>(3);
		add_cells<Coins4Cell>(2);
		add_cells<Coins5Cell>(1);

		add_cells<TreasureCell>(1);
		add_cells<CarambaCell>(1);
		add_cells<LightHouseCell>(1);

		add_cells<BenGunnCell>(1);
		add_cells<MissionaryCell>(1);
		add_cells<FridayCell>(1);
		add_cells<Rum1Cell>(3);
		add_cells<Rum2Cell>(2);
		add_cells<Rum3Cell>(1);
		add_cells<CaveCell>(4);
		add_cells<EarthquakeCell>(1);
		add_cells<JungleCell>(3);
		add_cells<GrassCell>(2);
	}
};