#include "stdafx.h"
#include "Zombie.h"
#include "TextureHolder.h"
#include<cstdlib>
#include<ctime>

using namespace std;

bool Zombie::hit()
{
	m_Health--;
	if (m_Health < 0)
	{
		//Dead
		m_Alive = false;
		m_Sprite.setTexture(TextureHolder::GetTexture("graphics/blood.png"));
		return true;

	}
	return false;
}

bool Zombie::isAlive()
{

	return m_Alive;
}

void Zombie::spawn(float startX, float startY, int type, int seed)
{
	switch (type)
	{
	case 0:
		//Bloater
		m_Sprite = Sprite(TextureHolder::GetTexture("graphics/bloater.png"));
		m_Speed = BLOATER_SPEED;
		m_Health = BLOATER_HEALTH;
		break;
	case 1 :
		//chaser
		m_Sprite = Sprite(TextureHolder::GetTexture("graphics/chaser.png"));
		m_Speed = CHASER_SPEED;
		m_Health = CHASER_HEALTH;
		break;
	case 2:
		//CRAWLER
		m_Sprite = Sprite(TextureHolder::GetTexture("graphics/crawler.png"));
		m_Speed = CRAWLER_SPEED;
		m_Health = CRAWLER_HEALTH;
		break;
	}//end case statment

	//make speed unique for each xombie
	srand((int)time(0)*seed);
	//crate speed modifier beetween 80 and 100
	float modifier = (rand() % MAX_VARIANCE) + OFFSET;
	//express as fraction of 1
	modifier /= 100;
	m_Speed *=modifier;

	//initial position
	m_Position.x = startX;
	m_Position.y = startY;
	//set origin position
	m_Sprite.setOrigin(25, 25);
	//set position
	m_Sprite.setPosition(m_Position);
	
}
FloatRect Zombie::getPosition()
{
	return m_Sprite.getGlobalBounds();
}



void Zombie::oldPosition()
{
	
	useLastPosition = true;
}

Vector2f Zombie::getCentre()
{
	return m_Position;
}
Sprite Zombie::getSprite()
{
	return m_Sprite;
}
void Zombie::update(float elapsedTime, Vector2f playerLocation)
{
	float playerX = playerLocation.x;
	float playerY = playerLocation.y;

	lastPosition = m_Position;
	//update Z position
	if (playerX > m_Position.x)
	{
		m_Position.x = m_Position.x + m_Speed * elapsedTime;
	}
	if (playerY > m_Position.y)
	{
		m_Position.y = m_Position.y + m_Speed * elapsedTime;
	}

	if (playerX < m_Position.x)
	{
		m_Position.x = m_Position.x - m_Speed * elapsedTime;
	}
	if (playerY < m_Position.y)
	{
		m_Position.y = m_Position.y - m_Speed * elapsedTime;
	}

	//move sprite
	if (useLastPosition == false)
	{
		m_Sprite.setPosition(m_Position);
	}
	else
	{
		m_Sprite.setPosition(lastPosition);
		useLastPosition = false;
	}

	//face in right direction
	float angle = (atan2(playerY - m_Position.y, playerX - m_Position.x) * 180) / 3.141;
	m_Sprite.setRotation(angle);

}
//end of zombie spawnage

