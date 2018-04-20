#include "stdafx.h"
#include "zombieArena.h"
#include "Zombie.h"

Zombie* createHorde(int numZombies, IntRect arena)
{
	
	// start as rubbish
	Zombie* zombies = new Zombie[numZombies];

	//min and max spawning limits
	int maxY = arena.height - 20;
	int maxX = arena.width - 20;
	int minY = arena.top = 20;
	int minX = arena.left + 20;

	//setup our zombies
	for (int i = 0; i < numZombies; ++i) //i++
	{
		//which side should the zombie spawn
		srand((int)time(0) * i);// could just do once at begining
		int side = (rand() % 4);

		float x, y;
		switch (side)
		{
		case 0:
			//left 
			x = minX;
			y = (rand() % maxY) + minY;// TOP + RANDOM NUM BETWEEN 0 & HEIGHT
			break;

		case 1:
			//Right 
			x = maxX;
			y = (rand() % maxY) + minY;// TOP + RANDOM NUM BETWEEN 0 & HEIGHT
			break;

		case 2:
			//Top 
			x = (rand() % maxX) + minX;
			y = minY;// TOP + RANDOM NUM BETWEEN 0 & HEIGHT
			break;

		case 3:
			//bottom 
			x = (rand() % maxX) + minX;
			y = maxY; // TOP + RANDOM NUM BETWEEN 0 & HEIGHT
			break;

		}//end switch

		//zombie type - bloater, crawler or runner
		srand((int)time(0)*i * 2);
		int type = (rand() % 3);//random number between 0-2

		zombies[i].spawn(x, y, type, i);

	}//end zombie setup


	return zombies;
}