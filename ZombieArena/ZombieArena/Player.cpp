#include "stdafx.h"
#include "Player.h"
#include "TextureHolder.h"

Player::Player()
{
	m_Speed = START_SPEED;
	m_Health = START_HEALTH;
	m_MaxHealth = START_HEALTH;

	//associate texture with sprite
	//m_Texture.loadFromFile("graphics/player.png");
	//m_Sprite.setTexture(m_Texture);
	m_Sprite = Sprite(TextureHolder::GetTexture("graphics/player.png"));

	//set origin to centre for smooth rotation
	m_Sprite.setOrigin(15, 23);

}

void Player::spawn(IntRect arena, Vector2f resolution, int tileSize)
{
	//put player in middle of arena
	m_Position.x = 100;
	m_Position.y = 100;

	//copy arena details to players m_arena 
	m_Arena.left = arena.left;
	m_Arena.width = arena.width;
	m_Arena.top = arena.top;
	m_Arena.height = arena.height;

	//tile size
	m_TileSize = tileSize;

	//resolution
	m_Resolution.x = resolution.x;
	m_Resolution.y = resolution.y;

}

void Player::resetPlayerStats()
{
	m_Speed = START_SPEED;
	m_Health = START_HEALTH;
	m_MaxHealth = START_HEALTH;

	
}

bool Player::hit(Time timeHit)
{
	if (timeHit.asMilliseconds() - m_LastHit.asMilliseconds() > 200)
	{
		m_LastHit = timeHit;
		m_Health -= 10;
		return true;
	}
	else
	{
		return false;
	}
}

Time Player::getLastHitTime()
{
	return m_LastHit;
}

FloatRect Player::getPosition()
{
	return m_Sprite.getGlobalBounds();
}

FloatRect Player::setPosition(FloatRect NewPosition)
{
	m_Sprite.setPosition() = NewPosition;
}

Vector2f Player::getCentre()
{
	return m_Position;
}







float Player::getRotation()
{
	return m_Sprite.getRotation();
}

Sprite Player::getSprite()
{
	return m_Sprite;
}



//keyboard controls
void Player::moveLeft()
{
	m_LeftPressed = true;
}

void Player::moveRight()
{

	m_RightPressed = true;
}

void Player::moveUp()
{
	m_UpPressed = true;
}

void Player::moveDown()
{
	m_DownPressed = true;
}

void Player::stopLeft()
{
	m_LeftPressed = false;
}

void Player::stopRight()
{
	m_RightPressed = false;
}

void Player::stopUp()
{
	m_UpPressed = false;
}

void Player::stopDown()
{
	m_DownPressed = false;
}












void Player::update(float elapsedTime, Vector2i mousePosition)
{
	if (m_UpPressed)
	{
		m_Position.y -= m_Speed*elapsedTime;
	}
	if (m_DownPressed)
	{
		m_Position.y += m_Speed*elapsedTime;
	}
	if (m_LeftPressed)
	{
		m_Position.x -= m_Speed*elapsedTime;
	}
	if (m_RightPressed)
	{
		m_Position.x += m_Speed*elapsedTime;
	}


	m_Sprite.setPosition(m_Position);

	//keep player in arena
	if (m_Position.x > m_Arena.width - m_TileSize)
	{
		m_Position.x = m_Arena.width - m_TileSize;
	}


	//left
	if (m_Position.x < m_Arena.left + m_TileSize)
	{
		m_Position.x = m_Arena.left + m_TileSize;
	}

	//bottom

	if (m_Position.y > m_Arena.height - m_TileSize)
	{
		m_Position.y = m_Arena.height - m_TileSize;
	}


	//top
	if (m_Position.y < m_Arena.top + m_TileSize)
	{
		m_Position.y = m_Arena.top + m_TileSize;
	}



	//calculate rotation
	float angle = (atan2(mousePosition.y - m_Resolution.y / 2, mousePosition.x - m_Resolution.x / 2) * 180) / 3.141;
	m_Sprite.setRotation(angle);
}

void Player::upgradeSpeed()
{
	//20% upgrade
	m_Speed += (START_SPEED * 0.2);
}

void Player::upgradeHealth()
{
	//20%health
	m_MaxHealth = +(START_HEALTH *0.2);
}

void Player::increaseHealthLevel(int amount)
{
	m_Health +=amount;
	//but bellow max
	if (m_Health > m_MaxHealth)
	{
		m_Health = m_MaxHealth;
	}
}

int Player::getHealth()
{
	return m_Health;
}