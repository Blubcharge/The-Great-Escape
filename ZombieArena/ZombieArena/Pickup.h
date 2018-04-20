#pragma once
#include <SFML\Graphics.hpp>

using namespace sf;
class Pickup
{
private:
	//start value for health pickups
	const int HEALTH_SATART_VALUE = 50;
	const int AMMO_START_VALUE = 12;
	const int START_WAIT_TIME = 10;
	const int START_SECONDS_TO_LIVE = 5;
	//the sprite that represents thus pickup
	Sprite m_Sprite;
	
	//the arena it xits in
	IntRect m_Arena;

	//how much is this pickup worth?
	int m_Value;
	//what type of pickup is this
	//1 = health, 2 = ammo
	int m_Type;

	//handle spawning and disappearing
	bool m_Spawned;
	float m_SecondsSinceSpawn;
	float m_SecondsSinceDeSpawn;
	float m_SecondsToLive;
	float m_SecondsToWait;

	//public prototypes go here
public:
	Pickup::Pickup(int type);

	//prepare a new pickup
	void setArena(IntRect arena);
	void spawn();

	//check the position of a pickup
	FloatRect getPosition();

	//get the sprite for drawing
	Sprite getSprite();

	//let the pickup update itself each frame
	void update(float elapsedTime);

	//os tjos pickup currently spawned?
	bool isSpawned();

	//get the pickup
	int gotIt();

	//upgrade the value of each pickup
	void upgrade();
};