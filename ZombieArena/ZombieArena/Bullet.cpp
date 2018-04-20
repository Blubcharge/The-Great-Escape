#include "stdafx.h"
#include "bullet.h"

//the Constructor
Bullet::Bullet()
{
	m_BulletShape.setSize(Vector2f(5.0f, 5.0f));
}

void Bullet::stop()
{
	m_InFlight = false;
}

bool Bullet::isInFlight()
{
	return m_InFlight;
}

void Bullet::shoot(float startX, float startY, float targetX, float targetY)
{
	//keep track of our bullet
	m_InFlight = true;
	m_Position.x = startX;
	m_Position.y = startY;
	
	//calculates gradient of the flight path
	float gradient = (startX - targetX) / (startY - targetY);

	if (gradient < 0)
	{
		gradient *= -1.0f;
	}

	//calculate the ratio between x and y
	float ratioXY = m_BulletSpeed / (1.0f + gradient);

	//set the velocity horizontally and vertically
	m_BulletDistanceY = ratioXY;
	m_BulletDistanceX = ratioXY * gradient;

	//point the bullet in the right direction
	if (targetX < startX)
	{
		m_BulletDistanceX *= -1.0f;
	}
	
	if (targetY < startY)
	{
		m_BulletDistanceY *= -1.0f;
	}

	//set range of bullet
	float range = 1000.0f;
	m_MinX = startX - range;
	m_MaxX = startX + range;
	m_MinY = startY - range;
	m_MaxY = startY + range;
	m_BulletShape.setPosition(m_Position);
}

FloatRect Bullet::GetPosition()
{
	return m_BulletShape.getGlobalBounds();
}

RectangleShape Bullet::getShape()
{
	return m_BulletShape;
}

void Bullet::update(float esapsedTime)//delta time
{

	//update position variable

	m_Position.x += (m_BulletDistanceX * esapsedTime);
	m_Position.y += (m_BulletDistanceY * esapsedTime);

	//move actual sprite shape

	m_BulletShape.setPosition(m_Position);


	//check if we have gone out of range
	if (m_Position.x < m_MinX|| m_Position.x > m_MaxX || m_Position.y < m_MinY || m_Position.y > m_MaxY )
	{
		stop();
	}

}
