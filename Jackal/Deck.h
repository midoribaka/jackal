#pragma once

#include "Cell.h"

#include <QList>

#include <random>

class ClassicDeck
{
	const size_t empty_cell_num = 40;
	const size_t side_arrow_cell_num = 3;
	const size_t diagonal_arrow_cell_num = 3;
	const size_t double_side_arrow_cell_num = 3;
	const size_t double_diagonal_arrow_cell_num = 3;
	const size_t triple_arrow_cell_num = 3;
	const size_t quad_side_arrow_cell_num = 3;
	const size_t quad_diagonal_arrow_cell_num = 3;

	const size_t horse_cell_num = 2;

	const size_t spinning2_cell_num = 5;	//ловушки 
	const size_t spinning3_cell_num = 4;
	const size_t spinning4_cell_num = 2;
	const size_t spinning5_cell_num = 1;

	const size_t ice_cell_num = 6;
	const size_t trap_cell_num = 3;	//яма
	const size_t canon_cell_num = 2;
	const size_t fortress_cell_num = 2;	//крепость
	const size_t girl_fortress_cell_num = 1;	//крепость с тёлкой =)
	const size_t rum_barrel_cell_num = 4;	//бочонки рома

	const size_t crocodile_cell_num = 4;
	const size_t ogre_cell_num = 1;
	const size_t baloon_cell_num = 2;
	const size_t plane_cell_num = 1;

	const size_t treasure1_cell_num = 5;
	const size_t treasure2_cell_num = 5;
	const size_t treasure3_cell_num = 3;
	const size_t treasure4_cell_num = 2;
	const size_t treasure5_cell_num = 1;

public:
	const size_t deck_size = empty_cell_num + side_arrow_cell_num;

	ClassicDeck() : counter(deck_size)
	{
		for (int i = 0; i < empty_cell_num; ++i)
			deck.push_back(new EmptyCell());

		for (int i = 0; i < side_arrow_cell_num; ++i)
			deck.push_back(new SideArrowCell());
	}

	Cell* pop_one()
	{
		if (deck.empty())
			return nullptr;

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(0, counter-1);

		size_t num = dis(gen);
		Cell* ptr = deck[num];

		deck.removeAt(num);
		counter--;

		return ptr;
	}

private:
	QList<Cell*> deck;
	size_t counter;
};

class TreasureIslandDeck
{
	const size_t empty_cell_num = 18;
	const size_t side_arrow_cell_num = 3;
	const size_t diagonal_arrow_cell_num = 3;
	const size_t double_side_arrow_cell_num = 3;
	const size_t double_diagonal_arrow_cell_num = 3;
	const size_t triple_arrow_cell_num = 3;
	const size_t quad_side_arrow_cell_num = 3;
	const size_t quad_diagonal_arrow_cell_num = 3;

	const size_t horse_cell_num = 2;

	const size_t spinning2_cell_num = 5;	//ловушки 
	const size_t spinning3_cell_num = 4;
	const size_t spinning4_cell_num = 2;
	const size_t spinning5_cell_num = 1;

	const size_t ice_cell_num = 6;
	const size_t trap_cell_num = 3;	//яма
	const size_t canon_cell_num = 2;
	const size_t fortress_cell_num = 2;	//крепость
	const size_t girl_fortress_cell_num = 1;	//крепость с тёлкой =)
	const size_t rum_barrel_cell_num = 4;	//бочонки рома

	const size_t crocodile_cell_num = 4;
	const size_t ogre_cell_num = 1;
	const size_t balloon_cell_num = 2;
	const size_t plane_cell_num = 1;

	const size_t coins1_cell_num = 5;
	const size_t coins2_cell_num = 5;
	const size_t coins3_cell_num = 3;
	const size_t coins4_cell_num = 2;
	const size_t coins5_cell_num = 1;

	const size_t treasure_cell_num = 1;
	const size_t caramba_cell_num = 1;
	const size_t lighthouse_cell_num = 1;

	const size_t ben_gunn_cell_num = 1;
	const size_t missionary_cell_num = 1;
	const size_t friday_cell_num = 1;

	const size_t rum1_cell_num = 3;
	const size_t rum2_cell_num = 2;
	const size_t rum3_cell_num = 1;

	const size_t cave_cell_num = 4;

	const size_t earthquake_cell_num = 1;
	const size_t jungle_cell_num = 3;
	const size_t grass_cell_num = 2;

public:
	const size_t deck_size = 117;	//мне лениво их все складывать

	TreasureIslandDeck() : counter(deck_size)
	{
		for (int i = 0; i < empty_cell_num; ++i)
			deck.push_back(new EmptyCell());

		for (int i = 0; i < side_arrow_cell_num; ++i)
			deck.push_back(new SideArrowCell());

		for (int i = 0; i < diagonal_arrow_cell_num; ++i)
			deck.push_back(new DiagonalArrowCell());

		for (int i = 0; i < double_side_arrow_cell_num; ++i)
			deck.push_back(new DoubleSideArrowCell());

		for (int i = 0; i < double_diagonal_arrow_cell_num; ++i)
			deck.push_back(new DoubleDiagonalArrowCell());

		for (int i = 0; i < triple_arrow_cell_num; ++i)
			deck.push_back(new TripleArrowCell());

		for (int i = 0; i < quad_side_arrow_cell_num; ++i)
			deck.push_back(new QuadSideArrowCell());

		for (int i = 0; i < quad_diagonal_arrow_cell_num; ++i)
			deck.push_back(new QuadDiagonalArrowCell());

		for (int i = 0; i < horse_cell_num; ++i)
			deck.push_back(new HorseCell());

		for (int i = 0; i < spinning2_cell_num; ++i)
			deck.push_back(new Spinning2Cell());

		for (int i = 0; i < spinning3_cell_num; ++i)
			deck.push_back(new Spinning3Cell());

		for (int i = 0; i < spinning4_cell_num; ++i)
			deck.push_back(new Spinning4Cell());

		for (int i = 0; i < spinning5_cell_num; ++i)
			deck.push_back(new Spinning5Cell());

		for (int i = 0; i < ice_cell_num; ++i)
			deck.push_back(new IceCell());

		for (int i = 0; i < trap_cell_num; ++i)
			deck.push_back(new TrapCell());

		for (int i = 0; i < canon_cell_num; ++i)
			deck.push_back(new CanonCell());

		for (int i = 0; i < fortress_cell_num; ++i)
			deck.push_back(new FortressCell());

		for (int i = 0; i < girl_fortress_cell_num; ++i)
			deck.push_back(new GirlFortressCell());

		for (int i = 0; i < rum_barrel_cell_num; ++i)
			deck.push_back(new RumBarrelCell());

		for (int i = 0; i <crocodile_cell_num; ++i)
			deck.push_back(new CrocodileCell());

		for (int i = 0; i < ogre_cell_num; ++i)
			deck.push_back(new OgreCell());

		for (int i = 0; i < balloon_cell_num; ++i)
			deck.push_back(new BalloonCell());

		for (int i = 0; i < plane_cell_num; ++i)
			deck.push_back(new PlaneCell());

		for (int i = 0; i < coins1_cell_num; ++i)
			deck.push_back(new Coins1Cell());

		for (int i = 0; i < coins2_cell_num; ++i)
			deck.push_back(new Coins2Cell());

		for (int i = 0; i < coins3_cell_num; ++i)
			deck.push_back(new Coins3Cell());

		for (int i = 0; i < coins4_cell_num; ++i)
			deck.push_back(new Coins4Cell());

		for (int i = 0; i < coins5_cell_num; ++i)
			deck.push_back(new Coins5Cell());

		for (int i = 0; i < treasure_cell_num; ++i)
			deck.push_back(new TreasureCell());

		for (int i = 0; i < caramba_cell_num; ++i)
			deck.push_back(new CarambaCell());

		for (int i = 0; i < lighthouse_cell_num; ++i)
			deck.push_back(new LightHouseCell());

		for (int i = 0; i < ben_gunn_cell_num; ++i)
			deck.push_back(new BenGunnCell());

		for (int i = 0; i < missionary_cell_num; ++i)
			deck.push_back(new MissionaryCell());

		for (int i = 0; i < friday_cell_num; ++i)
			deck.push_back(new FridayCell());

		for (int i = 0; i < rum1_cell_num; ++i)
			deck.push_back(new Rum1Cell());

		for (int i = 0; i < rum2_cell_num; ++i)
			deck.push_back(new Rum2Cell());

		for (int i = 0; i < rum3_cell_num; ++i)
			deck.push_back(new Rum3Cell());

		for (int i = 0; i < cave_cell_num; ++i)
			deck.push_back(new CaveCell());

		for (int i = 0; i < earthquake_cell_num; ++i)
			deck.push_back(new EarthquakeCell());

		for (int i = 0; i < jungle_cell_num; ++i)
			deck.push_back(new JungleCell());

		for (int i = 0; i < grass_cell_num; ++i)
			deck.push_back(new GrassCell());
	}

	Cell* pop_one()
	{
		if (deck.empty())
			return nullptr;

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(0, counter - 1);

		size_t num = dis(gen);
		Cell* ptr = deck[num];

		deck.removeAt(num);
		counter--;

		return ptr;
	}

private:
	QList<Cell*> deck;
	size_t counter;
};