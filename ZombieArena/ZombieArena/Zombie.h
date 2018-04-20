#pragma once
#include <SFML\Graphics.hpp>
using namespace sf;

class Zombie
{
private:
	//speed for each type
	const float BLOATER_SPEED = 40;
	const float CHASER_SPEED = 80;
	const float CRAWLER_SPEED = 20;

	//zombie health
	const float BLOATER_HEALTH = 5;
	const float CHASER_HEALTH = 1;
	const float CRAWLER_HEALTH = 3;

	//SPEED VARIANCE
	const int MAX_VARIANCE = 30;
	const int OFFSET = 101 - MAX_VARIANCE;
	//where is zombie?
	Vector2f m_Position;

	//zombie sprite
	Sprite m_Sprite;

	//speed
	float m_Speed;

	//helath
	float m_Health;

	//is it alive?
	bool m_Alive;

public:
	//bullet hits zombie
	bool hit();

	//is Zombie alive
	bool isAlive();

	//spawn new zombie
	void spawn(float startX, float startY, int type, int seed);

	//display rectangle
	FloatRect getPosition();

	//copy of sprite to draw
	Sprite getSprite();
	//Update Zombie each frame
	void update(float elapsedTime, Vector2f playerLocation);




};