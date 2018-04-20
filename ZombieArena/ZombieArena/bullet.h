#pragma once
#include <SFML\Graphics.hpp>

using namespace sf;

class Bullet
{
private:
	//wherew is the bullet
	Vector2f m_Position;

	//boundong box of each bullet
	RectangleShape m_BulletShape;

	//is bullet active
	bool m_InFlight = false;

	//how fast is the bullet?
	float m_BulletSpeed = 1000;

	//what fraction of 1 pixel does the bullet travel,
	//Horizontally and vertically each frame??
	//these values will be derived from m_BulletSpeed
	float m_BulletDistanceX;
	float m_BulletDistanceY;

	//some boundaries so the bullet dosent fly forever
	float m_MaxX;
	float m_MaxY;
	float m_MinX;
	float m_MinY;

public:

	//constructor
	Bullet();

	//Stop the bullet
	void stop();

	//return the value of m_inFlight
	bool isInFlight();

	//launch a new bullet
	void shoot(float startX, float startY, 
		       float targetX, float targetY);

	//tell the calling code where the bullet is
	FloatRect GetPosition();

	//return the actual shape (for drawing)
	RectangleShape getShape();

	void update(float esapsedTime);
};