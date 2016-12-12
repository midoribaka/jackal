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

	std::shared_ptr<Cell> pop_one()
	{
		if (deck.empty())
			return nullptr;

		std::uniform_int_distribution<> dis(0, --counter);

		size_t num = dis(gen);

		std::shared_ptr<Cell> ptr = deck[num];

		deck.removeAt(num);

		return ptr;
	}

protected:
	template<typename _T>
	void create_cell_set(size_t _num)
	{
		for (int i = 0; i < _num; ++i)
			deck.push_back(std::make_shared<_T>());
	}

	std::mt19937 gen;
	QList<std::shared_ptr<Cell> > deck;
	size_t counter;
};

class ClassicDeck : public Deck
{
public:
	ClassicDeck()
	{
		create_cell_set<EmptyCell>(40);
		create_cell_set<SideArrowCell>(3);
		create_cell_set<DiagonalArrowCell>(3);
		create_cell_set<DoubleSideArrowCell>(3);
		create_cell_set<DoubleDiagonalArrowCell>(3);
		create_cell_set<TripleArrowCell>(3);
		create_cell_set<QuadSideArrowCell>(3);
		create_cell_set<QuadDiagonalArrowCell>(3);

		create_cell_set<HorseCell>(2);
		create_cell_set<Spinning2Cell>(5);
		create_cell_set<Spinning3Cell>(4);
		create_cell_set<Spinning4Cell>(2);
		create_cell_set<Spinning5Cell>(1);

		create_cell_set<IceCell>(6);
		create_cell_set<TrapCell>(3);
		create_cell_set<CanonCell>(2);
		create_cell_set<FortressCell>(2);
		create_cell_set<GirlFortressCell>(1);
		create_cell_set<RumBarrelCell>(4);

		create_cell_set<CrocodileCell>(4);
		create_cell_set<OgreCell>(1);
		create_cell_set<BalloonCell>(2);
		create_cell_set<PlaneCell>(1);

		create_cell_set<Coins1Cell>(5);
		create_cell_set<Coins2Cell>(5);
		create_cell_set<Coins3Cell>(3);
		create_cell_set<Coins4Cell>(2);
		create_cell_set<Coins5Cell>(1);
	}
};

class TreasureIslandDeck : public Deck
{
public:
	TreasureIslandDeck()
	{
		create_cell_set<EmptyCell>(18);
		create_cell_set<SideArrowCell>(3);
		create_cell_set<DiagonalArrowCell>(3);
		create_cell_set<DoubleSideArrowCell>(3);
		create_cell_set<DoubleDiagonalArrowCell>(3);
		create_cell_set<TripleArrowCell>(3);
		create_cell_set<QuadSideArrowCell>(3);
		create_cell_set<QuadDiagonalArrowCell>(3);

		create_cell_set<HorseCell>(2);
		create_cell_set<Spinning2Cell>(5);
		create_cell_set<Spinning3Cell>(4);
		create_cell_set<Spinning4Cell>(2);
		create_cell_set<Spinning5Cell>(1);

		create_cell_set<IceCell>(6);
		create_cell_set<TrapCell>(3);
		create_cell_set<CanonCell>(2);
		create_cell_set<FortressCell>(2);
		create_cell_set<GirlFortressCell>(1);
		create_cell_set<RumBarrelCell>(4);

		create_cell_set<CrocodileCell>(4);
		create_cell_set<OgreCell>(1);
		create_cell_set<BalloonCell>(2);
		create_cell_set<PlaneCell>(1);

		create_cell_set<Coins1Cell>(5);
		create_cell_set<Coins2Cell>(5);
		create_cell_set<Coins3Cell>(3);
		create_cell_set<Coins4Cell>(2);
		create_cell_set<Coins5Cell>(1);

		create_cell_set<TreasureCell>(1);
		create_cell_set<CarambaCell>(1);
		create_cell_set<LightHouseCell>(1);

		create_cell_set<BenGunnCell>(1);
		create_cell_set<MissionaryCell>(1);
		create_cell_set<FridayCell>(1);
		create_cell_set<Rum1Cell>(3);
		create_cell_set<Rum2Cell>(2);
		create_cell_set<Rum3Cell>(1);
		create_cell_set<CaveCell>(4);
		create_cell_set<EarthquakeCell>(1);
		create_cell_set<JungleCell>(3);
		create_cell_set<GrassCell>(2);
	}
};